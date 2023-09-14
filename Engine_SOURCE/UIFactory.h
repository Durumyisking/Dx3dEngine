#pragma once
#include "Object.h"

class Panal;
class HUD;
class ImageUI;
class UIFactory
{
	SINGLE(UIFactory)

public:

	void Initialize();

	Panal* CreatePanal(GameObj* parent, Vector3 pos, Vector3 scale, const std::wstring& name, Scene* sceene);
	HUD* CreateHud(const std::wstring& name, const std::wstring& key, Vector3 pos, Vector3 scale, GameObj* parent, Scene* sceene);
	ImageUI* CreateImage(const std::wstring& name, const std::wstring& key, Vector3 pos, Vector3 scale, GameObj* parent, Scene* sceene);
	HUD* CreateHud(const std::wstring& name, const std::wstring& key, Vector3 pos, Vector3 scale, Scene* sceene);
	void CreateButton();


	template <typename T>
	T* CreateUI(const std::wstring& name, const std::wstring& key, eUIType type, Vector3 pos, Vector3 scale, GameObj* parent, Scene* scene)
	{
		T* hud = object::Instantiate<T>(eLayerType::UI, scene, key, type);
		hud->SetPos(pos);
		hud->SetScale(scale);
		hud->SetName(name);
		hud->GetComponent<Transform>()->SetParent(parent);

		return hud;
	}


private:

};