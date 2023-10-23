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

	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/Brick", L"BlockBrick", true);

	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/SmokeParticle", L"CloudParticle");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/Coin", L"Coin");



	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/HomeStageGroundCollider", L"HomeStageGroundCollider");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/NaviCollider", L"NaviCollider");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/TaxiCar/Car", L"Car");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/Motorcycle", L"Motorcycle");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/MotorcycleParkingLot", L"MotorcycleParkingLot");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/SouvenirCity1", L"SouvenirCity1");


	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBench000", L"CityWorldHomeBench000");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CheckpointFlag", L"CheckpointFlag");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CItyWorldFlag", L"CityWorldFlag");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldBush", L"CityWorldBush");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldBushA", L"CityWorldBushA");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldChairA", L"CityWorldChairA");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldChairB", L"CityWorldChairB");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityGround", L"CityWorldHomeGround000");


#pragma region Building
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding000", L"CityWorldHomeBuilding000");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding001", L"CityWorldHomeBuilding001");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding002", L"CityWorldHomeBuilding002");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding003", L"CityWorldHomeBuilding003");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding004", L"CityWorldHomeBuilding004");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding005", L"CityWorldHomeBuilding005");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding007", L"CityWorldHomeBuilding007");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding008", L"CityWorldHomeBuilding008");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding009", L"CityWorldHomeBuilding009");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding010", L"CityWorldHomeBuilding010");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding011", L"CityWorldHomeBuilding011");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding012", L"CityWorldHomeBuilding012");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding013", L"CityWorldHomeBuilding013");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding015", L"CityWorldHomeBuilding015");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding016", L"CityWorldHomeBuilding016");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding017", L"CityWorldHomeBuilding017");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding018", L"CityWorldHomeBuilding018");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding019", L"CityWorldHomeBuilding019");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding020", L"CityWorldHomeBuilding020");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding021", L"CityWorldHomeBuilding021");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding022", L"CityWorldHomeBuilding022");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding023", L"CityWorldHomeBuilding023");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding024", L"CityWorldHomeBuilding024");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding026", L"CityWorldHomeBuilding026");
	// 
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding014", L"CityWorldHomeBuilding014");  // --- 14¹ø Building Ãâ·Â¾ÈµÊ

#pragma endregion

#pragma region Fence
//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeFence/CityWorldHomeFence000", L"CityWorldHomeFence000");
//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeFence/CityWorldHomeFence001", L"CityWorldHomeFence001");
//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeFence/CityWorldHomeFence002", L"CityWorldHomeFence002");
//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeFence/CityWorldHomeFence003", L"CityWorldHomeFence003");
//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeFence/CityWorldHomeFence005", L"CityWorldHomeFence005");
//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeFence/CityWorldHomeFence006", L"CityWorldHomeFence006");

#pragma endregion

#pragma region Manhole
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeManhole/CityWorldHomeManhole000", L"CityWorldHomeManhole000");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeManhole/CityWorldHomeManhole001", L"CityWorldHomeManhole001");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeManhole/CityWorldHomeManhole002", L"CityWorldHomeManhole002");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeManhole/CityWorldHomeManhole003", L"CityWorldHomeManhole003");

#pragma endregion

#pragma region HomeSign
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSign/CityWorldHomeSign000", L"CityWorldHomeSign000");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSign/CityWorldHomeSign001", L"CityWorldHomeSign001");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSign/CityWorldHomeSign002", L"CityWorldHomeSign002");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSign/CityWorldHomeSign003", L"CityWorldHomeSign003");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSign/CityWorldHomeSign004", L"CityWorldHomeSign004");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSign/CityWorldHomeSign006", L"CityWorldHomeSign006");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSign/CityWorldHomeSign007", L"CityWorldHomeSign007");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSign/CityWorldHomeSign008", L"CityWorldHomeSign008");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSign/CityWorldHomeSign009", L"CityWorldHomeSign009");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSign/CityWorldHomeSign010", L"CityWorldHomeSign010");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSign/CityWorldHomeSign012", L"CityWorldHomeSign012");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSign/CityWorldHomeSign013", L"CityWorldHomeSign013");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSign/CityWorldHomeSign016", L"CityWorldHomeSign016");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSign/CityWorldHomeSign017", L"CityWorldHomeSign017");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSign/CityWorldHomeSign020", L"CityWorldHomeSign020");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSign/CityWorldHomeSign022", L"CityWorldHomeSign022");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSign/CityWorldHomeSign027", L"CityWorldHomeSign027");


#pragma endregion


	*bfinish = true;
}


void ResourceMgr::LoadModel_CityWorld(bool* bfinish)
{
	//GETSINGLE(FileMgr)->StageFolderLoad(L"..//Resources/CityWorld/HomeStage", L"CityWorld");


	*bfinish = true;
}
