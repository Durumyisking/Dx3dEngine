#pragma once
#include "Object.h"

class Panal;
class UIFactory
{
	SINGLE(UIFactory)

public:

	void Initialize();

	Panal* CreatePanal(GameObj* parent, Vector3 pos, Vector3 scale, const std::wstring& name);
	void CreateHud();
	void CreateButton();

private:

};