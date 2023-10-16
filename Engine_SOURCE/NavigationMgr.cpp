#include "NavigationMgr.h"
#include "Mesh.h"
#include "Graphics.h"
#include "Model.h"

NavigationMgr::NavigationMgr()
{
}

NavigationMgr::~NavigationMgr()
{
	for (auto navMesh : mNavMeshes)
	{
		navMesh.second->Clear();

		delete navMesh.second;
		navMesh.second = nullptr;
	}

	mNavMeshes.clear();
}

void NavigationMgr::Update()
{
}

void NavigationMgr::FixedUpdate()
{
}

SoloNaviMesh* NavigationMgr::CreateNavigationMesh(const std::wstring& name)
{
	SoloNaviMesh* soloMesh = new SoloNaviMesh();

	if (soloMesh == nullptr)
		return nullptr;

	mNavMeshes.insert(std::make_pair(name, soloMesh));

	return soloMesh;
}

void NavigationMgr::Navigation()
{
}

//수동으로 Recast NaviMesh 정보를 넣을경우의 예시
void ManualOperation()
{
	//vertex 정보 triangle 정보

	//	- # Build a #rcHeightfield.
	//@see rcAllocHeightfield, rcFreeHeightField, rcCreateHeightfield
	rcHeightfield hf;

	const float* orig = hf.bmin;
	const float cs = hf.cs;
	const float ch = hf.ch;

	const int w = hf.width;
	const int h = hf.height;

	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			// Deriving the minimum corner of the grid location.
			float fx = orig[0] + x * cs;
			float fz = orig[2] + y * cs;
			// The base span in the column. (May be null.)
			const rcSpan* s = hf.spans[x + y * w];
			while (s)
			{
				// Detriving the minium and maximum world position of the span.
				float fymin = orig[1] + s->smin * ch;
				float fymax = orig[1] + s->smax * ch;
				// Do other things with the span before moving up the column.
				s = s->next;
			}
		}
	}

	//	- # Build a #rcCompactHeightfield.
	//@see rcAllocCompactHeightfield, rcFreeCompactHeightfield, rcBuildCompactHeightfield
	rcCompactHeightfield chf;
	// Where chf is an instance of a rcCompactHeightfield.

	const float chcs = chf.cs;
	const float chch = chf.ch;

	for (int y = 0; y < chf.height; ++y)
	{
		for (int x = 0; x < chf.width; ++x)
		{
			// Deriving the minimum corner of the grid location.
			const float fx = chf.bmin[0] + x * chcs;
			const float fz = chf.bmin[2] + y * chcs;

			// Get the cell for the grid location then iterate
			// up the column.
			const rcCompactCell& c = chf.cells[x + y * chf.width];
			for (unsigned i = c.index, ni = c.index + c.count; i < ni; ++i)
			{
				const rcCompactSpan& s = chf.spans[i];

				//Deriving the minimum(floor) of the span.
				const float fy = chf.bmin[1] + (s.y + 1) * chch;

				// Testing the area assignment of the span.
				if (chf.areas[i] == RC_WALKABLE_AREA)
				{
					// The span is in the default 'walkable area'.
				}
				else if (chf.areas[i] == RC_NULL_AREA)
				{
					// The surface is not considered walkable.
					// E.g. It was filtered out during the build processes.
				}
				else
				{
					// Do something. (Only applicable for custom build
					// build processes.)
				}

				// Iterating the connected axis-neighbor spans.
				for (int dir = 0; dir < 4; ++dir)
				{
					if (rcGetCon(s, dir) != RC_NOT_CONNECTED)
					{
						// There is a neighbor in this direction.
						const int nx = x + rcGetDirOffsetX(dir);
						const int ny = y + rcGetDirOffsetY(dir);
						const int ni = (int)chf.cells[nx + ny * w].index + rcGetCon(s, 0);
						const rcCompactSpan& ns = chf.spans[ni];
						// Do something with the neighbor span.
					}
				}
			}
		}
	}

	//	- # Build a #rcContourSet.
	//@see rcAllocContourSet, rcFreeContourSet, rcBuildContours
	rcContourSet cset;
	rcContour* conts = cset.conts;

	float worldX = cset.bmin[0] + conts->verts[0] * cset.cs;
	float worldY = cset.bmin[1] + conts->verts[1] * cset.ch;
	float worldZ = cset.bmin[2] + conts->verts[2] * cset.cs;

	int r = conts->verts[conts->nverts * 4 + 3];

	int regionId = r & RC_CONTOUR_REG_MASK;

	if (r & RC_BORDER_VERTEX)
	{
		// The edge represents a solid border.
	}

	if (r & RC_AREA_BORDER)
	{
		// The edge represents a transition between different areas.
	}

	//	- # Build a #rcPolyMesh.
	rcPolyMesh mesh;

	const int nvp = mesh.nvp;
	const float mcs = mesh.cs;
	const float mch = mesh.ch;
	const float* morig = mesh.bmin;

	for (int i = 0; i < mesh.npolys; ++i)
	{
		const unsigned short* p = &mesh.polys[i * nvp * 2];

		// Iterate the vertices.
		unsigned short vi[3];  // The vertex indices.
		for (int j = 0; j < nvp; ++j)
		{
			if (p[j] == RC_MESH_NULL_IDX)
				break; // End of vertices.

			if (p[j + nvp] == RC_MESH_NULL_IDX)
			{
				// The edge beginning with this vertex is a solid border.
			}
			else
			{
				// The edge beginning with this vertex connects to 
				// polygon p[j + nvp].
			}

			// Convert to world space.
			const unsigned short* v = &mesh.verts[p[j] * 3];
			const float x = morig[0] + v[0] * mcs;
			const float y = morig[1] + v[1] * mch;
			const float z = morig[2] + v[2] * mcs;
			// Do something with the vertices.
		}
	}
	//The values of #bmin, #cs, and #ch are used to convert vertex coordinates to world space as follows :
	//float worldX = bmin[0] + verts[i * 3 + 0] * cs;
	//float worldY = bmin[1] + verts[i * 3 + 1] * ch;
	//float worldZ = bmin[2] + verts[i * 3 + 2] * cs;


	//	- # Build a #rcPolyMeshDetail.

	rcPolyMeshDetail dmesh;

	// Where dmesh is a reference to a rcPolyMeshDetail object.

	// Iterate the sub-meshes. (One for each source polygon.)
	for (int i = 0; i < dmesh.nmeshes; ++i)
	{
		const unsigned int* meshDef = &dmesh.meshes[i * 4];
		const unsigned int baseVerts = meshDef[0];
		const unsigned int baseTri = meshDef[2];
		const int ntris = (int)meshDef[3];

		const float* verts = &dmesh.verts[baseVerts * 3];
		const unsigned char* tris = &dmesh.tris[baseTri * 4];

		// Iterate the sub-mesh's triangles.
		for (int j = 0; j < ntris; ++j)
		{
			const float x = verts[tris[j * 4 + 0] * 3];
			const float y = verts[tris[j * 4 + 1] * 3];
			const float z = verts[tris[j * 4 + 2] * 3];
			// Do something with the vertex.
		}
	}
	//@var rcPolyMeshDetail::verts
	//	[(x, y, z) * #nverts]

	//	- # Use the rcPolyMesh and rcPolyMeshDetail to build a Detour navigation mesh
	//	tile.
}