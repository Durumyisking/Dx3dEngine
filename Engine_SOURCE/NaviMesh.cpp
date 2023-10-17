#include "NaviMesh.h"
#include "../External/Detour/include/DetourNavMesh.h"
#include "../External/Detour/include/DetourCrowd.h"
#include "../External/Detour/include/DetourCommon.h"
#include "../External/Detour/include/DetourNavMeshBuilder.h"
#include "../External/Detour/include/DetourNavMeshQuery.h"
#include "../External/Detour/include/DetourTileCacheBuilder.h"

#include "InputGeom.h"

NaviMesh::NaviMesh()
	: mGeom(nullptr)
	, mNavMesh(nullptr)
	, mNavQuery(nullptr)
	, mCrowd(nullptr)
	, mFilterLowHangingObstacles(true)
	, mFilterLedgeSpans(true)
	, mFilterWalkableLowHeightSpans(true)
{
	ResetCommonSettings();
	mNavQuery = dtAllocNavMeshQuery();
	mCrowd = dtAllocCrowd();

	for (int i = 0; i < MAX_TOOLS; i++)
	{
		mToolStates[i] = 0;
	}
}

NaviMesh::~NaviMesh()
{
	delete mGeom;
	mGeom = nullptr;

	dtFreeNavMeshQuery(mNavQuery);
	dtFreeNavMesh(mNavMesh);
	dtFreeCrowd(mCrowd);
	delete mTool;
	for (int i = 0; i < MAX_TOOLS; i++)
		delete mToolStates[i];
}

void NaviMesh::HandleMeshChanged(InputGeom* geom)
{
	mGeom = geom;

	const BuildSettings* buildSettings = geom->getBuildSettings();
	if (buildSettings)
	{
		mCellSize = buildSettings->cellSize;
		mCellHeight = buildSettings->cellHeight;
		mAgentHeight = buildSettings->agentHeight;
		mAgentRadius = buildSettings->agentRadius;
		mAgentMaxClimb = buildSettings->agentMaxClimb;
		mAgentMaxSlope = buildSettings->agentMaxSlope;
		mRegionMinSize = buildSettings->regionMinSize;
		mRegionMergeSize = buildSettings->regionMergeSize;
		mEdgeMaxLen = buildSettings->edgeMaxLen;
		mEdgeMaxError = buildSettings->edgeMaxError;
		mVertsPerPoly = buildSettings->vertsPerPoly;
		mDetailSampleDist = buildSettings->detailSampleDist;
		mDetailSampleMaxError = buildSettings->detailSampleMaxError;
		mPartitionType = buildSettings->partitionType;
	}
}


void NaviMesh::UpdateToolStates(const float dt)
{
	for (int i = 0; i < MAX_TOOLS; i++)
	{
		if (mToolStates[i])
			mToolStates[i]->handleUpdate(dt);
	}
}

void NaviMesh::InitToolStates(NaviMesh* sample)
{
	for (int i = 0; i < MAX_TOOLS; i++)
	{
		if (mToolStates[i])
			mToolStates[i]->init(sample);
	}
}

void NaviMesh::ResetToolStates()
{
	for (int i = 0; i < MAX_TOOLS; i++)
	{
		if (mToolStates[i])
			mToolStates[i]->reset();
	}
}

void NaviMesh::ResetCommonSettings()
{
	mCellSize = 0.88f;
	mCellHeight = 0.48f;
	mAgentHeight = 3.0f;
	mAgentRadius = 2.0f;
	mAgentMaxClimb = 1.2f;
	mAgentMaxSlope = 65.0f;
	mRegionMinSize = 8;
	mRegionMergeSize = 20;
	mEdgeMaxLen = 12.0f;
	mEdgeMaxError = 1.3f;
	mVertsPerPoly = 6.0f;
	mDetailSampleDist = 6.0f;
	mDetailSampleMaxError = 1.0f;
	mPartitionType = PartitionType::PARTITION_MONOTONE;
}

void NaviMesh::HandleCommonSettings()
{
	/*imguiLabel("Rasterization");
	imguiSlider("Cell Size", &m_cellSize, 0.1f, 1.0f, 0.01f);
	imguiSlider("Cell Height", &m_cellHeight, 0.1f, 1.0f, 0.01f);

	if (m_geom)
	{
		const float* bmin = m_geom->getNavMeshBoundsMin();
		const float* bmax = m_geom->getNavMeshBoundsMax();
		int gw = 0, gh = 0;
		rcCalcGridSize(bmin, bmax, m_cellSize, &gw, &gh);
		char text[64];
		snprintf(text, 64, "Voxels  %d x %d", gw, gh);
		imguiValue(text);
	}

	imguiSeparator();
	imguiLabel("Agent");
	imguiSlider("Height", &m_agentHeight, 0.1f, 5.0f, 0.1f);
	imguiSlider("Radius", &m_agentRadius, 0.0f, 5.0f, 0.1f);
	imguiSlider("Max Climb", &m_agentMaxClimb, 0.1f, 5.0f, 0.1f);
	imguiSlider("Max Slope", &m_agentMaxSlope, 0.0f, 90.0f, 1.0f);

	imguiSeparator();
	imguiLabel("Region");
	imguiSlider("Min Region Size", &m_regionMinSize, 0.0f, 150.0f, 1.0f);
	imguiSlider("Merged Region Size", &m_regionMergeSize, 0.0f, 150.0f, 1.0f);

	imguiSeparator();
	imguiLabel("Partitioning");
	if (imguiCheck("Watershed", m_partitionType == SAMPLE_PARTITION_WATERSHED))
		m_partitionType = SAMPLE_PARTITION_WATERSHED;
	if (imguiCheck("Monotone", m_partitionType == SAMPLE_PARTITION_MONOTONE))
		m_partitionType = SAMPLE_PARTITION_MONOTONE;
	if (imguiCheck("Layers", m_partitionType == SAMPLE_PARTITION_LAYERS))
		m_partitionType = SAMPLE_PARTITION_LAYERS;

	imguiSeparator();
	imguiLabel("Filtering");
	if (imguiCheck("Low Hanging Obstacles", m_filterLowHangingObstacles))
		m_filterLowHangingObstacles = !m_filterLowHangingObstacles;
	if (imguiCheck("Ledge Spans", m_filterLedgeSpans))
		m_filterLedgeSpans = !m_filterLedgeSpans;
	if (imguiCheck("Walkable Low Height Spans", m_filterWalkableLowHeightSpans))
		m_filterWalkableLowHeightSpans = !m_filterWalkableLowHeightSpans;

	imguiSeparator();
	imguiLabel("Polygonization");
	imguiSlider("Max Edge Length", &m_edgeMaxLen, 0.0f, 50.0f, 1.0f);
	imguiSlider("Max Edge Error", &m_edgeMaxError, 0.1f, 3.0f, 0.1f);
	imguiSlider("Verts Per Poly", &m_vertsPerPoly, 3.0f, 12.0f, 1.0f);

	imguiSeparator();
	imguiLabel("Detail Mesh");
	imguiSlider("Sample Distance", &m_detailSampleDist, 0.0f, 16.0f, 1.0f);
	imguiSlider("Max Sample Error", &m_detailSampleMaxError, 0.0f, 16.0f, 1.0f);

	imguiSeparator();*/
}

NaviMeshTool::~NaviMeshTool()
{
}

NaviMeshToolState::~NaviMeshToolState()
{
}

static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
static const int NAVMESHSET_VERSION = 1;

struct NavMeshSetHeader
{
	int magic;
	int version;
	int numTiles;
	dtNavMeshParams params;
};

struct NavMeshTileHeader
{
	dtTileRef tileRef;
	int dataSize;
};

dtNavMesh* NaviMesh::loadAll(const char* path)
{
	FILE* fp = nullptr;
	fopen_s(&fp, path, "rb");
	if (!fp) return 0;

	// Read header.
	NavMeshSetHeader header;
	size_t readLen = fread(&header, sizeof(NavMeshSetHeader), 1, fp);
	if (readLen != 1)
	{
		fclose(fp);
		return 0;
	}
	if (header.magic != NAVMESHSET_MAGIC)
	{
		fclose(fp);
		return 0;
	}
	if (header.version != NAVMESHSET_VERSION)
	{
		fclose(fp);
		return 0;
	}

	dtNavMesh* mesh = dtAllocNavMesh();
	if (!mesh)
	{
		fclose(fp);
		return 0;
	}
	dtStatus status = mesh->init(&header.params);
	if (dtStatusFailed(status))
	{
		fclose(fp);
		return 0;
	}

	// Read tiles.
	for (int i = 0; i < header.numTiles; ++i)
	{
		NavMeshTileHeader tileHeader;
		readLen = fread(&tileHeader, sizeof(tileHeader), 1, fp);
		if (readLen != 1)
		{
			fclose(fp);
			return 0;
		}

		if (!tileHeader.tileRef || !tileHeader.dataSize)
			break;

		unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
		if (!data) break;
		memset(data, 0, tileHeader.dataSize);
		readLen = fread(data, tileHeader.dataSize, 1, fp);
		if (readLen != 1)
		{
			dtFree(data);
			fclose(fp);
			return 0;
		}

		mesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
	}

	fclose(fp);

	return mesh;
}

void NaviMesh::saveAll(const char* path, const dtNavMesh* mesh)
{
	if (!mesh) return;

	FILE* fp = nullptr;
	fopen_s(&fp, path, "wb");
	if (!fp)
		return;

	// Store header.
	NavMeshSetHeader header;
	header.magic = NAVMESHSET_MAGIC;
	header.version = NAVMESHSET_VERSION;
	header.numTiles = 0;
	for (int i = 0; i < mesh->getMaxTiles(); ++i)
	{
		const dtMeshTile* tile = mesh->getTile(i);
		if (!tile || !tile->header || !tile->dataSize) continue;
		header.numTiles++;
	}
	memcpy(&header.params, mesh->getParams(), sizeof(dtNavMeshParams));
	fwrite(&header, sizeof(NavMeshSetHeader), 1, fp);

	// Store tiles.
	for (int i = 0; i < mesh->getMaxTiles(); ++i)
	{
		const dtMeshTile* tile = mesh->getTile(i);
		if (!tile || !tile->header || !tile->dataSize) continue;

		NavMeshTileHeader tileHeader;
		tileHeader.tileRef = mesh->getTileRef(tile);
		tileHeader.dataSize = tile->dataSize;
		fwrite(&tileHeader, sizeof(tileHeader), 1, fp);

		fwrite(tile->data, tile->dataSize, 1, fp);
	}

	fclose(fp);
}