#pragma once
#include "Engine.h"

class AsyncLoad
{
	SINGLE(AsyncLoad)


public:
	void Update();

	void LoadModels();
	void LoadTextures();
	void LoadMaterials();
	void LoadSounds();

	bool IsLoadFinish() const { return mbLoadFinish; }


private:
	void loadGoomba(std::wstring shaderName);
	void loadMario(std::wstring shaderName);
	void loadBuildings(std::wstring shaderName);

private:
	bool mbLoadFinish;


	bool mbTextureLoadFinish;
	bool mbMarioLoadFinish;
	bool mbMonsterLoadFinish;
	bool mbMapLoadFinish;
	bool mbSoundLoad;

};

