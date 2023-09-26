#pragma once
#include "Engine.h"
#include "SimpleMath.h"
#include "../External/Recast/include/Recast.h"
#include "../External/Detour/include/DetourNavMesh.h"
#include "../External/Detour/include/DetourNavMeshBuilder.h"

class Model;
class NavigationMgr
{
	SINGLE(NavigationMgr)

public:
	void Update();
	void FixedUpdate();

	void CreateNavigationMesh(Model* model);
	void SetRecast(math::Vector3* verts, int vertCount, int* indices, int indexCount);
	
	void Navigation();

private:

};