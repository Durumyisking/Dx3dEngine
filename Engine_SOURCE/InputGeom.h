#pragma once
#include "Engine.h"
#include "Mesh.h"

static const int MAX_CONVEXVOL_PTS = 12;
struct ConvexVolume
{
	float verts[MAX_CONVEXVOL_PTS * 3];
	float hmin, hmax;
	int nverts;
	int area;
};

struct BuildSettings
{
	// Cell size in world units
	float cellSize;
	// Cell height in world units
	float cellHeight;
	// Agent height in world units
	float agentHeight;
	// Agent radius in world units
	float agentRadius;
	// Agent max climb in world units
	float agentMaxClimb;
	// Agent max slope in degrees
	float agentMaxSlope;
	// Region minimum size in voxels.
	// regionMinSize = sqrt(regionMinArea)
	float regionMinSize;
	// Region merge size in voxels.
	// regionMergeSize = sqrt(regionMergeArea)
	float regionMergeSize;
	// Edge max length in world units
	float edgeMaxLen;
	// Edge max error in voxels
	float edgeMaxError;
	float vertsPerPoly;
	// Detail sample distance in voxels
	float detailSampleDist;
	// Detail sample max error in voxel heights.
	float detailSampleMaxError;
	// Partition type, see SamplePartitionType
	int partitionType;
	// Bounds of the area to mesh
	float navMeshBMin[3];
	float navMeshBMax[3];
	// Size of the tiles in voxels
	float tileSize;
};

class InputGeom
{
private:
	Mesh* mMesh;
	float mMeshBMin[3], mMeshBMax[3];
	BuildSettings mBuildSetting;
	bool mbHasBuildSettings;

	/// @name Off-Mesh connections.
	static const int MAX_OFFMESH_CONNECTIONS = 256;
	float mOffMeshConVerts[MAX_OFFMESH_CONNECTIONS * 3 * 2];
	float mOffMeshConRads[MAX_OFFMESH_CONNECTIONS];
	unsigned char mOffMeshConDirs[MAX_OFFMESH_CONNECTIONS];
	unsigned char mOffMeshConAreas[MAX_OFFMESH_CONNECTIONS];
	unsigned short mOffMeshConFlags[MAX_OFFMESH_CONNECTIONS];
	unsigned int mOffMeshConId[MAX_OFFMESH_CONNECTIONS];
	int mOffMeshConCount;

	/// @name Convex Volumes.
	static const int MAX_VOLUMES = 256;
	ConvexVolume mVolumes[MAX_VOLUMES];
	int mVolumeCount;

	bool loadMesh(const std::wstring& filepath);
	bool loadGeomSet(const std::wstring& filepath);

public:
	InputGeom();
	~InputGeom();


	bool Load(const std::wstring& filepath);
	bool SaveGeomSet(const BuildSettings* settings);

	/// Method to return static mesh data.
	const Mesh* GetMesh() const { return mMesh; }
	const float* GetMeshBoundsMin() const { return mMeshBMin; }
	const float* GetMeshBoundsMax() const { return mMeshBMax; }
	const float* GetNavMeshBoundsMin() const { return mbHasBuildSettings ? mBuildSetting.navMeshBMin : mMeshBMin; }
	const float* GetNavMeshBoundsMax() const { return mbHasBuildSettings ? mBuildSetting.navMeshBMax : mMeshBMax; }
	const BuildSettings* GetBuildSettings() const { return mbHasBuildSettings ? &mBuildSetting : 0; }

	/// @name Off-Mesh connections.
	int GetOffMeshConnectionCount() const { return mOffMeshConCount; }
	const float* GetOffMeshConnectionVerts() const { return mOffMeshConVerts; }
	const float* GetOffMeshConnectionRads() const { return mOffMeshConRads; }
	const unsigned char* GetOffMeshConnectionDirs() const { return mOffMeshConDirs; }
	const unsigned char* GetOffMeshConnectionAreas() const { return mOffMeshConAreas; }
	const unsigned short* GetOffMeshConnectionFlags() const { return mOffMeshConFlags; }
	const unsigned int* GetOffMeshConnectionId() const { return mOffMeshConId; }
	void AddOffMeshConnection(const float* spos, const float* epos, const float rad,
		unsigned char bidir, unsigned char area, unsigned short flags);
	void DeleteOffMeshConnection(int i);

	/// @name Box Volumes.
	int GetConvexVolumeCount() const { return mVolumeCount; }
	const ConvexVolume* GetConvexVolumes() const { return mVolumes; }
	void AddConvexVolume(const float* verts, const int nverts,
		const float minh, const float maxh, unsigned char area);
	void DeleteConvexVolume(int i);

private:
	// Explicitly disabled copy constructor and copy assignment operator.
	InputGeom(const InputGeom&);
	InputGeom& operator=(const InputGeom&);
};

