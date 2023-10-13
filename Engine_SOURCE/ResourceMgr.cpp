#include "ResourceMgr.h"
#include "Material.h"
#include "FileMgr.h"
#include "AudioClip.h"

ResourceMgr::ResourceMgr()
	: mResources{}
{

}

ResourceMgr::~ResourceMgr()
{

}

Material* ResourceMgr::CreateMaterial(std::wstring texture, eTextureSlot slot, std::wstring shaderName, std::wstring materialName)
{
	Material* mat = new Material(texture, slot, shaderName);
	Insert<Material>(materialName, mat);
	return mat;
}

Material* ResourceMgr::CreateMaterial(std::wstring shaderName, std::wstring materialName)
{
	Material* mat = new Material(shaderName);
	Insert<Material>(materialName, mat);
	return mat;
}

Material* ResourceMgr::CreateMaterial(std::wstring textureColor, std::wstring shaderName, std::wstring materialName)
{
	Material* mat = new Material(textureColor, shaderName);
	Insert<Material>(materialName, mat);
	return mat;
}

Material* ResourceMgr::CreateMaterial(std::wstring textureColor, std::wstring textureNormal, std::wstring shaderName, std::wstring materialName)
{
	Material* mat = new Material(textureColor, textureNormal, shaderName);
	Insert<Material>(materialName, mat);
	return mat;
}

Material* ResourceMgr::CreateMaterial(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring shaderName, std::wstring materialName)
{
	Material* mat = new Material(textureColor, textureNormal, textureMetal, shaderName);
	Insert<Material>(materialName, mat);
	return mat;
}

Material* ResourceMgr::CreateMaterial(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring textureRoughness, std::wstring shaderName, std::wstring materialName)
{
	Material* mat = new Material(textureColor, textureNormal, textureMetal, textureRoughness, shaderName);
	Insert<Material>(materialName, mat);
	return mat;
}

Material* ResourceMgr::CreateMaterial(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring textureRoughness, std::wstring textureEmissive, std::wstring shaderName, std::wstring materialName)
{
	Material* mat = new Material(textureColor, textureNormal, textureMetal, textureRoughness, textureEmissive, shaderName);
	Insert<Material>(materialName, mat);
	return mat;
}

void ResourceMgr::Initalize()
{
	//Load<AudioClip>(L"TitleBGM", L"Resources\\Sound\\Title\\TitleBGM.wav");
}

void ResourceMgr::Release()
{
	for (auto& iter : mResources)
	{
		if (iter.second == nullptr)
			continue;

		delete iter.second;
		iter.second = nullptr;
	}
}

void ResourceMgr::LoadModel_Mario(bool* bfinish)
{
	// Mario
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/MarioBody", L"Mario");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/MarioHandL", L"MarioHandL");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/MarioHandR", L"MarioHandR");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/MarioFace", L"MarioFace");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/MarioHead", L"MarioHead");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/MarioEye", L"MarioEye");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/MarioCap", L"MarioCap");

	*bfinish = true;
}

void ResourceMgr::LoadModel_Monster(bool* bfinish)
{
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/goomba", L"goomba");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/Packun", L"Packun");

	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/SmokeParticle", L"CloudParticle");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldHomeBuilding002", L"CityWorldHomeBuilding002");
	*bfinish = true;
}


void ResourceMgr::LoadModel_CityWorld(bool* bfinish)
{
	//GETSINGLE(FileMgr)->StageFolderLoad(L"..//Resources/CityWorld/HomeStage", L"CityWorld");

	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldHomeBuilding002", L"CityWorldHomeBuilding002");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityGround", L"CityGround");

	*bfinish = true;
}
