//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//


#include "SoloMeshPathTool.h"
#include "Engine.h"
#include "../External/Detour/include/RecastDebugDraw.h"
#include "../External/Detour/include/DetourNavMesh.h"
#include "../External/Detour/include/DetourNavMeshBuilder.h"
#include "../External/Detour/include/DetourDebugDraw.h"
#include "../External/Detour/include/DetourCommon.h"
#include "../External/Detour/include/DetourPathCorridor.h"
#include "GameObj.h"

inline bool InRange(const float* v1, const float* v2, const float r, const float h)
{
	const float dx = v2[0] - v1[0];
	const float dy = v2[1] - v1[1];
	const float dz = v2[2] - v1[2];
	return (dx*dx + dz*dz) < r*r && fabsf(dy) < h;
}

// This function checks if the path has a small U-turn, that is,
// a polygon further in the path is adjacent to the first polygon
// in the path. If that happens, a shortcut is taken.
// This can happen if the target (T) location is at tile boundary,
// and we're (S) approaching it parallel to the tile edge.
// The choice at the vertex can be arbitrary, 
//  +---+---+
//  |:::|:::|
//  +-S-+-T-+
//  |:::|   | <-- the step can end up in here, resulting U-turn path.
//  +---+---+
static int fixupShortcuts(dtPolyRef* path, int npath, dtNavMeshQuery* navQuery)
{
	if (npath < 3)
		return npath;

	// Get connected polygons
	static const int maxNeis = 16;
	dtPolyRef neis[maxNeis];
	int nneis = 0;

	const dtMeshTile* tile = 0;
	const dtPoly* poly = 0;
	if (dtStatusFailed(navQuery->getAttachedNavMesh()->getTileAndPolyByRef(path[0], &tile, &poly)))
		return npath;
	
	for (unsigned int k = poly->firstLink; k != DT_NULL_LINK; k = tile->links[k].next)
	{
		const dtLink* link = &tile->links[k];
		if (link->ref != 0)
		{
			if (nneis < maxNeis)
				neis[nneis++] = link->ref;
		}
	}

	// If any of the neighbour polygons is within the next few polygons
	// in the path, short cut to that polygon directly.
	static const int maxLookAhead = 6;
	int cut = 0;
	for (int i = dtMin(maxLookAhead, npath) - 1; i > 1 && cut == 0; i--) {
		for (int j = 0; j < nneis; j++)
		{
			if (path[i] == neis[j]) {
				cut = i;
				break;
			}
		}
	}
	if (cut > 1)
	{
		int offset = cut-1;
		npath -= offset;
		for (int i = 1; i < npath; i++)
			path[i] = path[i+offset];
	}

	return npath;
}

static bool getSteerTarget(dtNavMeshQuery* navQuery, const float* startPos, const float* endPos,
						   const float minTargetDist,
						   const dtPolyRef* path, const int pathSize,
						   float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef,
						   float* outPoints = 0, int* outPointCount = 0)							 
{
	// Find steer target.
	static const int MAX_STEER_POINTS = 3;
	float steerPath[MAX_STEER_POINTS*3];
	unsigned char steerPathFlags[MAX_STEER_POINTS];
	dtPolyRef steerPathPolys[MAX_STEER_POINTS];
	int nsteerPath = 0;
	navQuery->findStraightPath(startPos, endPos, path, pathSize,
							   steerPath, steerPathFlags, steerPathPolys, &nsteerPath, MAX_STEER_POINTS);
	if (!nsteerPath)
		return false;
		
	if (outPoints && outPointCount)
	{
		*outPointCount = nsteerPath;
		for (int i = 0; i < nsteerPath; ++i)
			dtVcopy(&outPoints[i*3], &steerPath[i*3]);
	}

	
	// Find vertex far enough to steer to.
	int ns = 0;
	while (ns < nsteerPath)
	{
		// Stop at Off-Mesh link or when point is further than slop away.
		if ((steerPathFlags[ns] & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ||
			!InRange(&steerPath[ns*3], startPos, minTargetDist, 1000.0f))
			break;
		ns++;
	}
	// Failed to find good point to steer to.
	if (ns >= nsteerPath)
		return false;
	
	dtVcopy(steerPos, &steerPath[ns*3]);
	steerPos[1] = startPos[1];
	steerPosFlag = steerPathFlags[ns];
	steerPosRef = steerPathPolys[ns];
	
	return true;
}


SoloMeshPathTool::SoloMeshPathTool() :
	mNaviMesh(0),
	mDetourMesh(0),
	mDetourMeshQuery(0),
	mPathFindStatus(DT_FAILURE),
	mStartRef(0),
	mEndRef(0),
	mNumPolys(0),
	mNumSmoothPath(0),
	mStartPos{0.f},
	mEndPos{0.f},
	mbStart(false),
	mbEnd(false)
{
	mFilter.setIncludeFlags(POLYFLAGS_ALL ^ POLYFLAGS_DISABLED);
	mFilter.setExcludeFlags(0);

	mPolyExt[0] = 2;
	mPolyExt[1] = 4;
	mPolyExt[2] = 2;
}

SoloMeshPathTool::~SoloMeshPathTool()
{
}

void SoloMeshPathTool::Initialize(NaviMesh* sample)
{
	Reset();

	mNaviMesh = sample;
	mDetourMesh = sample->GetNavMesh();
	mDetourMeshQuery = sample->GetNavMeshQuery();

	if (mDetourMeshQuery)
	{
		// Change costs.
		mFilter.setAreaCost(POLYAREA_GROUND, 1.0f);
		mFilter.setAreaCost(POLYAREA_WATER, 10.0f);
		mFilter.setAreaCost(POLYAREA_ROAD, 1.0f);
		mFilter.setAreaCost(POLYAREA_DOOR, 1.0f);
		mFilter.setAreaCost(POLYAREA_GRASS, 2.0f);
		mFilter.setAreaCost(POLYAREA_JUMP, 1.5f);
	}

	//옵션 설정 추가 필요
	{
		mFilter.setIncludeFlags(mFilter.getIncludeFlags() ^ POLYFLAGS_WALK);
		mFilter.setIncludeFlags(mFilter.getIncludeFlags() ^ POLYFLAGS_SWIM);
		mFilter.setIncludeFlags(mFilter.getIncludeFlags() ^ POLYFLAGS_DOOR);
		mFilter.setIncludeFlags(mFilter.getIncludeFlags() ^ POLYFLAGS_JUMP);

		//mFilter.setExcludeFlags(mFilter.getExcludeFlags() ^ POLYFLAGS_WALK);
		//mFilter.setExcludeFlags(mFilter.getIncludeFlags() ^ POLYFLAGS_SWIM);
		//mFilter.setExcludeFlags(mFilter.getIncludeFlags() ^ POLYFLAGS_DOOR);
		//mFilter.setExcludeFlags(mFilter.getIncludeFlags() ^ POLYFLAGS_JUMP);
	}
}

void SoloMeshPathTool::HandleMenu()
{
}

void SoloMeshPathTool::SetPosition(const math::Vector3 start, const math::Vector3 end)
{
	mStartPos[0] = start.x;
	mStartPos[1] = start.y;
	mStartPos[2] = start.z;
	mbStart = true;

	mEndPos[0] = end.x;
	mEndPos[1] = end.y;
	mEndPos[2] = end.z;
	mbEnd = true;
}

bool SoloMeshPathTool::SetPath(GameObj* obj)
{
	if (!recalc())
		return false;
	
	obj->UpdatePath(mSmoothPath, mNumSmoothPath);

	return true;
}

void SoloMeshPathTool::Reset()
{
	mStartPos[0] = 0.f;
	mStartPos[0] = 0.f;
	mStartPos[0] = 0.f;
	mbStart = false;

	mEndPos[0] = 0.f;
	mEndPos[0] = 0.f;
	mEndPos[0] = 0.f;
	mbEnd = false;

	mStartRef = 0;
	mEndRef = 0;
	mNumPolys = 0;
	mNumSmoothPath = 0;
}


bool SoloMeshPathTool::recalc()
{
	if (!mDetourMesh)
		return false;
	
	if (mbStart)
		mDetourMeshQuery->findNearestPoly(mStartPos, mPolyExt, &mFilter, &mStartRef, 0);
	else
		mStartRef = 0;

	if (mbEnd)
		mDetourMeshQuery->findNearestPoly(mEndPos, mPolyExt, &mFilter, &mEndRef, 0);
	else
		mEndRef = 0;
	
	mPathFindStatus = DT_FAILURE;
	
	if (mbStart && mbEnd && mStartRef && mEndRef)
	{
#ifdef DUMP_REQS
		printf("pi  %f %f %f  %f %f %f  0x%x 0x%x\n",
				mStartPos[0],mStartPos[1],mStartPos[2], mEndPos[0],mEndPos[1],mEndPos[2],
				mFilter.getIncludeFlags(), mFilter.getExcludeFlags()); 
#endif

		mDetourMeshQuery->findPath(mStartRef, mEndRef, mStartPos, mEndPos, &mFilter, mPolys, &mNumPolys, MAX_POLYS);

		mNumSmoothPath = 0;

		if (mNumPolys)
		{
			// Iterate over the path to find smooth path on the detail mesh surface.
			dtPolyRef polys[MAX_POLYS];
			memcpy(polys, mPolys, sizeof(dtPolyRef)*mNumPolys); 
			int npolys = mNumPolys;
				
			float iterPos[3], targetPos[3];
			mDetourMeshQuery->closestPointOnPoly(mStartRef, mStartPos, iterPos, 0);
			mDetourMeshQuery->closestPointOnPoly(polys[npolys-1], mEndPos, targetPos, 0);
				
			static const float STEP_SIZE = 0.5f;
			static const float SLOP = 0.01f;
				
			mNumSmoothPath = 0;
				
			dtVcopy(&mSmoothPath[mNumSmoothPath*3], iterPos);
			mNumSmoothPath++;
				
			// Move towards target a small advancement at a time until target reached or
			// when ran out of memory to store the path.
			while (npolys && mNumSmoothPath < MAX_SMOOTH)
			{
				// Find location to steer towards.
				float steerPos[3];
				unsigned char steerPosFlag;
				dtPolyRef steerPosRef;
					
				if (!getSteerTarget(mDetourMeshQuery, iterPos, targetPos, SLOP,
									polys, npolys, steerPos, steerPosFlag, steerPosRef))
					break;
					
				bool endOfPath = (steerPosFlag & DT_STRAIGHTPATH_END) ? true : false;
				bool offMeshConnection = (steerPosFlag & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ? true : false;
					
				// Find movement delta.
				float delta[3], len;
				dtVsub(delta, steerPos, iterPos);
				len = dtMathSqrtf(dtVdot(delta, delta));
				// If the steer target is end of path or off-mesh link, do not move past the location.
				if ((endOfPath || offMeshConnection) && len < STEP_SIZE)
					len = 1;
				else
					len = STEP_SIZE / len;
				float moveTgt[3];
				dtVmad(moveTgt, iterPos, delta, len);
					
				// Move
				float result[3];
				dtPolyRef visited[16];
				int nvisited = 0;
				mDetourMeshQuery->moveAlongSurface(polys[0], iterPos, moveTgt, &mFilter,
												result, visited, &nvisited, 16);

				npolys = dtMergeCorridorStartMoved(polys, npolys, MAX_POLYS, visited, nvisited);
				npolys = fixupShortcuts(polys, npolys, mDetourMeshQuery);

				float h = 0;
				mDetourMeshQuery->getPolyHeight(polys[0], result, &h);
				result[1] = h;
				dtVcopy(iterPos, result);

				// Handle end of path and off-mesh links when close enough.
				if (endOfPath && InRange(iterPos, steerPos, SLOP, 1.0f))
				{
					// Reached end of path.
					dtVcopy(iterPos, targetPos);
					if (mNumSmoothPath < MAX_SMOOTH)
					{
						dtVcopy(&mSmoothPath[mNumSmoothPath*3], iterPos);
						mNumSmoothPath++;
					}
					break;
				}
				else if (offMeshConnection && InRange(iterPos, steerPos, SLOP, 1.0f))
				{
					// Reached off-mesh connection.
					float startPos[3], endPos[3];
						
					// Advance the path up to and over the off-mesh connection.
					dtPolyRef prevRef = 0, polyRef = polys[0];
					int npos = 0;
					while (npos < npolys && polyRef != steerPosRef)
					{
						prevRef = polyRef;
						polyRef = polys[npos];
						npos++;
					}
					for (int i = npos; i < npolys; ++i)
						polys[i-npos] = polys[i];
					npolys -= npos;
						
					// Handle the connection.
					dtStatus status = mDetourMesh->getOffMeshConnectionPolyEndPoints(prevRef, polyRef, startPos, endPos);
					if (dtStatusSucceed(status))
					{
						if (mNumSmoothPath < MAX_SMOOTH)
						{
							dtVcopy(&mSmoothPath[mNumSmoothPath*3], startPos);
							mNumSmoothPath++;
							// Hack to make the dotted path not visible during off-mesh connection.
							if (mNumSmoothPath & 1)
							{
								dtVcopy(&mSmoothPath[mNumSmoothPath*3], startPos);
								mNumSmoothPath++;
							}
						}
						// Move position at the other side of the off-mesh link.
						dtVcopy(iterPos, endPos);
						float eh = 0.0f;
						mDetourMeshQuery->getPolyHeight(polys[0], iterPos, &eh);
						iterPos[1] = eh;
					}
				}
					
				// Store results.
				if (mNumSmoothPath < MAX_SMOOTH)
				{
					dtVcopy(&mSmoothPath[mNumSmoothPath*3], iterPos);
					mNumSmoothPath++;
				}
			}
		}


		return true;
	}
	else
	{
		mNumPolys = 0;
		mNumSmoothPath = 0;

		return false;
	}
}

static void GetPolyCenter(dtNavMesh* navMesh, dtPolyRef ref, float* center)
{
	center[0] = 0;
	center[1] = 0;
	center[2] = 0;
	
	const dtMeshTile* tile = 0;
	const dtPoly* poly = 0;
	dtStatus status = navMesh->getTileAndPolyByRef(ref, &tile, &poly);
	if (dtStatusFailed(status))
		return;
		
	for (int i = 0; i < (int)poly->vertCount; ++i)
	{
		const float* v = &tile->verts[poly->verts[i]*3];
		center[0] += v[0];
		center[1] += v[1];
		center[2] += v[2];
	}
	const float s = 1.0f / poly->vertCount;
	center[0] *= s;
	center[1] *= s;
	center[2] *= s;
}