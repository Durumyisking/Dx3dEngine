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

	enum class TextureState
	{
		Al,
		Em,
		Nr,
		AlNr,
		AlRg,
		AlEm,
		AlEmNr,
		AlMsNr,
		AlNrRg,
		AlMtNrRg,
		AlEmNrRg,
		AlEmMtNrRg,
		AlEmMsNrRg,
		AlEmMsMtNr,
		AlMtMsNrRg,
	};

private:
	void loadGoomba(std::wstring shaderName);
	void loadMario(std::wstring shaderName);
	//void loadBuildings(std::wstring shaderName);

	void loadCityObjectMaterial();
	void loadCityTexture();
	void loadBuildingMaterials();


	void createMaterial(std::wstring fileName, std::wstring shaderName, std::wstring materialName, TextureState state);
	void textureLoad(std::wstring fileName,std::wstring _PathAndUntilfileName,TextureState state);
private:
	bool mbLoadFinish;


	bool mbTextureLoadFinish;
	bool mbMarioLoadFinish;
	bool mbMonsterLoadFinish;
	bool mbMapLoadFinish;
	bool mbSoundLoad;

};

