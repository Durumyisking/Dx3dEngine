#pragma once
#include "Engine.h"
#include "SimpleMath.h"
#include "SoloNaviMesh.h"

class GameObj;

class NavigationMgr
{
	SINGLE(NavigationMgr)

public:
	SoloNaviMesh* CreateNavigationMesh();
	SoloNaviMesh* CreateNavigationMesh(const std::wstring& name);

	bool SettingMesh(SoloNaviMesh* soloMesh, std::wstring path);

	bool FindPath(GameObj* obj, math::Vector3 end);
	bool FindPath(GameObj* obj, math::Vector3 end, const std::wstring& name);

	void EnableLog(bool tf) { mContext->enableLog(tf); }
	void EnableTimer(bool tf) { mContext->enableTimer(tf); }

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
	rcContext* mContext;
};