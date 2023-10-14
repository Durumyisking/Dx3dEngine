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
		AlNr,
		AlRg,
		AlNrRg,
		AlMtNrRg,
		AlEmMtNrRg,
		AlEmMsNrRg,
	};

private:
	void loadGoomba(std::wstring shaderName);
	void loadMario(std::wstring shaderName);
	void loadBuildings(std::wstring shaderName);

	void LoadCityObjectMaterial();
	void LoadCityTexture();


	void CreateMaterial(std::wstring fileName, std::wstring shaderName, std::wstring materialName, TextureState state);
	void TextureLoad(std::wstring fileName, std::wstring path, TextureState state);
private:
	bool mbLoadFinish;


	bool mbTextureLoadFinish;
	bool mbMarioLoadFinish;
	bool mbMonsterLoadFinish;
	bool mbMapLoadFinish;
	bool mbSoundLoad;

};

