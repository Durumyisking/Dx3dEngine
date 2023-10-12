#include "SoloNavMesh.h"

#include "Model.h"
#include "Mesh.h"


SoloNavMesh::SoloNavMesh()
	: mDetailMesh(nullptr)
	, mPolyMesh(nullptr)
{
}

SoloNavMesh::SoloNavMesh(const SoloNavMesh& other)
	: mDetailMesh(nullptr)
	, mPolyMesh(nullptr)
{
}

SoloNavMesh::~SoloNavMesh()
{
}

void SoloNavMesh::Save(FILE* File)
{
}

void SoloNavMesh::Load(FILE* File)
{
}

bool SoloNavMesh::CreateByModel(Model* model)
{
	//- # Prepare the input triangle mesh.

	const std::vector<Mesh*> meshes = model->GetMeshes();
	std::vector<float> vertices;
	std::vector<int> indexes;
	int allVertexCount = 0;
	int allIndexCount = 0;
	int currentIndexOffset = 0;

	for (Mesh* mesh : meshes)
	{
		std::vector<Vertex> meshVertices;
		std::vector<UINT> meshIndexes;
		mesh->GetVerticesFromBuffer(&meshVertices);
		mesh->GetIndexesFromBuffer(&meshIndexes);
		int vertexCount = mesh->GetVertexCount();
		int indexCount = mesh->GetIndexCount();
		allVertexCount += vertexCount;
		allIndexCount += indexCount;

		// Copy from cvector array to PxVec3 array
		for (int i = 0; i < vertexCount; i++)
		{
			vertices.push_back(meshVertices[i].pos.x);
			vertices.push_back(meshVertices[i].pos.y);
			vertices.push_back(meshVertices[i].pos.z);
		}

		for (int i = 0; i < indexCount; i++)
		{
			indexes.emplace_back(static_cast<int>(meshIndexes[i]) + currentIndexOffset);
		}

		currentIndexOffset += vertexCount;
	}


	rcConfig cfg;
	memset(&cfg, 0, sizeof(cfg));

	// Configure the cfg based on your specific needs (size, agent height, walkable slope, etc.)
	// Example:
	cfg.cs = 0.3f;
	cfg.ch = 0.2f;
	cfg.walkableSlopeAngle = 45.0f;
	cfg.walkableHeight = (int)ceilf(2.0f / cfg.ch);
	cfg.walkableClimb = (int)floorf(0.9f / cfg.ch);
	cfg.walkableRadius = (int)ceilf(0.6f / cfg.cs);
	cfg.maxEdgeLen = (int)(12.0f / cfg.cs);
	cfg.maxSimplificationError = 1.3f;
	cfg.minRegionArea = (int)rcSqr(8);
	cfg.mergeRegionArea = (int)rcSqr(20);
	cfg.maxVertsPerPoly = 6;
	cfg.detailSampleDist = 6.0f < 0.9f ? 0 : cfg.cs * 6.0f;
	cfg.detailSampleMaxError = cfg.ch * 1.0f;

	// Calculate the bounding box of the input geometry
	rcVcopy(cfg.bmin, &vertices[0]);
	rcVcopy(cfg.bmax, &vertices[0]);
	for (int i = 1; i < vertices.size(); ++i)
	{
		rcVmin(cfg.bmin, &vertices[i]);
		rcVmax(cfg.bmax, &vertices[i]);
	}

	rcContext ctx;

	//- # Build a #rcHeightfield.
	rcHeightfield* solid = rcAllocHeightfield();
	if (!solid)
	{
		// Handle memory allocation failure
		return false;
	}

	if (!rcCreateHeightfield(&ctx, *solid, cfg.width, cfg.height, cfg.bmin, cfg.bmax, cfg.cs, cfg.ch))
	{
		// Handle error
		return false;
	}

	// Prepare triangle meshes for rcMarkWalkableTriangles
	// This step is important to mark which triangles are walkable based on user-defined settings
	const int* inTris = &indexes[0];
	const float* inVerts = &vertices[0];

	unsigned char* triAreas = new unsigned char[allIndexCount / 3];
	if (!triAreas)
	{
		// Handle memory allocation failure
		return false;
	}

	memset(triAreas, 0, allIndexCount / 3);

	rcMarkWalkableTriangles(&ctx, cfg.walkableSlopeAngle, inVerts, allVertexCount, inTris, allIndexCount / 3, triAreas);
	if (!rcRasterizeTriangles(&ctx, inVerts, allVertexCount, inTris, triAreas, allIndexCount / 3, *solid, cfg.walkableClimb))
	{
		// Handle rasterization error
		return false;
	}

	delete[] triAreas;

	// ... [Other Recast steps like filtering, compacting the heightfield, constructing contours, etc.]
	// Step 1: Filter the Walkable Surfaces
	rcFilterLowHangingWalkableObstacles(&ctx, cfg.walkableClimb, *solid);
	rcFilterLedgeSpans(&ctx, cfg.walkableHeight, cfg.walkableClimb, *solid);
	rcFilterWalkableLowHeightSpans(&ctx, cfg.walkableHeight, *solid);

	// Step 2: Compact the Heightfield
	rcCompactHeightfield* chf = rcAllocCompactHeightfield();
	if (!chf)
	{
		// Handle memory allocation failure
		return false;
	}

	if (!rcBuildCompactHeightfield(&ctx, cfg.walkableHeight, cfg.walkableClimb, *solid, *chf))
	{
		// Handle error during compacting
		return false;
	}

	// We won't need the solid heightfield anymore
	rcFreeHeightField(solid);
	solid = 0;

	// Erode the walkable area by agent radius.
	if (!rcErodeWalkableArea(&ctx, cfg.walkableRadius, *chf))
	{
		// Handle error during erosion
		return false;
	}

	// Step 3: Trace the Contours
	rcContourSet* cset = rcAllocContourSet();
	if (!cset)
	{
		// Handle memory allocation failure
		return false;
	}

	if (!rcBuildContours(&ctx, *chf, cfg.maxSimplificationError, cfg.maxEdgeLen, *cset))
	{
		// Handle error during contour creation
		return false;
	}

	// Step 4: Construct the Polygon Mesh
	rcPolyMesh* pmesh = rcAllocPolyMesh();
	if (!pmesh)
	{
		// Handle memory allocation failure
		return false;
	}

	if (!rcBuildPolyMesh(&ctx, *cset, cfg.maxVertsPerPoly, *pmesh))
	{
		// Handle error during polygon mesh creation
		return false;
	}

	// Step 5: Generate the Detail Mesh
	rcPolyMeshDetail* dmesh = rcAllocPolyMeshDetail();
	if (!dmesh)
	{
		// Handle memory allocation failure
		return false;
	}

	if (!rcBuildPolyMeshDetail(&ctx, *pmesh, *chf, cfg.detailSampleDist, cfg.detailSampleMaxError, *dmesh))
	{
		// Handle error during detail mesh creation
		return false;
	}

	// Clean up intermediate structures. 
	// Note: This is not a complete cleanup. Depending on the next steps (like creating a Detour navmesh), you might want to keep some structures.
	rcFreeCompactHeightfield(chf);
	rcFreeContourSet(cset);

	// Clean up
	rcFreeHeightField(solid);

	//rcPolyMesh and rcPolyMeshDetail 사용해서 dtNavMesh 구성해주기 :
	
	mDetailMesh = dmesh;
	mPolyMesh = pmesh;

	dtNavMesh dtNavMS; // 추가 작업

    return true;
}
