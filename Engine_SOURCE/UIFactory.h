#pragma once
#include "Object.h"
#include "SpriteRenderer.h"

class UIBase;
class Panal;
class HUD;
class ImageUI;
class ColorChangeUI;
class UIFactory
{
	SINGLE(UIFactory)

public:

	void Initialize();

	Panal* CreatePanal(GameObj* parent, Vector3 pos, Vector3 scale, const std::wstring& name, Scene* sceene, eUIType type);
	HUD* CreateHud(const std::wstring& name, const std::wstring& key, Vector3 pos, Vector3 scale, GameObj* parent, Scene* sceene);
	ImageUI* CreateImage(const std::wstring& name, const std::wstring& key, Vector3 pos, Vector3 scale, GameObj* parent, Scene* sceene);
	ImageUI* CreateImage(const std::wstring& name, const std::wstring& key, Vector3 pos, Vector3 scale, Scene* sceene);
	HUD* CreateHud(const std::wstring& name, const std::wstring& key, Vector3 pos, Vector3 scale, Scene* sceene);
	void CreateButton();



	//void CreateString(const std::wstring& string, Vector3 pos, Vector3 scale, float interval, GameObj* parent, Scene* scene);
	void CreateString(const std::wstring& string, Vector3 pos, Vector3 scale, float interval, UIBase* parent, Scene* scene);
	void CreateString(const std::wstring& string, const std::wstring& matKey, Vector3 pos, Vector3 scale, Vector3 rot, float interval, UIBase* parent, Scene* scene);

	template <typename T>
	T* CreateUI(const std::wstring& name, const std::wstring& key, eUIType type, Vector3 pos, Vector3 scale, GameObj* parent, Scene* scene)
	{
		T* object = object::Instantiate<T>(eLayerType::UI, scene, key, type);
		object->SetPos(pos);
		object->SetScale(scale);
		object->SetName(name);
		object->GetComponent<Transform>()->SetParent(parent);

		return object;
	}

private:

};