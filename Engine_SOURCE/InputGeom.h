#pragma once
#include "Engine.h"

struct BuildSettings
{
	float cellSize; // Cell size in world units
	
	float cellHeight; // Cell height in world units
	
	float agentHeight; // Agent height in world units
	
	float agentRadius; // Agent radius in world units
	
	float agentMaxClimb; // Agent max climb in world units
	
	float agentMaxSlope; // Agent max slope in degrees
	
	float regionMinSize; // Region minimum size in voxels. regionMinSize = sqrt(regionMinArea)
	
	float regionMergeSize; // Region merge size in voxels. regionMergeSize = sqrt(regionMergeArea)
	
	float edgeMaxLen; // Edge max length in world units
	
	float edgeMaxError; // Edge max error in voxels
	float vertsPerPoly; 
	
	float detailSampleDist; // Detail sample distance in voxels
	
	float detailSampleMaxError; // Detail sample max error in voxel heights.
	
	int partitionType; // Partition type, see SamplePartitionType
	
	float navMeshBMin[3]; // Bounds of the area to mesh
	float navMeshBMax[3];
	
	float tileSize; // Size of the tiles in voxels
};

class GameObj;
class Mesh;
class Model;

class InputGeom
{
public:
	InputGeom();
	~InputGeom();

	bool CreateGeom(const std::wstring& name, GameObj* obj);
	bool CalcModelVertex(math::Matrix world, Model* model);
	bool CalcMeshVertexes(math::Matrix world, Mesh* mesh);

	const std::wstring& GetName() { return mGeomName; }
	const float* GetVerts() const { return mVerts.data(); }
	const int GetNumVerts() const { return mNumVerts; }
	const int* GetTriangles() const { return mTriangles.data(); }
	const int GetNumTriangles() const { return mNumTriangles; }

	/// Method to return static mesh data.
	//const Mesh* GetMesh(UINT index) const { if(mMeshes.size() >= index) return mMeshes[index]; }
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

private:
	std::wstring mGeomName;	
	std::vector<float> mVerts;
	int mNumVerts;
	std::vector<int> mTriangles;
	int mNumTriangles;
	int mArea;
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


	// Explicitly disabled copy constructor and copy assignment operator.
	InputGeom(const InputGeom&);
	InputGeom& operator=(const InputGeom&);
};

