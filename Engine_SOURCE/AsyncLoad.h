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
		AlNrRg,
		AlMtNrRg,
		AlEmMtNrRg,
	};

private:
	void loadGoomba(std::wstring shaderName);
	void loadMario(std::wstring shaderName);


	void CreateMaterial();
	void TextureLoad(std::wstring& fileName,std::wstring& _PathAndUntilfileName,TextureState state);
private:
	bool mbLoadFinish;


	bool mbTextureLoadFinish;
	bool mbMarioLoadFinish;
	bool mbMonsterLoadFinish;
	bool mbMapLoadFinish;
	bool mbSoundLoad;

};

