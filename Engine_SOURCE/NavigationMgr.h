#pragma once
#include "Engine.h"
#include "SimpleMath.h"
#include "../External/Recast/include/Recast.h"
#include "../External/Detour/include/DetourNavMesh.h"
#include "../External/Detour/include/DetourNavMeshBuilder.h"

#include "SoloNaviMesh.h"

class Model;
class NavigationMgr
{
	SINGLE(NavigationMgr)

public:
	void Update();
	void FixedUpdate();

	SoloNaviMesh* CreateNavigationMesh(const std::wstring& name);
	
	void Navigation();

	template<typename T>
	T* GetNaviMesh(const std::wstring& name)
	{
		auto iter = mNavMeshes.find(name);
		if (iter == mNavMeshes.end())
			return nullptr;

		return dynamic_cast<T*>(iter->second);
	}

private:
	std::map<std::wstring, SoloNaviMesh*> mNavMeshes;
};