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

	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/Brick", L"BlockBrick");

	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/SmokeParticle", L"CloudParticle");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/Coin", L"Coin");

	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/HomeStageGroundCollider", L"HomeStageGroundCollider");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/NaviCollider", L"NaviCollider");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/TaxiCar/Car", L"Car");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldBush", L"CityWorldBush");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldBushA", L"CityWorldBushA");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldChairA", L"CityWorldChairA");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldChairB", L"CityWorldChairB");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBench000", L"CityWorldHomeBench000");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CheckpointFlag", L"CheckpointFlag");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CItyWorldFlag", L"CityWorldFlag");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding001", L"CityWorldHomeBuilding000");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding001", L"CityWorldHomeBuilding001");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding001", L"CityWorldHomeBuilding002");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding002", L"CityWorldHomeBuilding003");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding004", L"CityWorldHomeBuilding004");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding005", L"CityWorldHomeBuilding005");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding007", L"CityWorldHomeBuilding007");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding008", L"CityWorldHomeBuilding008");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding009", L"CityWorldHomeBuilding009");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding010", L"CityWorldHomeBuilding010");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding011", L"CityWorldHomeBuilding011");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding011", L"CityWorldHomeBuilding012");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding011", L"CityWorldHomeBuilding013");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding011", L"CityWorldHomeBuilding014");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding011", L"CityWorldHomeBuilding015");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding011", L"CityWorldHomeBuilding016");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding011", L"CityWorldHomeBuilding017");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding011", L"CityWorldHomeBuilding018");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding011", L"CityWorldHomeBuilding019");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding011", L"CityWorldHomeBuilding020");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding011", L"CityWorldHomeBuilding021");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding011", L"CityWorldHomeBuilding022");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding011", L"CityWorldHomeBuilding023");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding011", L"CityWorldHomeBuilding024");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding011", L"CityWorldHomeBuilding026");
	*bfinish = true;
}


void ResourceMgr::LoadModel_CityWorld(bool* bfinish)
{
	//GETSINGLE(FileMgr)->StageFolderLoad(L"..//Resources/CityWorld/HomeStage", L"CityWorld");
	
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityGround", L"CityGround");


	*bfinish = true;
}
