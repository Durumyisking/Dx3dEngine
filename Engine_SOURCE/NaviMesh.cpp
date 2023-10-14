#include "NaviMesh.h"

NaviMesh::NaviMesh()
	: mGeom(0)
	, mNavMesh(0)
	, mNavQuery(0)
	, mCrowd(0)
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
	mCellSize = 0.3f;
	mCellHeight = 0.2f;
	mAgentHeight = 2.0f;
	mAgentRadius = 0.6f;
	mAgentMaxClimb = 0.9f;
	mAgentMaxSlope = 45.0f;
	mRegionMinSize = 8;
	mRegionMergeSize = 20;
	mEdgeMaxLen = 12.0f;
	mEdgeMaxError = 1.3f;
	mVertsPerPoly = 6.0f;
	mDetailSampleDist = 6.0f;
	mDetailSampleMaxError = 1.0f;
	mPartitionType = PARTITION_WATERSHED;
}

void NaviMesh::handleCommonSettings()
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
