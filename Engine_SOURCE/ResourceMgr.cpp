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
	Model* model = nullptr;

	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/goomba", L"goomba");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/Packun", L"Packun");

	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/Brick", L"BlockBrick", true);

	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/SmokeParticle", L"CloudParticle");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/Coin", L"Coin");

	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CapWorld/Tower", L"CapWorldHomeTower000");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CapWorld/Collider", L"CapTowerCollider");

	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/HomeStageGroundCollider", L"HomeStageGroundCollider");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/NaviCollider", L"NaviCollider");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/TaxiCar/Car", L"Car");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/Motorcycle", L"Motorcycle");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/MotorcycleParkingLot", L"MotorcycleParkingLot");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/SouvenirCity1", L"SouvenirCity1");


	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBench000", L"CityWorldHomeBench000");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CheckpointFlag", L"CheckpointFlag");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CItyWorldFlag", L"CityWorldFlag");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldBush", L"CityWorldBush");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldBushA", L"CityWorldBushA");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldChairA", L"CityWorldChairA");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldChairB", L"CityWorldChairB");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityGround", L"CityWorldHomeGround000");

	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldFence/CityWorldFenceA", L"CityWorldFenceA");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldFence/CityWorldFenceB", L"CityWorldFenceB");


#pragma region Building
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding000", L"CityWorldHomeBuilding000");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding001", L"CityWorldHomeBuilding001");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding002", L"CityWorldHomeBuilding002");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding003", L"CityWorldHomeBuilding003");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding004", L"CityWorldHomeBuilding004");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding005", L"CityWorldHomeBuilding005");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding007", L"CityWorldHomeBuilding007");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding008", L"CityWorldHomeBuilding008");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding009", L"CityWorldHomeBuilding009");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding010", L"CityWorldHomeBuilding010");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding011", L"CityWorldHomeBuilding011");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding012", L"CityWorldHomeBuilding012");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding013", L"CityWorldHomeBuilding013");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding015", L"CityWorldHomeBuilding015");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding016", L"CityWorldHomeBuilding016");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding017", L"CityWorldHomeBuilding017");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding018", L"CityWorldHomeBuilding018");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding019", L"CityWorldHomeBuilding019");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding020", L"CityWorldHomeBuilding020");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding021", L"CityWorldHomeBuilding021");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding022", L"CityWorldHomeBuilding022");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding023", L"CityWorldHomeBuilding023");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding024", L"CityWorldHomeBuilding024");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding026", L"CityWorldHomeBuilding026");
	 
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeBuilding/CityWorldHomeBuilding014", L"CityWorldHomeBuilding014");  // --- 14번 Building 출력안됨

#pragma endregion

#pragma region Fence
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeFence/CityWorldHomeFence000", L"CityWorldHomeFence000");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeFence/CityWorldHomeFence001", L"CityWorldHomeFence001");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeFence/CityWorldHomeFence002", L"CityWorldHomeFence002");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeFence/CityWorldHomeFence003", L"CityWorldHomeFence003");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeFence/CityWorldHomeFence005", L"CityWorldHomeFence005");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeFence/CityWorldHomeFence006", L"CityWorldHomeFence006");

#pragma endregion

#pragma region Manhole
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeManhole/CityWorldHomeManhole000", L"CityWorldHomeManhole000");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeManhole/CityWorldHomeManhole001", L"CityWorldHomeManhole001");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeManhole/CityWorldHomeManhole002", L"CityWorldHomeManhole002");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeManhole/CityWorldHomeManhole003", L"CityWorldHomeManhole003");

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

#pragma region Door
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeDoor/CityWorldHomeDoor000", L"CityWorldHomeDoor000");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeDoor/CityWorldHomeDoor001", L"CityWorldHomeDoor001");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeDoor/CityWorldHomeDoor002", L"CityWorldHomeDoor002");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeDoor/CityWorldHomeDoor003", L"CityWorldHomeDoor003");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeDoor/CityWorldHomeDoor004", L"CityWorldHomeDoor004");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeDoor/CityWorldHomeDoor005", L"CityWorldHomeDoor005");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeDoor/CityWorldHomeDoor006", L"CityWorldHomeDoor006");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeDoor/CityWorldHomeDoor007", L"CityWorldHomeDoor007");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeDoor/CityWorldHomeDoor008", L"CityWorldHomeDoor008");

#pragma endregion

#pragma region Signal
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSignal/CityWorldHomeSignal000", L"CityWorldHomeSignal000");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSignal/CityWorldHomeSignal001", L"CityWorldHomeSignal001");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSignal/CityWorldHomeSignal002", L"CityWorldHomeSignal002");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSignal/CityWorldHomeSignal003", L"CityWorldHomeSignal003");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSignal/CityWorldHomeSignal004", L"CityWorldHomeSignal004");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeSignal/CityWorldHomeSignal006", L"CityWorldHomeSignal006");

#pragma endregion

#pragma region Streetlight
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeStreetlight/CityWorldHomeStreetlight000", L"CityWorldHomeStreetlight000");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeStreetlight/CityWorldHomeStreetlight001", L"CityWorldHomeStreetlight001");

#pragma endregion

#pragma region Tree
		GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorldObject/CityWorldHomeTree/CityWorldHomeTree000", L"CityWorldHomeTree000");

#pragma endregion

#pragma region BrickBreak
		GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/BrickBreak", L"BlockBrickBreak");

#pragma endregion


	*bfinish = true;
}


void ResourceMgr::LoadModel_CityWorld(bool* bfinish)
{
	//GETSINGLE(FileMgr)->StageFolderLoad(L"..//Resources/CityWorld/HomeStage", L"CityWorld");


	*bfinish = true;
}

void ResourceMgr::SettingModelMaterial()
{
	Model* model = nullptr;
#pragma region Building
	model = Find<Model>(L"CityWorldHomeBuilding000");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding000_0Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding000_1Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding000_2Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding000_3Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding000_4Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding000_5Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding000_6Material");
	model->SetVariableMaterialsByKey(7, L"HomeBuilding000_7Material");
	model->SetVariableMaterialsByKey(8, L"HomeBuilding000_8Material");
	model->SetVariableMaterialsByKey(9, L"HomeBuilding000_9Material");
	model->SetVariableMaterialsByKey(10, L"HomeBuilding000_10Material");
	model->SetVariableMaterialsByKey(11, L"HomeBuilding000_11Material");
	model->SetVariableMaterialsByKey(12, L"HomeBuilding000_12Material");
	model->SetVariableMaterialsByKey(13, L"HomeBuilding000_13Material");
	model->SetVariableMaterialsByKey(14, L"HomeBuilding000_14Material");
	model->SetVariableMaterialsByKey(15, L"HomeBuilding000_15Material");
	model->SetVariableMaterialsByKey(16, L"HomeBuilding000_16Material");
	model->SetVariableMaterialsByKey(17, L"HomeBuilding000_17Material");
	model->SetVariableMaterialsByKey(18, L"HomeBuilding000_13Material");
	model->SetVariableMaterialsByKey(19, L"HomeBuilding000_18Material");
	model->SetVariableMaterialsByKey(20, L"HomeBuilding000_19Material");
	model->SetVariableMaterialsByKey(21, L"HomeBuilding000_20Material");

	model = Find<Model>(L"CityWorldHomeBuilding002");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding002_0Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding002_1Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding002_0Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding002_3Material");
	model = Find<Model>(L"CityWorldHomeBuilding004");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding004_0Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding004_1Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding004_2Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding004_1Material"); //수정 필요 BuildingEntrance02
	model->SetVariableMaterialsByKey(4, L"HomeBuilding004_3Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding004_4Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding004_5Material");
	model->SetVariableMaterialsByKey(7, L"HomeBuilding004_6Material");
	model->SetVariableMaterialsByKey(8, L"HomeBuilding004_7Material");
	model->SetVariableMaterialsByKey(9, L"HomeBuilding004_8Material");
	model->SetVariableMaterialsByKey(10, L"HomeBuilding004_9Material");
	model->SetVariableMaterialsByKey(11 , L"HomeBuilding004_10Material");
	model->SetVariableMaterialsByKey(12 , L"HomeBuilding004_11Material");
	model->SetVariableMaterialsByKey(13 , L"HomeBuilding004_13Material");
	model->SetVariableMaterialsByKey(14 , L"HomeBuilding004_12Material");
	model = Find<Model>(L"CityWorldHomeBuilding005");
	model->SetVariableMaterialsByKey(0,L"HomeBuilding005_0Material");
	model->SetVariableMaterialsByKey(1,L"HomeBuilding005_1Material");
	model->SetVariableMaterialsByKey(2,L"HomeBuilding005_2Material");
	model->SetVariableMaterialsByKey(3,L"HomeBuilding005_1Material"); //수정 필요 BuildingEntrance02
	model->SetVariableMaterialsByKey(4,L"HomeBuilding005_3Material");
	model->SetVariableMaterialsByKey(5,L"HomeBuilding005_4Material");
	model->SetVariableMaterialsByKey(6,L"HomeBuilding005_5Material");
	model->SetVariableMaterialsByKey(7,L"HomeBuilding005_6Material");
	model->SetVariableMaterialsByKey(8,L"HomeBuilding005_7Material");
	model->SetVariableMaterialsByKey(9,L"HomeBuilding005_8Material");
	model->SetVariableMaterialsByKey(10,L"HomeBuilding005_9Material");
	model->SetVariableMaterialsByKey(11,L"HomeBuilding005_10Material");
	model->SetVariableMaterialsByKey(12,L"HomeBuilding005_11Material");
	model->SetVariableMaterialsByKey(13,L"HomeBuilding005_13Material");
	model->SetVariableMaterialsByKey(14,L"HomeBuilding005_12Material");
	model = Find<Model>(L"CityWorldHomeBuilding007");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding007_0Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding007_1Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding007_0Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding007_1Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding007_2Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding007_3Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding007_4Material");
	model = Find<Model>(L"CityWorldHomeBuilding008");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding008_0Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding008_1Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding008_2Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding008_3Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding008_2Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding008_3Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding008_4Material");
	model->SetVariableMaterialsByKey(7, L"HomeBuilding008_5Material");
	model->SetVariableMaterialsByKey(8, L"HomeBuilding008_6Material");
	model = Find<Model>(L"CityWorldHomeBuilding009");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding009_0Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding009_1Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding009_2Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding009_1Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding009_2Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding009_3Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding009_4Material");
	model->SetVariableMaterialsByKey(7, L"HomeBuilding009_5Material");
	model->SetVariableMaterialsByKey(8, L"HomeBuilding009_6Material");
	model = Find<Model>(L"CityWorldHomeBuilding010");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding0010_0Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding0010_1Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding0010_3Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding0010_3Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding0010_4Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding0010_5Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding0010_6Material");
	model->SetVariableMaterialsByKey(7, L"HomeBuilding0010_7Material");
	model->SetVariableMaterialsByKey(8, L"HomeBuilding0010_8Material");
	model->SetVariableMaterialsByKey(9, L"HomeBuilding0010_9Material");
	model->SetVariableMaterialsByKey(10, L"HomeBuilding0010_9Material");
	model->SetVariableMaterialsByKey(11, L"HomeBuilding0010_10Material");
	model = Find<Model>(L"CityWorldHomeBuilding011");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding0011_0Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding0011_1Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding0011_3Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding0011_3Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding0011_4Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding0011_5Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding0011_6Material");
	model->SetVariableMaterialsByKey(7, L"HomeBuilding0011_7Material");
	model->SetVariableMaterialsByKey(8, L"HomeBuilding0011_8Material");
	model->SetVariableMaterialsByKey(9, L"HomeBuilding0011_9Material");
	model->SetVariableMaterialsByKey(10, L"HomeBuilding0011_9Material");
	model->SetVariableMaterialsByKey(11,L"HomeBuilding0011_10Material");
	model = Find<Model>(L"CityWorldHomeBuilding012");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding0012_0Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding0012_1Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding0012_3Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding0012_3Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding0012_4Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding0012_5Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding0012_6Material");
	model->SetVariableMaterialsByKey(7, L"HomeBuilding0012_7Material");
	model->SetVariableMaterialsByKey(8, L"HomeBuilding0012_8Material");
	model->SetVariableMaterialsByKey(9, L"HomeBuilding0012_9Material");
	model->SetVariableMaterialsByKey(10, L"HomeBuilding0012_9Material");
	model->SetVariableMaterialsByKey(11, L"HomeBuilding0012_10Material");
	model = Find<Model>(L"CityWorldHomeBuilding013");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding0013_1Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding0013_1Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding0013_2Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding0013_3Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding0013_4Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding0013_5Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding0013_6Material");
	model->SetVariableMaterialsByKey(7, L"HomeBuilding0013_7Material");
	model->SetVariableMaterialsByKey(8, L"HomeBuilding0013_7Material");
	model->SetVariableMaterialsByKey(9, L"HomeBuilding0013_8Material");
	model = Find<Model>(L"CityWorldHomeBuilding015");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding0015_2Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding0015_1Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding0015_2Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding0015_3Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding0015_4Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding0015_5Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding0015_6Material");
	model->SetVariableMaterialsByKey(7, L"HomeBuilding0015_7Material");
	model->SetVariableMaterialsByKey(8, L"HomeBuilding0015_8Material");
	model->SetVariableMaterialsByKey(9, L"HomeBuilding0015_9Material");
	model = Find<Model>(L"CityWorldHomeBuilding016");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding0016_7Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding0016_1Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding0016_2Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding0016_3Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding0016_4Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding0016_5Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding0016_6Material");
	model->SetVariableMaterialsByKey(7, L"HomeBuilding0016_7Material");
	model = Find<Model>(L"CityWorldHomeBuilding017");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding0017_7Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding0017_1Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding0017_2Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding0017_3Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding0017_4Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding0017_5Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding0017_6Material");
	model->SetVariableMaterialsByKey(7, L"HomeBuilding0017_7Material");

	model = Find<Model>(L"CityWorldHomeBuilding018");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding0018_0Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding0018_0Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding0018_4Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding0018_10Material"); //
	model->SetVariableMaterialsByKey(4, L"HomeBuilding0018_4Material"); 
	model->SetVariableMaterialsByKey(5, L"HomeBuilding0018_5Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding0018_6Material");
	model->SetVariableMaterialsByKey(7, L"HomeBuilding0018_7Material"); //
	model->SetVariableMaterialsByKey(8, L"HomeBuilding0018_8Material");//
	model->SetVariableMaterialsByKey(9, L"HomeBuilding0018_9Material");//
	model->SetVariableMaterialsByKey(10, L"HomeBuilding0018_10Material"); //

	model = Find<Model>(L"CityWorldHomeBuilding019");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding0019_6Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding0019_1Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding0019_2Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding0019_3Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding0019_4Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding0019_5Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding0019_6Material");
	model = Find<Model>(L"CityWorldHomeBuilding020");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding0020_7Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding0020_9Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding0020_2Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding0020_3Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding0020_4Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding0020_5Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding0020_6Material");
	model->SetVariableMaterialsByKey(7, L"HomeBuilding0020_7Material");
	model->SetVariableMaterialsByKey(8, L"HomeBuilding0020_8Material");
	model = Find<Model>(L"CityWorldHomeBuilding021");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding0021_7Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding0021_9Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding0021_2Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding0021_3Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding0021_4Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding0021_5Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding0021_6Material");
	model->SetVariableMaterialsByKey(7, L"HomeBuilding0021_7Material");
	model->SetVariableMaterialsByKey(8, L"HomeBuilding0021_8Material");
	model = Find<Model>(L"CityWorldHomeBuilding022");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding0022_7Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding0022_9Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding0022_2Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding0022_3Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding0022_4Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding0022_5Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding0022_6Material");
	model->SetVariableMaterialsByKey(7, L"HomeBuilding0022_7Material");
	model->SetVariableMaterialsByKey(8, L"HomeBuilding0022_8Material");
	model = Find<Model>(L"CityWorldHomeBuilding023");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding0023_5Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding0023_1Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding0023_2Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding0023_3Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding0023_4Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding0023_5Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding0023_6Material");
	model = Find<Model>(L"CityWorldHomeBuilding024");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding0024_9Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding0024_1Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding0024_2Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding0024_3Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding0024_4Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding0024_5Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding0024_6Material");
	model->SetVariableMaterialsByKey(7, L"HomeBuilding0024_7Material");
	model->SetVariableMaterialsByKey(8, L"HomeBuilding0024_8Material");
	model->SetVariableMaterialsByKey(9, L"HomeBuilding0024_9Material");
	model->SetVariableMaterialsByKey(10, L"HomeBuilding0024_10Material");
	model->SetVariableMaterialsByKey(11, L"HomeBuilding0024_11Material");
	model = Find<Model>(L"CityWorldHomeBuilding026");
	model->SetVariableMaterialsByKey(0, L"HomeBuilding0026_0Material");
	model->SetVariableMaterialsByKey(1, L"HomeBuilding0026_2Material");
	model->SetVariableMaterialsByKey(2, L"HomeBuilding0026_2Material");
	model->SetVariableMaterialsByKey(3, L"HomeBuilding0026_3Material");
	model->SetVariableMaterialsByKey(4, L"HomeBuilding0026_4Material");
	model->SetVariableMaterialsByKey(5, L"HomeBuilding0026_5Material");
	model->SetVariableMaterialsByKey(6, L"HomeBuilding0026_6Material");
	model->SetVariableMaterialsByKey(7, L"HomeBuilding0026_7Material");
	model->SetVariableMaterialsByKey(8, L"HomeBuilding0026_8Material");
	model->SetVariableMaterialsByKey(9, L"HomeBuilding0026_9Material");
	model->SetVariableMaterialsByKey(10, L"HomeBuilding0026_10Material");
	model->SetVariableMaterialsByKey(11, L"HomeBuilding0026_11Material");
#pragma endregion

	model = Find<Model>(L"CapWorldHomeTower000");
	model->SetVariableMaterialsByKey(0, L"PhongMaterial");
	model->SetVariableMaterialsByKey(1, L"DazzleCamouflage03");
	model->SetVariableMaterialsByKey(2, L"DazzleCamouflageSingleLayer");
	model->SetVariableMaterialsByKey(3, L"DazzleCamouflageSingleLayer");
	model->SetVariableMaterialsByKey(4, L"FeltHair01_Material");
	model->SetVariableMaterialsByKey(5, L"FeltHair01_Material");
	model->SetVariableMaterialsByKey(6, L"FeltPlain02_Material");
	model->SetVariableMaterialsByKey(7, L"FeltPlain02_Material");
	model->SetVariableMaterialsByKey(8, L"FeltPlainHair01_Material");
	model->SetVariableMaterialsByKey(9, L"FeltPlainHair01_Material");
	model->SetVariableMaterialsByKey(10, L"FurPlane03_Material");
	model->SetVariableMaterialsByKey(11, L"FurPlane05_Material");
	model->SetVariableMaterialsByKey(12, L"FurPlane06_Material");
	model->SetVariableMaterialsByKey(13, L"PhongMaterial");
	model->SetVariableMaterialsByKey(14, L"PhongMaterial");
	model->SetVariableMaterialsByKey(15, L"WallBrick00_Material");
	model->SetVariableMaterialsByKey(16, L"WallPlaster00_Material");

#pragma region Fence
	model = Find<Model>(L"CityWorldFenceA");
	model->SetVariableMaterialsByKey(0, L"FenceA_0Material");

	model = Find<Model>(L"CityWorldFenceB");
	model->SetVariableMaterialsByKey(0, L"FenceB_0Material");

	model = Find<Model>(L"CityWorldHomeFence000");
	model->SetVariableMaterialsByKey(0, L"Fence000_0Material");
	model->SetVariableMaterialsByKey(1, L"Fence000_1Material");
	model->SetVariableMaterialsByKey(2, L"Fence000_2Material");

	model = Find<Model>(L"CityWorldHomeFence001");
	model->SetVariableMaterialsByKey(0, L"Fence001_0Material");
	model->SetVariableMaterialsByKey(1, L"Fence001_0Material");

	model = Find<Model>(L"CityWorldHomeFence002");
	model->SetVariableMaterialsByKey(0, L"Fence002_0Material");
	model->SetVariableMaterialsByKey(1, L"Fence002_0Material");
	model->SetVariableMaterialsByKey(2, L"Fence002_1Material");

	model = Find<Model>(L"CityWorldHomeFence003");
	model->SetVariableMaterialsByKey(0, L"Fence003_0Material");

	model = Find<Model>(L"CityWorldHomeFence005");
	model->SetVariableMaterialsByKey(0, L"Fence005_0Material");

	model = Find<Model>(L"CityWorldHomeFence006");
	model->SetVariableMaterialsByKey(0, L"Fence006_0Material");
#pragma endregion

#pragma region Bench
	model = Find<Model>(L"CityWorldHomeBench000");
	model->SetVariableMaterialsByKey(0, L"BenchiWood00Material");
#pragma endregion

#pragma region Bush
	model = Find<Model>(L"CityWorldBush");
	model->SetVariableMaterialsByKey(0, L"CityWorldBush_0Material");
	model->SetVariableMaterialsByKey(1, L"CityWorldBush_1Material");

	model = Find<Model>(L"CityWorldBushA");
	model->SetVariableMaterialsByKey(0, L"CityWorldBushA_0Material");
	model->SetVariableMaterialsByKey(1, L"CityWorldBushA_1Material");
	model->SetVariableMaterialsByKey(2, L"CityWorldBushA_0Material");
#pragma endregion

#pragma region Manhole
	model = Find<Model>(L"CityWorldHomeManhole000");
	model->SetVariableMaterialsByKey(0, L"Manhole000_0Material");

	model = Find<Model>(L"CityWorldHomeManhole001");
	model->SetVariableMaterialsByKey(0, L"Manhole001_0Material");

	model = Find<Model>(L"CityWorldHomeManhole002");
	model->SetVariableMaterialsByKey(0, L"Manhole002_0Material");

	model = Find<Model>(L"CityWorldHomeManhole003");
	model->SetVariableMaterialsByKey(0, L"Manhole003_0Material");
#pragma endregion

#pragma region Door
	model = Find<Model>(L"CityWorldHomeDoor000");
	model->SetVariableMaterialsByKey(0, L"Door000_0Material");
	model->SetVariableMaterialsByKey(1, L"Door000_0Material");
	model->SetVariableMaterialsByKey(2, L"Door000_1Material");

	model = Find<Model>(L"CityWorldHomeDoor001");
	model->SetVariableMaterialsByKey(0, L"Door001_0Material");
	model->SetVariableMaterialsByKey(1, L"Door001_0Material");
	model->SetVariableMaterialsByKey(2, L"Door001_0Material");
	model->SetVariableMaterialsByKey(3, L"Door001_0Material");

	model = Find<Model>(L"CityWorldHomeDoor002");
	model->SetVariableMaterialsByKey(0, L"Door002_0Material");
	model->SetVariableMaterialsByKey(1, L"Door002_0Material");
	model->SetVariableMaterialsByKey(2, L"Door002_0Material");
	model->SetVariableMaterialsByKey(3, L"Door002_0Material");

	model = Find<Model>(L"CityWorldHomeDoor003");
	model->SetVariableMaterialsByKey(0, L"Door003_1Material");
	model->SetVariableMaterialsByKey(1, L"Door003_0Material");
	model->SetVariableMaterialsByKey(2, L"Door003_0Material");
	model->SetVariableMaterialsByKey(3, L"Door003_0Material");
	model->SetVariableMaterialsByKey(4, L"Door003_0Material");

	model = Find<Model>(L"CityWorldHomeDoor004");
	model->SetVariableMaterialsByKey(0, L"Door004_0Material");
	model->SetVariableMaterialsByKey(1, L"Door004_1Material");
	model->SetVariableMaterialsByKey(2, L"Door004_1Material");
	model->SetVariableMaterialsByKey(3, L"Door004_1Material");
	model->SetVariableMaterialsByKey(4, L"Door004_1Material");

	model = Find<Model>(L"CityWorldHomeDoor005");
	model->SetVariableMaterialsByKey(0, L"Door005_0Material");
	model->SetVariableMaterialsByKey(1, L"Door005_0Material");
	model->SetVariableMaterialsByKey(2, L"Door005_0Material");
	model->SetVariableMaterialsByKey(3, L"Door005_0Material");

	model = Find<Model>(L"CityWorldHomeDoor006");
	model->SetVariableMaterialsByKey(0, L"Door006_0Material");
	model->SetVariableMaterialsByKey(1, L"Door006_1Material");
	model->SetVariableMaterialsByKey(2, L"Door006_1Material");
	model->SetVariableMaterialsByKey(3, L"Door006_2Material");
	model->SetVariableMaterialsByKey(4, L"Door006_2Material");

	model = Find<Model>(L"CityWorldHomeDoor007");
	model->SetVariableMaterialsByKey(0, L"Door007_0Material");
	model->SetVariableMaterialsByKey(1, L"Door007_0Material");
	model->SetVariableMaterialsByKey(2, L"Door007_0Material");
	model->SetVariableMaterialsByKey(3, L"Door007_0Material");
	model->SetVariableMaterialsByKey(4, L"Door007_1Material");
	model->SetVariableMaterialsByKey(5, L"Door007_1Material");
	model->SetVariableMaterialsByKey(6, L"Door007_1Material");
	model->SetVariableMaterialsByKey(7, L"Door007_1Material");

	model = Find<Model>(L"CityWorldHomeDoor008");
	model->SetVariableMaterialsByKey(0, L"Door008_0Material");
	model->SetVariableMaterialsByKey(1, L"Door008_0Material");
	model->SetVariableMaterialsByKey(2, L"Door008_1Material");
	model->SetVariableMaterialsByKey(3, L"Door008_1Material");
	model->SetVariableMaterialsByKey(4, L"Door008_1Material");
	model->SetVariableMaterialsByKey(5, L"Door008_1Material");
	model->SetVariableMaterialsByKey(6, L"Door008_2Material");
	model->SetVariableMaterialsByKey(7, L"Door008_2Material");
	model->SetVariableMaterialsByKey(8, L"Door008_2Material");
	model->SetVariableMaterialsByKey(9, L"Door008_2Material");

#pragma endregion

#pragma region Signal
	model = Find<Model>(L"CityWorldHomeSignal000");
	model->SetVariableMaterialsByKey(0, L"Signal000_0Material");
	model->SetVariableMaterialsByKey(1, L"Signal000_1Material");
	model->SetVariableMaterialsByKey(2, L"Signal000_2Material");

	model = Find<Model>(L"CityWorldHomeSignal001");
	model->SetVariableMaterialsByKey(0, L"Signal001_0Material");

	model = Find<Model>(L"CityWorldHomeSignal002");
	model->SetVariableMaterialsByKey(0, L"Signal002_0Material");

	model = Find<Model>(L"CityWorldHomeSignal003");
	model->SetVariableMaterialsByKey(0, L"Signal003_0Material");
	model->SetVariableMaterialsByKey(1, L"Signal003_1Material");
	model->SetVariableMaterialsByKey(2, L"Signal003_2Material");
	model->SetVariableMaterialsByKey(3, L"Signal003_3Material");

	model = Find<Model>(L"CityWorldHomeSignal004");
	model->SetVariableMaterialsByKey(0, L"Signal004_0Material");
	model->SetVariableMaterialsByKey(1, L"Signal004_1Material");

	model = Find<Model>(L"CityWorldHomeSignal006");
	model->SetVariableMaterialsByKey(0, L"Signal006_0Material");

#pragma endregion

#pragma region Streetlight
	model = Find<Model>(L"CityWorldHomeStreetlight000");
	model->SetVariableMaterialsByKey(0, L"Streetlight000_0Material");
	model->SetVariableMaterialsByKey(1, L"Streetlight000_1Material");

#pragma endregion

#pragma region Tree
	model = Find<Model>(L"CityWorldHomeTree000");
	model->SetVariableMaterialsByKey(0, L"Tree000_0Material");
	model->SetVariableMaterialsByKey(1, L"Tree000_1Material");
	model->SetVariableMaterialsByKey(2, L"Tree000_2Material");
	model->SetVariableMaterialsByKey(3, L"Tree000_3Material");
	model->SetVariableMaterialsByKey(4, L"Tree000_4Material");

#pragma endregion
	model = Find<Model>(L"BlockBrickBreak");
	model->SetVariableMaterialsByKey(0, L"BlockBrickMaterial");
	model->SetVariableMaterialsByKey(1, L"BlockBrickMaterial");
	model->SetVariableMaterialsByKey(2, L"BlockBrickMaterial");
	model->SetVariableMaterialsByKey(3, L"BlockBrickMaterial");
	model->SetVariableMaterialsByKey(4, L"BlockBrickMaterial");
	model->SetVariableMaterialsByKey(5, L"BlockBrickMaterial");
	model->SetVariableMaterialsByKey(6, L"BlockBrickMaterial");
}
