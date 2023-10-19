#include "NaviMesh.h"
#include "../External/Detour/include/DetourNavMesh.h"
#include "../External/Detour/include/DetourNavMeshQuery.h"

class GameObj;

class SoloMeshPathTool
{
public:
	SoloMeshPathTool();
	~SoloMeshPathTool();

	void Initialize(NaviMesh* sample);
	void Reset();
	void HandleMenu();
	void SetPosition(const math::Vector3 start, const math::Vector3 end);

	void SetPath(GameObj* obj);

	void recalc();

private:
	NaviMesh* mNaviMesh;
	
	dtNavMesh* mDetourMesh;
	dtNavMeshQuery* mDetourMeshQuery;

	dtQueryFilter mFilter;	

	dtStatus mPathFindStatus;

	static const int MAX_POLYS = 256;
	static const int MAX_SMOOTH = 2048;
	
	dtPolyRef mStartRef;
	dtPolyRef mEndRef;
	dtPolyRef mPolys[MAX_POLYS];
	dtPolyRef mParent[MAX_POLYS];
	int mNumPolys;
	float mPolyExt[3];
	float mSmoothPath[MAX_SMOOTH*3];
	int mNumSmoothPath;
	float mQueryPoly[4*3];

	float mStartPos[3];
	float mEndPos[3];
	bool mbStart;
	bool mbEnd;
};
