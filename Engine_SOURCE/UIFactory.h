#pragma once
#include "Object.h"

class Panal;
class HUD;
class UIFactory
{
	SINGLE(UIFactory)

public:

	void Initialize();

	Panal* CreatePanal(GameObj* parent, Vector3 pos, Vector3 scale, const std::wstring& name);
	HUD* CreateHud(const std::wstring& name, const std::wstring& key, Vector3 pos, Vector3 scale, GameObj* parent);
	HUD* CreateHud(const std::wstring& name, const std::wstring& key, Vector3 pos, Vector3 scale);
	void CreateButton();

private:

};