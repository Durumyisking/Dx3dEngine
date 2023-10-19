#include "SoloNaviMesh.h"
#include "../External/Detour/include/DetourNavMesh.h"
#include "../External/Detour/include/DetourCrowd.h"
#include "../External/Detour/include/DetourCommon.h"
#include "../External/Detour/include/DetourNavMeshBuilder.h"
#include "../External/Detour/include/DetourNavMeshQuery.h"
#include "../External/Detour/include/DetourTileCacheBuilder.h"

#include "InputGeom.h"
#include "SoloMeshPathTool.h"

SoloNaviMesh::SoloNaviMesh()
	: NaviMesh()
	, mTriAreas(nullptr)
	, mHeightField(nullptr)
	, mCompactHeightField(nullptr)
	, mContourSet(nullptr)
	, mPolyMesh(nullptr)
	, mDetailMesh(nullptr)
	, mKeepInterResults(false)
	, mConfig()
{
}

SoloNaviMesh::~SoloNaviMesh()
{
}

void SoloNaviMesh::HandleSettings()
{
	NaviMesh::HandleCommonSettings();
}


void SoloNaviMesh::saveAll()
{
	NaviMesh::saveAll("solo_navmesh.bin", mNavMesh);
}

void SoloNaviMesh::loadAll()
{
	dtFreeNavMesh(mNavMesh);
	mNavMesh = NaviMesh::loadAll("solo_navmesh.bin");
	mNavQuery->init(mNavMesh, 2048);
}

bool SoloNaviMesh::Build()
{
	if (!mGeom || !mGeom->getMesh())
	{
		//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Input mesh is not specified.");
		std::cout<< "buildNavigation: Input mesh is not specified." << std::endl;
		return false;
	}

	Clear();

	const float* bmin = mGeom->getNavMeshBoundsMin();
	const float* bmax = mGeom->getNavMeshBoundsMax();
	const float* verts = mGeom->getMesh()->getVerts();
	const int nverts = mGeom->getMesh()->getVertCount();
	const int* tris = mGeom->getMesh()->getTris();
	const int ntris = mGeom->getMesh()->getTriCount();

	//build settings
	memset(&mConfig, 0, sizeof(mConfig));
	mConfig.cs = mCellSize;
	mConfig.ch = mCellHeight;
	mConfig.walkableSlopeAngle = mAgentMaxSlope;
	mConfig.walkableHeight = (int)ceilf(mAgentHeight / mConfig.ch);
	mConfig.walkableClimb = (int)floorf(mAgentMaxClimb / mConfig.ch);
	mConfig.walkableRadius = (int)ceilf(mAgentRadius / mConfig.cs);
	mConfig.maxEdgeLen = (int)(mEdgeMaxLen / mCellSize);
	mConfig.maxSimplificationError = mEdgeMaxError;
	mConfig.minRegionArea = (int)rcSqr(mRegionMinSize);		// Note: area = size*size
	mConfig.mergeRegionArea = (int)rcSqr(mRegionMergeSize);	// Note: area = size*size
	mConfig.maxVertsPerPoly = (int)mVertsPerPoly;
	mConfig.detailSampleDist = mDetailSampleDist < 0.9f ? 0 : mCellSize * mDetailSampleDist;
	mConfig.detailSampleMaxError = mCellHeight * mDetailSampleMaxError;

	// Set the area where the navigation will be build.
	// Here the bounds of the input mesh are used, but the
	// area could be specified by an user defined box, etc.
	rcVcopy(mConfig.bmin, bmin);
	rcVcopy(mConfig.bmax, bmax);
	rcCalcGridSize(mConfig.bmin, mConfig.bmax, mConfig.cs, &mConfig.width, &mConfig.height);


	mHeightField = rcAllocHeightfield();
	if (!mHeightField)
	{
		//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'solid'.");
		std::cout << "buildNavigation: Out of memory 'solid'." << std::endl;
		return false;
	}
	if (!rcCreateHeightfield(m_ctx, *mHeightField, mConfig.width, mConfig.height, mConfig.bmin, mConfig.bmax, mConfig.cs, mConfig.ch))
	{
		std::cout << "buildNavigation: Could not create solid heightfield." << std::endl;
		return false;
	}
	
	mTriAreas = new unsigned char[ntris];
	if (!mTriAreas)
	{
		std::cout << "buildNavigation: Out of memory 'mTriAreas'" << ntris << std::endl;
		return false;
	}

	memset(mTriAreas, 0, ntris * sizeof(unsigned char));
	rcMarkWalkableTriangles(m_ctx, mConfig.walkableSlopeAngle, verts, nverts, tris, ntris, mTriAreas);
	if (!rcRasterizeTriangles(m_ctx, verts, nverts, tris, mTriAreas, ntris, *mHeightField, mConfig.walkableClimb))
	{
		std::cout << "buildNavigation: Could not rasterize triangles." << std::endl;
		return false;
	}

	if (!mKeepInterResults)
	{
		delete[] mTriAreas;
		mTriAreas = 0;
	}

	if (mFilterLowHangingObstacles)
		rcFilterLowHangingWalkableObstacles(m_ctx, mConfig.walkableClimb, *mHeightField);
	if (mFilterLedgeSpans)
		rcFilterLedgeSpans(m_ctx, mConfig.walkableHeight, mConfig.walkableClimb, *mHeightField);
	if (mFilterWalkableLowHeightSpans)
		rcFilterWalkableLowHeightSpans(m_ctx, mConfig.walkableHeight, *mHeightField);


	mCompactHeightField = rcAllocCompactHeightfield();
	if (!mCompactHeightField)
	{
		std::cout << "buildNavigation: Out of memory 'chf'." << std::endl;
		return false;
	}
	if (!rcBuildCompactHeightfield(m_ctx, mConfig.walkableHeight, mConfig.walkableClimb, *mHeightField, *mCompactHeightField))
	{
		std::cout << "buildNavigation: Could not build compact data." << std::endl;
		return false;
	}

	if (!mKeepInterResults)
	{
		rcFreeHeightField(mHeightField);
		mHeightField = 0;
	}

	// Erode the walkable area by agent radius.
	if (!rcErodeWalkableArea(m_ctx, mConfig.walkableRadius, *mCompactHeightField))
	{
		std::cout << "buildNavigation: Could not erode." << std::endl;
		return false;
	}


	// Partition the heightfield so that we can use simple algorithm later to triangulate the walkable areas.
	// There are 3 partitioning methods, each with some pros and cons:
	// 1) Watershed partitioning
	//   - the classic Recast partitioning
	//   - creates the nicest tessellation
	//   - usually slowest
	//   - partitions the heightfield into nice regions without holes or overlaps
	//   - the are some corner cases where this method creates produces holes and overlaps
	//      - holes may appear when a small obstacles is close to large open area (triangulation can handle this)
	//      - overlaps may occur if you have narrow spiral corridors (i.e stairs), this make triangulation to fail
	//   * generally the best choice if you precompute the navmesh, use this if you have large open areas
	// 2) Monotone partitioning
	//   - fastest
	//   - partitions the heightfield into regions without holes and overlaps (guaranteed)
	//   - creates long thin polygons, which sometimes causes paths with detours
	//   * use this if you want fast navmesh generation
	// 3) Layer partitoining
	//   - quite fast
	//   - partitions the heighfield into non-overlapping regions
	//   - relies on the triangulation code to cope with holes (thus slower than monotone partitioning)
	//   - produces better triangles than monotone partitioning
	//   - does not have the corner cases of watershed partitioning
	//   - can be slow and create a bit ugly tessellation (still better than monotone)
	//     if you have large open areas with small obstacles (not a problem if you use tiles)
	//   * good choice to use for tiled navmesh with medium and small sized tiles

	if (mPartitionType == PARTITION_WATERSHED)
	{
		// Prepare for region partitioning, by calculating distance field along the walkable surface.
		if (!rcBuildDistanceField(m_ctx, *mCompactHeightField))
		{
			std::cout << "buildNavigation: Could not build distance field." << std::endl;
			return false;
		}

		// Partition the walkable surface into simple regions without holes.
		if (!rcBuildRegions(m_ctx, *mCompactHeightField, 0, mConfig.minRegionArea, mConfig.mergeRegionArea))
		{
			std::cout << "buildNavigation: Could not build watershed regions." << std::endl;
			return false;
		}
	}
	else if (mPartitionType == PARTITION_MONOTONE)
	{
		// Partition the walkable surface into simple regions without holes.
		// Monotone partitioning does not need distancefield.
		if (!rcBuildRegionsMonotone(m_ctx, *mCompactHeightField, 0, mConfig.minRegionArea, mConfig.mergeRegionArea))
		{
			std::cout << "buildNavigation: Could not build monotone regions." << std::endl;
			return false;
		}
	}
	else // PARTITION_LAYERS
	{
		if (!rcBuildLayerRegions(m_ctx, *mCompactHeightField, 0, mConfig.minRegionArea))
		{
			std::cout << "buildNavigation: Could not build layer regions." << std::endl;
			return false;
		}
	}

	// Create contours.
	mContourSet = rcAllocContourSet();
	if (!mContourSet)
	{
		std::cout << "buildNavigation: Out of memory 'cset'." << std::endl;
		return false;
	}
	if (!rcBuildContours(m_ctx, *mCompactHeightField, mConfig.maxSimplificationError, mConfig.maxEdgeLen, *mContourSet))
	{
		std::cout << "buildNavigation: Could not create contours." << std::endl;
		return false;
	}

	// Build polygon navmesh from the contours.
	mPolyMesh = rcAllocPolyMesh();
	if (!mPolyMesh)
	{
		std::cout << "buildNavigation: Out of memory 'pmesh'." << std::endl;
		return false;
	}
	if (!rcBuildPolyMesh(m_ctx, *mContourSet, mConfig.maxVertsPerPoly, *mPolyMesh))
	{
		std::cout << "buildNavigation: Could not triangulate contours." << std::endl;
		return false;
	}

	// Create detail mesh which allows to access approximate height on each polygon.
	mDetailMesh = rcAllocPolyMeshDetail();
	if (!mDetailMesh)
	{
		std::cout << "buildNavigation: Out of memory 'pmdtl'." << std::endl;
		return false;
	}

	if (!rcBuildPolyMeshDetail(m_ctx, *mPolyMesh, *mCompactHeightField, mConfig.detailSampleDist, mConfig.detailSampleMaxError, *mDetailMesh))
	{
		std::cout << "buildNavigation: Could not build detail mesh." << std::endl;
		return false;
	}

	if (!mKeepInterResults)
	{
		rcFreeCompactHeightfield(mCompactHeightField);
		mCompactHeightField = 0;
		rcFreeContourSet(mContourSet);
		mContourSet = 0;
	}

	// At this point the navigation mesh data is ready, you can access it from m_pmesh.
	// See duDebugDrawPolyMesh or dtCreateNavMeshData as examples how to access the data.

	//
	// (Optional) Create Detour data from Recast poly mesh.
	//


	// Show performance stats.
	std::cout << ">> Polymesh: "<< mPolyMesh->nverts << "  vertices" << mPolyMesh->npolys << "  polygons" << std::endl;

	if (mTool)
		mTool->Initialize(this);

	return true;
}

void SoloNaviMesh::Clear()
{
	delete[] mTriAreas;
	mTriAreas = nullptr;
	rcFreeHeightField(mHeightField);
	mHeightField = nullptr;
	rcFreeCompactHeightfield(mCompactHeightField);
	mCompactHeightField = nullptr;
	rcFreeContourSet(mContourSet);
	mContourSet = nullptr;
	rcFreePolyMesh(mPolyMesh);
	mPolyMesh = nullptr;
	rcFreePolyMeshDetail(mDetailMesh);
	mDetailMesh = nullptr;
	dtFreeNavMesh(mNavMesh);
	mNavMesh = nullptr;
}
