#pragma once
#include "Entity.h"
#include "../External/Recast/include/Recast.h"
#include "../External/Detour/include/DetourNavMesh.h"
#include "../External/Detour/include/DetourCrowd.h"
#include "../External/Detour/include/DetourNavMeshBuilder.h"
#include "../External/Detour/include/DetourNavMeshQuery.h"
#include "../External/Detour/include/DetourTileCacheBuilder.h"

#include "InputGeom.h"

/// Tool types.
enum NaviMeshToolType
{
	TOOL_NONE = 0,
	TOOL_TILE_EDIT,
	TOOL_TILE_HIGHLIGHT,
	TOOL_TEMP_OBSTACLE,
	TOOL_NAVMESH_TESTER,
	TOOL_NAVMESH_PRUNE,
	TOOL_OFFMESH_CONNECTION,
	TOOL_CONVEX_VOLUME,
	TOOL_CROWD,
	MAX_TOOLS
};

/// These are just sample areas to use consistent values across the samples.
/// The use should specify these base on his needs.
enum PolyAreas
{
	POLYAREA_GROUND,
	POLYAREA_WATER,
	POLYAREA_ROAD,
	POLYAREA_DOOR,
	POLYAREA_GRASS,
	POLYAREA_JUMP
};
enum PolyFlags
{
	POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
	POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
	POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
	POLYFLAGS_JUMP = 0x08,		// Ability to jump.
	POLYFLAGS_DISABLED = 0x10,		// Disabled polygon
	POLYFLAGS_ALL = 0xffff	// All abilities.
};

enum PartitionType
{
	PARTITION_WATERSHED,
	PARTITION_MONOTONE,
	PARTITION_LAYERS
};

struct NaviMeshTool
{
	virtual ~NaviMeshTool();
	virtual int type() = 0;
	virtual void init(NaviMesh* sample) = 0;
	virtual void reset() = 0;
	virtual void handleMenu() = 0;
	virtual void handleClick(const float* s, const float* p, bool shift) = 0;
	virtual void handleRender() = 0;
	virtual void handleRenderOverlay(double* proj, double* model, int* view) = 0;
	virtual void handleToggle() = 0;
	virtual void handleStep() = 0;
	virtual void handleUpdate(const float dt) = 0;
};

struct NaviMeshToolState {
	virtual ~NaviMeshToolState();
	virtual void init(NaviMesh* sample) = 0;
	virtual void reset() = 0;
	virtual void handleRender() = 0;
	virtual void handleRenderOverlay(double* proj, double* model, int* view) = 0;
	virtual void handleUpdate(const float dt) = 0;
};

class NaviMesh :
    public Entity
{
public:
	NaviMesh();
	virtual ~NaviMesh();


	virtual void SetAgentRadius(float radius) { mAgentRadius = radius; }
	virtual void SetAgentHeight(float Height) { mAgentHeight = Height; }
	virtual void SetAgentClimb(float Climb) { mAgentMaxClimb = Climb; }
	virtual float GetAgentRadius() { return mAgentRadius; }
	virtual float GetAgentHeight() { return mAgentHeight; }
	virtual float GetAgentClimb() { return mAgentMaxClimb; }

	virtual dtNavMesh* getNavMesh() { return mNavMesh; }
	virtual dtNavMeshQuery* getNavMeshQuery() { return mNavQuery; }
	virtual dtCrowd* getCrowd() { return mCrowd; }

	void UpdateToolStates(const float dt);
	void InitToolStates(NaviMesh* sample);
	void ResetToolStates();

	void ResetCommonSettings();
	void handleCommonSettings();

protected:
	dtNavMesh* loadAll(const char* path);
	void saveAll(const char* path, const dtNavMesh* mesh);

protected:
	InputGeom* mGeom;

	dtNavMesh* mNavMesh;
	dtNavMeshQuery* mNavQuery;
	dtCrowd* mCrowd;

	float mCellSize;
	float mCellHeight;
	float mAgentHeight;
	float mAgentRadius;
	float mAgentMaxClimb;
	float mAgentMaxSlope;
	float mRegionMinSize;
	float mRegionMergeSize;
	float mEdgeMaxLen;
	float mEdgeMaxError;
	float mVertsPerPoly;
	float mDetailSampleDist;
	float mDetailSampleMaxError;
	int	  mPartitionType;

	bool mFilterLowHangingObstacles;
	bool mFilterLedgeSpans;
	bool mFilterWalkableLowHeightSpans;

	NaviMeshTool* mTool;
	NaviMeshToolState* mToolStates[MAX_TOOLS];
};

