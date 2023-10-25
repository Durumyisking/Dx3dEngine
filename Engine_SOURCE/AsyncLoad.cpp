#include "AsyncLoad.h"
#include "ResourceMgr.h"
#include "Texture.h"
#include "Material.h"

AsyncLoad::AsyncLoad()
	: mbLoadFinish(false)
	, mbTextureLoadFinish (false)
	, mbMarioLoadFinish (false)
	, mbMonsterLoadFinish (false)
	, mbMapLoadFinish (false)
	, mbSoundLoad(false)
{
}
AsyncLoad::~AsyncLoad()
{

}


void AsyncLoad::Update()
{
	if (mbTextureLoadFinish &&
		mbMarioLoadFinish &&
		mbMonsterLoadFinish &&
		//mbMapLoadFinish &&
		mbSoundLoad)
	{
		mbLoadFinish = true;
	}
}

void AsyncLoad::LoadModels()
{
	std::thread thread1([this]()
	{
		GETSINGLE(ResourceMgr)->LoadModel_Mario(&mbMarioLoadFinish);
	});
	thread1.detach();
	
	//std::thread thread2([this]()
	//{
	//	GETSINGLE(ResourceMgr)->LoadModel_Monster(&mbMonsterLoadFinish);
	//});
	//thread2.detach();	

	GETSINGLE(ResourceMgr)->LoadModel_Monster(&mbMonsterLoadFinish);

	// 쓰레드를 너무 많이 돌리면 제대로 동작을 안하나봄? 똑같은 코드 LoadModel_Monster 여기서 돌리면 잘 됨
	//std::thread thread3([this]()
	//{
	//	GETSINGLE(ResourceMgr)->LoadModel_CityWorld(&mbMapLoadFinish);
	//});
	//thread3.detach();
}

void AsyncLoad::LoadTextures()
{

	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaBody_alb", L"goomba/Image/KuriboBody_alb.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaBody_nrm", L"goomba/Image/KuriboBody_nrm.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaBody_rgh", L"goomba/Image/KuriboBody_rgh.png");

	// goomba
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaEye_alb0", L"goomba/Image/KuriboEye_alb.0.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaEye_alb1", L"goomba/Image/KuriboEye_alb.1.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaEye_alb2", L"goomba/Image/KuriboEye_alb.2.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaEye_nrm0", L"goomba/Image/KuriboEye_nrm.0.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaEye_nrm1", L"goomba/Image/KuriboEye_nrm.1.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaEye_nrm2", L"goomba/Image/KuriboEye_nrm.2.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaEye_rgh0", L"goomba/Image/KuriboEye_rgh.0.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaEye_rgh1", L"goomba/Image/KuriboEye_rgh.1.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaEye_rgh2", L"goomba/Image/KuriboEye_rgh.2.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaEye_emm", L"goomba/Image/KuriboEye_emm.png");

	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaHairFace_alb", L"goomba/Image/MarioHairFace_alb.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaHairFace_nrm", L"goomba/Image/MarioHairFace_nrm.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaHairFace_rgh", L"goomba/Image/MarioHairFace_rgh.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaHairFace_emm", L"goomba/Image/MarioHairFace_emm.png");

	// mario
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioBody_alb", L"MarioBody/Image/MarioBody_alb.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioBody_nrm", L"MarioBody/Image/MarioBody_nrm.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioBody_mtl", L"MarioBody/Image/MarioBody_mtl.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioBody_rgh", L"MarioBody/Image/MarioBody_rgh.png");

	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioCap_alb", L"MarioCap/Image/MarioCap_alb.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioCap_nrm", L"MarioCap/Image/MarioCap_nrm.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioCap_mtl", L"MarioCap/Image/MarioCap_mtl.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioCap_rgh", L"MarioCap/Image/MarioCap_rgh.png");

	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioEye_alb", L"MarioEye/Image/MarioEye_alb.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioEye_rgh", L"MarioEye/Image/MarioEye_rgh.png");

	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioEyePupil_alb", L"MarioEye/Image/MarioEyePupil_alb.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioEyePupil_nrm", L"MarioEye/Image/MarioEyePupil_nrm.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioEyePupil_mtl", L"MarioEye/Image/MarioEyePupil_mtl.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioEyePupil_rgh", L"MarioEye/Image/MarioEyePupil_rgh.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioEyePupil_emm", L"MarioEye/Image/MarioEyePupil_emm.png");

	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioEyeBall_alb", L"MarioFace/Image/MarioEyeBall_alb.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioEyeBall_rgh", L"MarioFace/Image/MarioEyeBall_rgh.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioEyeBall_emm", L"MarioFace/Image/MarioEyeBall_emm.png");

	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioFace_alb", L"MarioFace/Image/MarioFace_alb.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioFace_rgh", L"MarioFace/Image/MarioFace_rgh.png");

	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioHairFace_alb", L"MarioFace/Image/MarioHairFace_alb.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioHairFace_nrm", L"MarioFace/Image/MarioHairFace_nrm.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioHairFace_rgh", L"MarioFace/Image/MarioHairFace_rgh.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioHairFace_emm", L"MarioFace/Image/MarioHairFace_emm.png");

	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioHand_alb", L"MarioHandR/Image/MarioHand_alb.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioHand_nrm", L"MarioHandR/Image/MarioHand_nrm.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioHand_rgh", L"MarioHandR/Image/MarioHand_rgh.png");

	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioHair_alb", L"MarioHead/Image/MarioHair_alb.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioHair_nrm", L"MarioHead/Image/MarioHair_nrm.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"marioHair_rgh", L"MarioHead/Image/MarioHair_rgh.png");


	GETSINGLE(ResourceMgr)->Load<Texture>(L"SkySphereTexture", L"SkySphere/SkyCityNightStar_color.png");

	GETSINGLE(ResourceMgr)->Load<Texture>(L"night1", L"Cube/night/DarkNight_.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"night2", L"Cube/night/DarkNight_Scenario2.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"night3", L"Cube/night/DarkNight_Scenario3.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"night4", L"Cube/night/DarkNight_Scenario4.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"night5", L"Cube/night/DarkNight_Scenario5.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"night6", L"Cube/night/DarkNight_Scenario6.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"night7", L"Cube/night/DarkNight_Scenario7.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"night8", L"Cube/night/DarkNight_Scenario8.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"night9", L"Cube/night/DarkNight_Scenario9.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"night10", L"Cube/night/DarkNight_Scenario10.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"night11", L"Cube/night/DarkNight_Scenario11.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"night12", L"Cube/night/DarkNight_Scenario12.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"night13", L"Cube/night/DarkNight_Scenario13.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"night14", L"Cube/night/DarkNight_Scenario14.dds");
	 
	GETSINGLE(ResourceMgr)->Load<Texture>(L"skyonly1", L"Cube/people/SkyOnly_.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"skyonly2", L"Cube/people/SkyOnly_Scenario2.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"skyonly3", L"Cube/people/SkyOnly_Scenario3.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"skyonly4", L"Cube/people/SkyOnly_Scenario4.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"skyonly5", L"Cube/people/SkyOnly_Scenario5.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"skyonly6", L"Cube/people/SkyOnly_Scenario6.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"skyonly7", L"Cube/people/SkyOnly_Scenario7.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"skyonly8", L"Cube/people/SkyOnly_Scenario8.dds");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"skyonly9", L"Cube/people/SkyOnly_Scenario9.dds");


	renderer::CreateUITexture();

	loadCityTexture();
	LoadMaterials();

	mbTextureLoadFinish = true;
}

void AsyncLoad::LoadMaterials()
{
	// object materials
#pragma region SkySphere Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"SkySphereShader");
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"SkySphereTexture", eTextureSlot::SkySphere);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"SkySphereMaterial", material);
	}
#pragma endregion

#pragma region MarioMustache Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DeferredShader");
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"goombaHairFace_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(L"goombaHairFace_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(L"goombaHairFace_rgh", eTextureSlot::Roughness);
		material->SetTextureByKey(L"goombaHairFace_emm", eTextureSlot::Emissive);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"MarioMustacheMaterial", material);
	}
#pragma endregion

#pragma region basicPostProcess Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"BasicPostProcessShader");
		Material* material = new Material();
		material->SetShader(shader);
		material->SetRenderingMode(eRenderingMode::PostProcess);
		material->SetTextureByKey(L"PostProcessTexture", eTextureSlot::Albedo);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"BasicPostProcessMaterial", material);
	}
#pragma region lensFlare Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"LensFlareShader");
		Material* material = new Material();
		material->SetShader(shader);
		material->SetRenderingMode(eRenderingMode::PostProcess);
		material->SetTextureByKey(L"PostProcessTexture", eTextureSlot::Albedo);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"LensFlareMaterial", material);
	}
#pragma endregion


	loadGoomba(L"DeferredShader");
	loadMario( L"DeferredShader");
	loadCityObjectMaterial();


	renderer::CreateUIMaterial();
}

void AsyncLoad::loadCityObjectMaterial()
{
#pragma region CityWorldHomeBuilding000 Material
	{
		createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding000_0Material", TextureState::Al);
		createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding000_1Material", TextureState::Em);
		createMaterial(L"ConcreteWallMain00", L"DeferredShader", L"HomeBuilding000_2Material", TextureState::AlNrRg);
		createMaterial(L"ConcreteWallMain01", L"DeferredShader", L"HomeBuilding000_3Material", TextureState::AlEmNrRg);
		createMaterial(L"ConcreteWallMain02", L"DeferredShader", L"HomeBuilding000_4Material", TextureState::AlNrRg);
		createMaterial(L"ConcreteWallMain03", L"DeferredShader", L"HomeBuilding000_5Material", TextureState::AlNrRg);
		createMaterial(L"ConcreteWallMain04", L"DeferredShader", L"HomeBuilding000_6Material", TextureState::AlNrRg);
		createMaterial(L"ConcreteWallMain05", L"DeferredShader", L"HomeBuilding000_7Material", TextureState::AlNrRg);
		createMaterial(L"ConcreteWallMain06", L"DeferredShader", L"HomeBuilding000_8Material", TextureState::AlNrRg);
		createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding000_9Material", TextureState::Em);
		createMaterial(L"GlassWindowMain00", L"DeferredShader", L"HomeBuilding000_10Material", TextureState::AlEmMsNrRg);
		createMaterial(L"GroundSoil00", L"DeferredShader", L"HomeBuilding000_11Material", TextureState::AlNrRg);
		//Light01
		createMaterial(L"MainTowerFont00", L"DeferredShader", L"HomeBuilding000_12Material", TextureState::AlNrRg);
		createMaterial(L"MetalfloorMain00", L"DeferredShader", L"HomeBuilding000_13Material", TextureState::AlNrRg);
		createMaterial(L"MetalWallMain00", L"DeferredShader", L"HomeBuilding000_14Material", TextureState::AlNrRg);
		createMaterial(L"MetalWallMain01", L"DeferredShader", L"HomeBuilding000_15Material", TextureState::AlEmNrRg);
		createMaterial(L"MetalWallMain02", L"DeferredShader", L"HomeBuilding000_16Material", TextureState::AlNrRg);
		createMaterial(L"MetalWallMain03", L"DeferredShader", L"HomeBuilding000_17Material", TextureState::AlMtNrRg);
		createMaterial(L"TopFloorMain00", L"DeferredShader", L"HomeBuilding000_18Material", TextureState::AlNrRg);
		createMaterial(L"WindowGlassEmn00", L"DeferredShader", L"HomeBuilding000_19Material", TextureState::AlEmMsNrRg);
		createMaterial(L"WindowGlassEmn01", L"DeferredShader", L"HomeBuilding000_20Material", TextureState::Em);

		//Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding000");

		//if (model)
		//{
		//	model->SetVariableMaterialsByKey(0, L"HomeBuilding000_0Material");
		//	model->SetVariableMaterialsByKey(1, L"HomeBuilding000_1Material");
		//	model->SetVariableMaterialsByKey(2, L"HomeBuilding000_2Material");
		//	model->SetVariableMaterialsByKey(3, L"HomeBuilding000_3Material");
		//	model->SetVariableMaterialsByKey(4, L"HomeBuilding000_4Material");
		//	model->SetVariableMaterialsByKey(5, L"HomeBuilding000_5Material");
		//	model->SetVariableMaterialsByKey(6, L"HomeBuilding000_6Material");
		//	model->SetVariableMaterialsByKey(7, L"HomeBuilding000_7Material");
		//	model->SetVariableMaterialsByKey(8, L"HomeBuilding000_8Material");
		//	model->SetVariableMaterialsByKey(9, L"HomeBuilding000_9Material");
		//	model->SetVariableMaterialsByKey(10, L"HomeBuilding000_10Material");
		//	model->SetVariableMaterialsByKey(11, L"HomeBuilding000_11Material");
		//	model->SetVariableMaterialsByKey(12, L"HomeBuilding000_12Material");
		//	model->SetVariableMaterialsByKey(13, L"HomeBuilding000_13Material");
		//	model->SetVariableMaterialsByKey(14, L"HomeBuilding000_14Material");
		//	model->SetVariableMaterialsByKey(15, L"HomeBuilding000_15Material");
		//	model->SetVariableMaterialsByKey(16, L"HomeBuilding000_16Material");
		//	model->SetVariableMaterialsByKey(17, L"HomeBuilding000_17Material");
		//	model->SetVariableMaterialsByKey(18, L"HomeBuilding000_13Material");
		//	model->SetVariableMaterialsByKey(19, L"HomeBuilding000_18Material");
		//	model->SetVariableMaterialsByKey(20, L"HomeBuilding000_19Material");
		//	model->SetVariableMaterialsByKey(21, L"HomeBuilding000_20Material");
		//}
	}
#pragma endregion

#pragma region CityWorldHomeBuilding001 Material
	{
		createMaterial(L"GlassBuildingWall00", L"DeferredShader", L"HomeBuilding001_0Material", TextureState::AlEmMtNrRg);
		createMaterial(L"RoofConcrete00", L"DeferredShader", L"HomeBuilding001_1Material", TextureState::AlNrRg);
		createMaterial(L"RoofConcrete01", L"DeferredShader", L"HomeBuilding001_2Material", TextureState::AlNrRg);
		createMaterial(L"WallGlassPaintedSteel00", L"DeferredShader", L"HomeBuilding001_3Material", TextureState::AlNr);

		//Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding001");

		//if (model)
		//{
		//	model->SetVariableMaterialsByKey(0, L"HomeBuilding001_0Material");
		//	model->SetVariableMaterialsByKey(1, L"HomeBuilding001_1Material");
		//	model->SetVariableMaterialsByKey(2, L"HomeBuilding001_2Material");
		//	model->SetVariableMaterialsByKey(3, L"HomeBuilding001_3Material");
		//}
	}
#pragma endregion

#pragma region CityWorldHomeBuilding002 Material
	{
		createMaterial(L"GlassBuildingWall00", L"DeferredShader", L"HomeBuilding002_0Material", TextureState::AlEmMtNrRg);
		createMaterial(L"RoofConcrete00", L"DeferredShader", L"HomeBuilding002_1Material", TextureState::AlNrRg);
		createMaterial(L"RoofConcrete01", L"DeferredShader", L"HomeBuilding002_2Material", TextureState::AlNrRg);
		createMaterial(L"WallGlassPaintedSteel00", L"DeferredShader", L"HomeBuilding002_3Material", TextureState::AlNr);

		//Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding002");

		//if (model)
		//{
		//	model->SetVariableMaterialsByKey(0, L"HomeBuilding002_0Material");
		//	model->SetVariableMaterialsByKey(1, L"HomeBuilding002_1Material");
		//	model->SetVariableMaterialsByKey(2, L"HomeBuilding002_2Material");
		//	model->SetVariableMaterialsByKey(3, L"HomeBuilding002_3Material");
		//}
	}
#pragma endregion

#pragma region CityWorldHomeBuilding003 Material
	{
		createMaterial(L"GroundLawn00", L"DeferredShader", L"HomeBuilding003_0Material", TextureState::AlNrRg);
		createMaterial(L"MetalFence00", L"DeferredShader", L"HomeBuilding003_1Material", TextureState::AlMtNrRg);

		//Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding003");

		//if (model)
		//{
		//	model->SetVariableMaterialsByKey(0, L"HomeBuilding003_0Material");
		//	model->SetVariableMaterialsByKey(1, L"HomeBuilding003_1Material");
		//}
	}

#pragma endregion

#pragma region CityWorldHomeBuilding004 Material
	{
		createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding004_0Material", TextureState::AlEm);
		createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding004_1Material", TextureState::Em);
		createMaterial(L"BuildingBrickWall02_rep", L"DeferredShader", L"HomeBuilding004_2Material", TextureState::Al);
		//createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding004_3Material", TextureState::Em);
		createMaterial(L"BuildingStoneParts01", L"DeferredShader", L"HomeBuilding004_3Material", TextureState::Al);
		createMaterial(L"BuildingStoneWall00_rep", L"DeferredShader", L"HomeBuilding004_4Material", TextureState::AlNrRg);
		createMaterial(L"BuildingStoneWall02_rep", L"DeferredShader", L"HomeBuilding004_5Material", TextureState::Al);
		createMaterial(L"BuildingWindowSet00", L"DeferredShader", L"HomeBuilding004_6Material", TextureState::AlEmNrRg);
		createMaterial(L"BuildingWindowSet01", L"DeferredShader", L"HomeBuilding004_7Material", TextureState::AlEmMtNrRg);
		createMaterial(L"DoorWoodBill00", L"DeferredShader", L"HomeBuilding004_8Material", TextureState::AlNr);
		createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding004_9Material", TextureState::Em);
		createMaterial(L"WallConcreteFlat01", L"DeferredShader", L"HomeBuilding004_10Material", TextureState::AlNr);
		createMaterial(L"WallConcreteFlat02", L"DeferredShader", L"HomeBuilding004_11Material", TextureState::AlNr);
		createMaterial(L"WallConcreteTopflloor02", L"DeferredShader", L"HomeBuilding004_12Material", TextureState::AlNrRg);
		createMaterial(L"WallConcreteTopVer00", L"DeferredShader", L"HomeBuilding004_13Material", TextureState::AlNr);

		//Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding004");

		//if (model)
		//{
		//	model->SetVariableMaterialsByKey(0, L"HomeBuilding004_0Material");
		//	model->SetVariableMaterialsByKey(1, L"HomeBuilding004_1Material");
		//	model->SetVariableMaterialsByKey(2, L"HomeBuilding004_2Material");
		//	model->SetVariableMaterialsByKey(3, L"HomeBuilding004_10Material");
		//	model->SetVariableMaterialsByKey(4, L"HomeBuilding004_3Material");
		//	model->SetVariableMaterialsByKey(5, L"HomeBuilding004_4Material");
		//	model->SetVariableMaterialsByKey(6, L"HomeBuilding004_5Material");
		//	model->SetVariableMaterialsByKey(7, L"HomeBuilding004_6Material");
		//	model->SetVariableMaterialsByKey(8, L"HomeBuilding004_7Material");
		//	model->SetVariableMaterialsByKey(9, L"HomeBuilding004_8Material");
		//	model->SetVariableMaterialsByKey(10, L"HomeBuilding004_9Material");
		//	model->SetVariableMaterialsByKey(11, L"HomeBuilding004_10Material");
		//	model->SetVariableMaterialsByKey(12, L"HomeBuilding004_11Material");
		//	model->SetVariableMaterialsByKey(13, L"HomeBuilding004_13Material");
		//	model->SetVariableMaterialsByKey(14, L"HomeBuilding004_12Material");
		//}
	}

#pragma endregion

#pragma region CityWorldHomeBuilding005 Material
	{
		createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding005_0Material", TextureState::Al);
		createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding005_1Material", TextureState::Em);
		createMaterial(L"BuildingBrickWall00_rep", L"DeferredShader", L"HomeBuilding005_2Material", TextureState::AlNrRg);
		createMaterial(L"BuildingStoneParts00", L"DeferredShader", L"HomeBuilding005_3Material", TextureState::AlNrRg);
		createMaterial(L"BuildingStoneWall00_rep", L"DeferredShader", L"HomeBuilding005_4Material", TextureState::AlNrRg);
		createMaterial(L"BuildingStoneWall01_rep", L"DeferredShader", L"HomeBuilding005_5Material", TextureState::Al);
		createMaterial(L"BuildingWindowSet00", L"DeferredShader", L"HomeBuilding005_6Material", TextureState::AlEmNrRg);
		createMaterial(L"BuildingWindowSet02", L"DeferredShader", L"HomeBuilding005_7Material", TextureState::Al);
		createMaterial(L"DoorWoodBill00", L"DeferredShader", L"HomeBuilding005_8Material", TextureState::AlNr);
		createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding005_9Material", TextureState::Em);
		createMaterial(L"WallConcreteFlat01", L"DeferredShader", L"HomeBuilding005_10Material", TextureState::AlNr);
		createMaterial(L"WallConcreteFlat02", L"DeferredShader", L"HomeBuilding005_11Material", TextureState::AlNr);
		createMaterial(L"WallConcreteTopflloor02", L"DeferredShader", L"HomeBuilding005_12Material", TextureState::AlNrRg);
		createMaterial(L"WallConcreteTopVer00", L"DeferredShader", L"HomeBuilding005_13Material", TextureState::AlNr);

		//Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding005");

		//if (model)
		//{
		//	model->SetVariableMaterialsByKey(0, L"HomeBuilding005_0Material");
		//	model->SetVariableMaterialsByKey(1, L"HomeBuilding005_1Material");
		//	model->SetVariableMaterialsByKey(2, L"HomeBuilding005_2Material");
		//	model->SetVariableMaterialsByKey(3, L"HomeBuilding005_10Material");
		//	model->SetVariableMaterialsByKey(4, L"HomeBuilding005_3Material");
		//	model->SetVariableMaterialsByKey(5, L"HomeBuilding005_4Material");
		//	model->SetVariableMaterialsByKey(6, L"HomeBuilding005_5Material");
		//	model->SetVariableMaterialsByKey(7, L"HomeBuilding005_6Material");
		//	model->SetVariableMaterialsByKey(8, L"HomeBuilding005_7Material");
		//	model->SetVariableMaterialsByKey(9, L"HomeBuilding005_8Material");
		//	model->SetVariableMaterialsByKey(10, L"HomeBuilding005_9Material");
		//	model->SetVariableMaterialsByKey(11, L"HomeBuilding005_10Material");
		//	model->SetVariableMaterialsByKey(12, L"HomeBuilding005_11Material");
		//	model->SetVariableMaterialsByKey(13, L"HomeBuilding005_13Material");
		//	model->SetVariableMaterialsByKey(14, L"HomeBuilding005_12Material");
		//}
	}

#pragma endregion

#pragma region CityWorldHomeBuilding007 Material
	{
		createMaterial(L"DoorGlassSteel00", L"DeferredShader", L"HomeBuilding007_0Material", TextureState::AlNr);
		createMaterial(L"GlassBuildingWall00", L"DeferredShader", L"HomeBuilding007_1Material", TextureState::AlEmMtNrRg);
		createMaterial(L"RoofConcrete00", L"DeferredShader", L"HomeBuilding007_2Material", TextureState::AlNrRg);
		createMaterial(L"RoofConcrete01", L"DeferredShader", L"HomeBuilding007_3Material", TextureState::AlNrRg);
		createMaterial(L"WallGlassPaintedSteel00", L"DeferredShader", L"HomeBuilding007_4Material", TextureState::AlNr);


		//Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding007");

		//if (model)
		//{
		//	model->SetVariableMaterialsByKey(0, L"HomeBuilding007_0Material");
		//	model->SetVariableMaterialsByKey(1, L"HomeBuilding007_1Material");
		//	model->SetVariableMaterialsByKey(2, L"HomeBuilding007_0Material");
		//	model->SetVariableMaterialsByKey(3, L"HomeBuilding007_1Material");
		//	model->SetVariableMaterialsByKey(4, L"HomeBuilding007_2Material");
		//	model->SetVariableMaterialsByKey(5, L"HomeBuilding007_3Material");
		//	model->SetVariableMaterialsByKey(6, L"HomeBuilding007_4Material");
		//}
	}

#pragma endregion

#pragma region CityWorldHomeBuilding008 Material
	{
		createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding008_0Material", TextureState::AlEm);
		createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding008_1Material", TextureState::Em);
		createMaterial(L"DoorGlassSteel00", L"DeferredShader", L"HomeBuilding008_2Material", TextureState::AlNr);
		createMaterial(L"GlassBuildingWall00", L"DeferredShader", L"HomeBuilding008_3Material", TextureState::AlEmMtNrRg);
		createMaterial(L"RoofConcrete00", L"DeferredShader", L"HomeBuilding008_4Material", TextureState::AlNrRg);
		createMaterial(L"RoofConcrete01", L"DeferredShader", L"HomeBuilding008_5Material", TextureState::AlNrRg);
		createMaterial(L"WallGlassPaintedSteel00", L"DeferredShader", L"HomeBuilding008_6Material", TextureState::AlNr);

		//Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding008");

		//if (model)
		//{
		//	model->SetVariableMaterialsByKey(0, L"HomeBuilding008_0Material");
		//	model->SetVariableMaterialsByKey(1, L"HomeBuilding008_1Material");
		//	model->SetVariableMaterialsByKey(2, L"HomeBuilding008_2Material");
		//	model->SetVariableMaterialsByKey(3, L"HomeBuilding008_3Material");
		//	model->SetVariableMaterialsByKey(4, L"HomeBuilding008_2Material");
		//	model->SetVariableMaterialsByKey(5, L"HomeBuilding008_3Material");
		//	model->SetVariableMaterialsByKey(6, L"HomeBuilding008_4Material");
		//	model->SetVariableMaterialsByKey(7, L"HomeBuilding008_5Material");
		//	model->SetVariableMaterialsByKey(8, L"HomeBuilding008_6Material");
		//}
	}


#pragma endregion

#pragma region CityWorldHomeBuilding009 Material
	{
		createMaterial(L"ConcreteWallMain01", L"DeferredShader", L"HomeBuilding009_0Material", TextureState::Al);
		createMaterial(L"DoorGlassSteel00", L"DeferredShader", L"HomeBuilding009_1Material", TextureState::AlNr);
		createMaterial(L"GlassBuildingWall00", L"DeferredShader", L"HomeBuilding009_2Material", TextureState::AlEmMtNrRg);
		createMaterial(L"MetalFence00", L"DeferredShader", L"HomeBuilding009_3Material", TextureState::AlMtNrRg);
		createMaterial(L"RoofConcrete00", L"DeferredShader", L"HomeBuilding009_4Material", TextureState::AlNrRg);
		createMaterial(L"RoofConcrete01", L"DeferredShader", L"HomeBuilding009_5Material", TextureState::AlNrRg);
		createMaterial(L"WallGlassPaintedSteel00", L"DeferredShader", L"HomeBuilding009_6Material", TextureState::AlNr);

		//Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding009");

		//if (model)
		//{
		//	model->SetVariableMaterialsByKey(0, L"HomeBuilding009_0Material");
		//	model->SetVariableMaterialsByKey(1, L"HomeBuilding009_1Material");
		//	model->SetVariableMaterialsByKey(2, L"HomeBuilding009_2Material");
		//	model->SetVariableMaterialsByKey(3, L"HomeBuilding009_1Material");
		//	model->SetVariableMaterialsByKey(4, L"HomeBuilding009_2Material");
		//	model->SetVariableMaterialsByKey(5, L"HomeBuilding009_3Material");
		//	model->SetVariableMaterialsByKey(6, L"HomeBuilding009_4Material");
		//	model->SetVariableMaterialsByKey(7, L"HomeBuilding009_5Material");
		//	model->SetVariableMaterialsByKey(8, L"HomeBuilding009_6Material");
		//}
	}

#pragma endregion

#pragma region CityWorldHomeBuilding0010 Material
	{
		createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding0010_0Material", TextureState::AlEm);
		createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding0010_1Material", TextureState::Em);
		createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding0010_2Material", TextureState::Em);
		createMaterial(L"HouseAttachmentSet00", L"DeferredShader", L"HomeBuilding0010_3Material", TextureState::AlEmMsMtNr);
		createMaterial(L"HouseAttachmentSet03", L"DeferredShader", L"HomeBuilding0010_4Material", TextureState::Al);
		createMaterial(L"HouseAttachmentTop00", L"DeferredShader", L"HomeBuilding0010_5Material", TextureState::AlNr);
		createMaterial(L"HouseBrickWall00", L"DeferredShader", L"HomeBuilding0010_6Material", TextureState::AlNrRg);
		createMaterial(L"HouseIronFence01", L"DeferredShader", L"HomeBuilding0010_7Material", TextureState::Al);
		createMaterial(L"HousePaintedIron00", L"DeferredShader", L"HomeBuilding0010_8Material", TextureState::AlNr);
		createMaterial(L"WallConcreteTopflloor06", L"DeferredShader", L"HomeBuilding0010_9Material", TextureState::AlNrRg);
		createMaterial(L"WallConcreteTopVer01", L"DeferredShader", L"HomeBuilding0010_10Material", TextureState::AlNr);

		//Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding010");

		//if (model)
		//{
		//	model->SetVariableMaterialsByKey(0, L"HomeBuilding0010_0Material");
		//	model->SetVariableMaterialsByKey(1, L"HomeBuilding0010_1Material");
		//	model->SetVariableMaterialsByKey(2, L"HomeBuilding0010_3Material");
		//	model->SetVariableMaterialsByKey(3, L"HomeBuilding0010_3Material");
		//	model->SetVariableMaterialsByKey(4, L"HomeBuilding0010_4Material");
		//	model->SetVariableMaterialsByKey(5, L"HomeBuilding0010_5Material");
		//	model->SetVariableMaterialsByKey(6, L"HomeBuilding0010_6Material");
		//	model->SetVariableMaterialsByKey(7, L"HomeBuilding0010_7Material");
		//	model->SetVariableMaterialsByKey(8, L"HomeBuilding0010_8Material");
		//	model->SetVariableMaterialsByKey(9, L"HomeBuilding0010_9Material");
		//	model->SetVariableMaterialsByKey(10, L"HomeBuilding0010_9Material");
		//	model->SetVariableMaterialsByKey(11, L"HomeBuilding0010_10Material");
		//}
	}

#pragma endregion

#pragma region CityWorldHomeBuilding0011 Material
	{
		createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding0011_0Material", TextureState::AlEm);
		createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding0011_1Material", TextureState::Em);
		createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding0011_2Material", TextureState::Em);
		createMaterial(L"HouseAttachmentSet00", L"DeferredShader", L"HomeBuilding0011_3Material", TextureState::AlEmMsMtNr);
		createMaterial(L"HouseAttachmentSet03", L"DeferredShader", L"HomeBuilding0011_4Material", TextureState::Al);
		createMaterial(L"HouseAttachmentTop00", L"DeferredShader", L"HomeBuilding0011_5Material", TextureState::AlNr);
		createMaterial(L"HouseBrickWall02", L"DeferredShader", L"HomeBuilding0011_6Material", TextureState::Al);
		createMaterial(L"HouseIronFence00", L"DeferredShader", L"HomeBuilding0011_7Material", TextureState::AlNrRg);
		createMaterial(L"HousePaintedIron00", L"DeferredShader", L"HomeBuilding0011_8Material", TextureState::AlNr);
		createMaterial(L"WallConcreteTopflloor06", L"DeferredShader", L"HomeBuilding0011_9Material", TextureState::AlNrRg);
		createMaterial(L"WallConcreteTopVer01", L"DeferredShader", L"HomeBuilding0011_10Material", TextureState::AlNr);

		//Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding011");

		//if (model)
		//{
		//	model->SetVariableMaterialsByKey(0, L"HomeBuilding0011_0Material");
		//	model->SetVariableMaterialsByKey(1, L"HomeBuilding0011_1Material");
		//	model->SetVariableMaterialsByKey(2, L"HomeBuilding0010_3Material");
		//	model->SetVariableMaterialsByKey(3, L"HomeBuilding0011_3Material");
		//	model->SetVariableMaterialsByKey(4, L"HomeBuilding0011_4Material");
		//	model->SetVariableMaterialsByKey(5, L"HomeBuilding0011_5Material");
		//	model->SetVariableMaterialsByKey(6, L"HomeBuilding0011_6Material");
		//	model->SetVariableMaterialsByKey(7, L"HomeBuilding0011_7Material");
		//	model->SetVariableMaterialsByKey(8, L"HomeBuilding0011_8Material");
		//	model->SetVariableMaterialsByKey(9, L"HomeBuilding0011_9Material");
		//	model->SetVariableMaterialsByKey(10, L"HomeBuilding0011_9Material");
		//	model->SetVariableMaterialsByKey(11, L"HomeBuilding0011_10Material");
		//}
	}

#pragma endregion

#pragma region CityWorldHomeBuilding0012 Material
	{
		createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding0012_0Material", TextureState::AlEm);
		createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding0012_1Material", TextureState::Em);
		createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding0012_2Material", TextureState::Em);
		createMaterial(L"HouseAttachmentSet00", L"DeferredShader", L"HomeBuilding0012_3Material", TextureState::AlEmMsMtNr);
		createMaterial(L"HouseAttachmentSet03", L"DeferredShader", L"HomeBuilding0012_4Material", TextureState::Al);
		createMaterial(L"HouseAttachmentTop00", L"DeferredShader", L"HomeBuilding0012_5Material", TextureState::AlNr);
		createMaterial(L"HouseBrickWall01", L"DeferredShader", L"HomeBuilding0012_6Material", TextureState::Al);
		createMaterial(L"HouseIronFence02", L"DeferredShader", L"HomeBuilding0012_7Material", TextureState::Al);
		createMaterial(L"HousePaintedIron00", L"DeferredShader", L"HomeBuilding0012_8Material", TextureState::AlNr);
		createMaterial(L"WallConcreteTopflloor06", L"DeferredShader", L"HomeBuilding0012_9Material", TextureState::AlNrRg);
		createMaterial(L"WallConcreteTopVer01", L"DeferredShader", L"HomeBuilding0012_10Material", TextureState::AlNr);

		//Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding012");

		//if (model)
		//{
		//	model->SetVariableMaterialsByKey(0, L"HomeBuilding0012_0Material");
		//	model->SetVariableMaterialsByKey(1, L"HomeBuilding0012_1Material");
		//	model->SetVariableMaterialsByKey(2, L"HomeBuilding0012_3Material");
		//	model->SetVariableMaterialsByKey(3, L"HomeBuilding0012_3Material");
		//	model->SetVariableMaterialsByKey(4, L"HomeBuilding0012_4Material");
		//	model->SetVariableMaterialsByKey(5, L"HomeBuilding0012_5Material");
		//	model->SetVariableMaterialsByKey(6, L"HomeBuilding0012_6Material");
		//	model->SetVariableMaterialsByKey(7, L"HomeBuilding0012_7Material");
		//	model->SetVariableMaterialsByKey(8, L"HomeBuilding0012_8Material");
		//	model->SetVariableMaterialsByKey(9, L"HomeBuilding0012_9Material");
		//	model->SetVariableMaterialsByKey(10, L"HomeBuilding0012_9Material");
		//	model->SetVariableMaterialsByKey(11, L"HomeBuilding0012_10Material");
		//}
	}

#pragma endregion

#pragma region CityWorldHomeBuilding0013 Material
	{
		createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding0013_0Material", TextureState::Em);
		createMaterial(L"HouseAttachmentSet00", L"DeferredShader", L"HomeBuilding0013_1Material", TextureState::AlEmMsMtNr);
		createMaterial(L"HouseAttachmentSet03", L"DeferredShader", L"HomeBuilding0013_2Material", TextureState::Al);
		createMaterial(L"HouseAttachmentTop01", L"DeferredShader", L"HomeBuilding0013_3Material", TextureState::Al);
		createMaterial(L"HouseBrickWall00", L"DeferredShader", L"HomeBuilding0013_4Material", TextureState::AlNrRg);
		createMaterial(L"HouseIronFence02", L"DeferredShader", L"HomeBuilding0013_5Material", TextureState::Al);
		createMaterial(L"HousePaintedIron00", L"DeferredShader", L"HomeBuilding0013_6Material", TextureState::AlNr);
		createMaterial(L"WallConcreteTopflloor06", L"DeferredShader", L"HomeBuilding0013_7Material", TextureState::AlNrRg);
		createMaterial(L"WallConcreteTopVer01", L"DeferredShader", L"HomeBuilding0013_8Material", TextureState::AlNr);

		//Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding013");

		//if (model)
		//{
		//	model->SetVariableMaterialsByKey(0, L"HomeBuilding0013_1Material");
		//	model->SetVariableMaterialsByKey(1, L"HomeBuilding0013_1Material");
		//	model->SetVariableMaterialsByKey(2, L"HomeBuilding0013_2Material");
		//	model->SetVariableMaterialsByKey(3, L"HomeBuilding0013_3Material");
		//	model->SetVariableMaterialsByKey(4, L"HomeBuilding0013_4Material");
		//	model->SetVariableMaterialsByKey(5, L"HomeBuilding0013_5Material");
		//	model->SetVariableMaterialsByKey(6, L"HomeBuilding0013_6Material");
		//	model->SetVariableMaterialsByKey(7, L"HomeBuilding0013_7Material");
		//	model->SetVariableMaterialsByKey(8, L"HomeBuilding0013_7Material");
		//	model->SetVariableMaterialsByKey(9, L"HomeBuilding0013_8Material");
		//}
	}

#pragma endregion

#pragma region CityWorldHomeBuilding0014 Material
	//createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding0014_0Material", TextureState::AlEm);
	//createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding0014_1Material", TextureState::Em);
	//createMaterial(L"EntranceStreetLight00", L"DeferredShader", L"HomeBuilding0014_2Material", TextureState::AlNrRg);
	////createMaterial(L"EntranceStreetLight01", L"DeferredShader", L"HomeBuilding0014_3Material", TextureState::AlNrRg); //없거나 찾아봐야함
	////createMaterial(L"EntranceStreetLight02", L"DeferredShader", L"HomeBuilding0014_4Material", TextureState::AlNrRg); //없거나 찾아봐야함
	//createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding0014_3Material", TextureState::Em);
	//createMaterial(L"WallConcreteClaf00", L"DeferredShader", L"HomeBuilding0014_4Material", TextureState::AlNrRg);
	//createMaterial(L"WallConcreteClaf01", L"DeferredShader", L"HomeBuilding0014_5Material", TextureState::AlNrRg);
	//createMaterial(L"WallConcreteClaf02", L"DeferredShader", L"HomeBuilding0014_6Material", TextureState::AlNrRg);
	//createMaterial(L"WallConcreteClaf03", L"DeferredShader", L"HomeBuilding0014_7Material", TextureState::AlNrRg);
	//createMaterial(L"WallConcreteClaf04", L"DeferredShader", L"HomeBuilding0014_8Material", TextureState::AlNrRg);
	//createMaterial(L"WallConcreteClaf05", L"DeferredShader", L"HomeBuilding0014_9Material", TextureState::AlNrRg);
	//createMaterial(L"WallConcreteTopflloor07", L"DeferredShader", L"HomeBuilding0014_10Material", TextureState::AlNrRg);
	//createMaterial(L"WindowlGlassClaf01", L"DeferredShader", L"HomeBuilding0014_11Material", TextureState::AlMtMsNrRg);
	//createMaterial(L"WindowlGlassClaf02", L"DeferredShader", L"HomeBuilding0014_12Material", TextureState::AlEmNrRg);


	//Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding014");

	//if (model)
	//{
	//	model->SetVariableMaterialsByKey(0, L"HomeBuilding0014_0Material");
	//	model->SetVariableMaterialsByKey(1, L"HomeBuilding0014_1Material");
	//	model->SetVariableMaterialsByKey(2, L"HomeBuilding0014_2Material");
	//	model->SetVariableMaterialsByKey(3, L"HomeBuilding0014_2Material");
	//	model->SetVariableMaterialsByKey(4, L"HomeBuilding0014_2Material");
	//	model->SetVariableMaterialsByKey(5, L"HomeBuilding0014_5Material");
	//	model->SetVariableMaterialsByKey(6, L"HomeBuilding0014_4Material");
	//	model->SetVariableMaterialsByKey(7, L"HomeBuilding0014_5Material");
	//	model->SetVariableMaterialsByKey(8, L"HomeBuilding0014_6Material");
	//	model->SetVariableMaterialsByKey(9, L"HomeBuilding0014_7Material");
	//	model->SetVariableMaterialsByKey(10, L"HomeBuilding0014_8Material");
	//	model->SetVariableMaterialsByKey(11, L"HomeBuilding0014_9Material");
	//	model->SetVariableMaterialsByKey(12, L"HomeBuilding0014_10Material");
	//	model->SetVariableMaterialsByKey(13, L"HomeBuilding0014_11Material");
	//	model->SetVariableMaterialsByKey(14, L"HomeBuilding0014_12Material");
	//	model->SetVariableMaterialsByKey(15, L"HomeBuilding0014_12Material");
	//}

#pragma endregion

#pragma region CityWorldHomeBuilding0015 Material
	createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding0015_0Material", TextureState::Em);
	createMaterial(L"WallConcreteClaf00", L"DeferredShader", L"HomeBuilding0015_1Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteClaf01", L"DeferredShader", L"HomeBuilding0015_2Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteClaf02", L"DeferredShader", L"HomeBuilding0015_3Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteClaf03", L"DeferredShader", L"HomeBuilding0015_4Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteClaf04", L"DeferredShader", L"HomeBuilding0015_5Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteClaf05", L"DeferredShader", L"HomeBuilding0015_6Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteTopflloor07", L"DeferredShader", L"HomeBuilding0015_7Material", TextureState::AlNrRg);
	createMaterial(L"WindowlGlassClaf01", L"DeferredShader", L"HomeBuilding0015_8Material", TextureState::AlMtMsNrRg);
	createMaterial(L"WindowlGlassClaf02", L"DeferredShader", L"HomeBuilding0015_9Material", TextureState::AlEmNrRg);

#pragma endregion

#pragma region CityWorldHomeBuilding0016 Material
	createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding0016_0Material", TextureState::Em);
	createMaterial(L"WallConcreteFlat04", L"DeferredShader", L"HomeBuilding0016_1Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteFlat05", L"DeferredShader", L"HomeBuilding0016_2Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteFlat06", L"DeferredShader", L"HomeBuilding0016_3Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteFlat07", L"DeferredShader", L"HomeBuilding0016_4Material", TextureState::AlMtNrRg);
	createMaterial(L"WallConcreteFlat08", L"DeferredShader", L"HomeBuilding0016_5Material", TextureState::AlMtNrRg);
	createMaterial(L"WallConcreteTopflloor02", L"DeferredShader", L"HomeBuilding0016_6Material", TextureState::AlNrRg);
	createMaterial(L"WindowlGlassFlat00", L"DeferredShader", L"HomeBuilding0016_7Material", TextureState::AlNrRg);

#pragma endregion

#pragma region CityWorldHomeBuilding0017 Material
	createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding0017_0Material", TextureState::Em);
	createMaterial(L"WallConcreteFlat04", L"DeferredShader", L"HomeBuilding0017_1Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteFlat05", L"DeferredShader", L"HomeBuilding0017_2Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteFlat06", L"DeferredShader", L"HomeBuilding0017_3Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteFlat07", L"DeferredShader", L"HomeBuilding0017_4Material", TextureState::AlMtNrRg);
	createMaterial(L"WallConcreteFlat08", L"DeferredShader", L"HomeBuilding0017_5Material", TextureState::AlMtNrRg);
	createMaterial(L"WallConcreteTopflloor02", L"DeferredShader", L"HomeBuilding0017_6Material", TextureState::AlNrRg);
	createMaterial(L"WindowlGlassFlat00", L"DeferredShader", L"HomeBuilding0017_7Material", TextureState::AlNrRg);


#pragma endregion

#pragma region CityWorldHomeBuilding0018 Material
	createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding0018_0Material", TextureState::AlEm);
	createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding0018_1Material", TextureState::Em);
	createMaterial(L"EntranceLightSeal00", L"DeferredShader", L"HomeBuilding0018_2Material", TextureState::Em);
	createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding0018_3Material", TextureState::Em);
	createMaterial(L"WallConcreteFlat09", L"DeferredShader", L"HomeBuilding0018_4Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteFlat010", L"DeferredShader", L"HomeBuilding0018_5Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteFlat011", L"DeferredShader", L"HomeBuilding0018_6Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteFlat12", L"DeferredShader", L"HomeBuilding0018_7Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteFlat13", L"DeferredShader", L"HomeBuilding0018_8Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteTopflloor06", L"DeferredShader", L"HomeBuilding0018_9Material", TextureState::AlNrRg);
	createMaterial(L"WindowlGlassFlat01", L"DeferredShader", L"HomeBuilding0018_10Material", TextureState::AlNrRg);

#pragma endregion

#pragma region CityWorldHomeBuilding0019 Material
	createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding0019_0Material", TextureState::Em);
	createMaterial(L"WallConcreteFlat09", L"DeferredShader", L"HomeBuilding0019_1Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteFlat010", L"DeferredShader", L"HomeBuilding0019_2Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteFlat011", L"DeferredShader", L"HomeBuilding0019_3Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteFlat12", L"DeferredShader", L"HomeBuilding0019_4Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteTopflloor06", L"DeferredShader", L"HomeBuilding0019_5Material", TextureState::AlNrRg);
	createMaterial(L"WindowlGlassFlat01", L"DeferredShader", L"HomeBuilding0019_6Material", TextureState::AlNrRg);

#pragma endregion

#pragma region CityWorldHomeBuilding0020 Material
	createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding0020_0Material", TextureState::Em);
	createMaterial(L"GlassBuilding1F01", L"DeferredShader", L"HomeBuilding0020_1Material", TextureState::Em);
	createMaterial(L"WallConcreteDepart00", L"DeferredShader", L"HomeBuilding0020_2Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteDepart01", L"DeferredShader", L"HomeBuilding0020_3Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteDepart02", L"DeferredShader", L"HomeBuilding0020_4Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteTopflloor02", L"DeferredShader", L"HomeBuilding0020_5Material", TextureState::AlNrRg);
	createMaterial(L"WallTentDepart00", L"DeferredShader", L"HomeBuilding0020_6Material", TextureState::AlNrRg);
	createMaterial(L"WindowGlassConcrete00", L"DeferredShader", L"HomeBuilding0020_7Material", TextureState::AlMtMsNrRg);
	createMaterial(L"WindowGlassConcrete01", L"DeferredShader", L"HomeBuilding0020_8Material", TextureState::AlEmNrRg);
	createMaterial(L"WindowGlassConcrete02", L"DeferredShader", L"HomeBuilding0020_9Material", TextureState::AlNrRg);

#pragma endregion

#pragma region CityWorldHomeBuilding0021 Material
	createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding0021_0Material", TextureState::Em);
	createMaterial(L"GlassBuilding1F01", L"DeferredShader", L"HomeBuilding0021_1Material", TextureState::Em);
	createMaterial(L"WallConcreteDepart00", L"DeferredShader", L"HomeBuilding0021_2Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteDepart01", L"DeferredShader", L"HomeBuilding0021_3Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteDepart02", L"DeferredShader", L"HomeBuilding0021_4Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteTopflloor02", L"DeferredShader", L"HomeBuilding0021_5Material", TextureState::AlNrRg);
	createMaterial(L"WallTentDepart00", L"DeferredShader", L"HomeBuilding0021_6Material", TextureState::AlNrRg);
	createMaterial(L"WindowGlassConcrete00", L"DeferredShader", L"HomeBuilding0021_7Material", TextureState::AlMtMsNrRg);
	createMaterial(L"WindowGlassConcrete03", L"DeferredShader", L"HomeBuilding0021_8Material", TextureState::AlEmMtNrRg);
	createMaterial(L"WindowGlassConcrete02", L"DeferredShader", L"HomeBuilding0021_9Material", TextureState::AlNrRg);


#pragma endregion

#pragma region CityWorldHomeBuilding0022 Material
	createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding0022_0Material", TextureState::Em);
	createMaterial(L"GlassBuilding1F01", L"DeferredShader", L"HomeBuilding0022_1Material", TextureState::Em);
	createMaterial(L"WallConcreteDepart00", L"DeferredShader", L"HomeBuilding0022_2Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteDepart01", L"DeferredShader", L"HomeBuilding0022_3Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteDepart02", L"DeferredShader", L"HomeBuilding0022_4Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteTopflloor02", L"DeferredShader", L"HomeBuilding0022_5Material", TextureState::AlNrRg);
	createMaterial(L"WallTentDepart00", L"DeferredShader", L"HomeBuilding0022_6Material", TextureState::AlNrRg);
	createMaterial(L"WindowGlassConcrete00", L"DeferredShader", L"HomeBuilding0022_7Material", TextureState::AlMtMsNrRg);
	createMaterial(L"WindowGlassConcrete01", L"DeferredShader", L"HomeBuilding0022_8Material", TextureState::AlEmNrRg);
	createMaterial(L"WindowGlassConcrete02", L"DeferredShader", L"HomeBuilding0022_9Material", TextureState::AlNrRg);


#pragma endregion

#pragma region CityWorldHomeBuilding0023 Material
	createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding0023_0Material", TextureState::Em);
	createMaterial(L"WallConcreteDepart00", L"DeferredShader", L"HomeBuilding0023_1Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteDepart01", L"DeferredShader", L"HomeBuilding0023_2Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteDepart02", L"DeferredShader", L"HomeBuilding0023_3Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteTopflloor02", L"DeferredShader", L"HomeBuilding0023_4Material", TextureState::AlNrRg);
	createMaterial(L"WindowGlassConcrete00", L"DeferredShader", L"HomeBuilding0023_5Material", TextureState::AlMtMsNrRg);
	createMaterial(L"WindowGlassConcrete01", L"DeferredShader", L"HomeBuilding0023_6Material", TextureState::AlEmNrRg);

#pragma endregion

#pragma region CityWorldHomeBuilding0024 Material
	createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding0024_0Material", TextureState::Em);
	createMaterial(L"WallConcreteClaf00", L"DeferredShader", L"HomeBuilding0024_1Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteClaf01", L"DeferredShader", L"HomeBuilding0024_2Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteClaf02", L"DeferredShader", L"HomeBuilding0024_3Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteClaf03", L"DeferredShader", L"HomeBuilding0024_4Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteClaf04", L"DeferredShader", L"HomeBuilding0024_5Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteFlat09", L"DeferredShader", L"HomeBuilding0024_6Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteFlat12", L"DeferredShader", L"HomeBuilding0024_7Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteTopflloor07", L"DeferredShader", L"HomeBuilding0024_8Material", TextureState::AlNrRg);
	createMaterial(L"WindowlGlassClaf01", L"DeferredShader", L"HomeBuilding0024_9Material", TextureState::AlMtMsNrRg);
	createMaterial(L"WindowlGlassClaf02", L"DeferredShader", L"HomeBuilding0024_10Material", TextureState::AlEmNrRg);
	createMaterial(L"WindowlGlassFlat01", L"DeferredShader", L"HomeBuilding0024_11Material", TextureState::AlNrRg);

#pragma endregion

#pragma region CityWorldHomeBuilding0026 Material
	//DoorConcreteWallMain01 0
	createMaterial(L"ConcreteWallMain01", L"DeferredShader", L"HomeBuilding0026_0Material", TextureState::AlEmNrRg);
	createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding0026_1Material", TextureState::Em);
	createMaterial(L"HouseAttachmentSet00", L"DeferredShader", L"HomeBuilding0026_2Material", TextureState::AlEmMsMtNr);
	createMaterial(L"HouseAttachmentSet03", L"DeferredShader", L"HomeBuilding0026_3Material", TextureState::Al);
	createMaterial(L"HouseAttachmentTop00", L"DeferredShader", L"HomeBuilding0026_4Material", TextureState::AlNr);
	createMaterial(L"HouseBrickWall00", L"DeferredShader", L"HomeBuilding0026_5Material", TextureState::AlNrRg);
	createMaterial(L"HouseBrickWall01", L"DeferredShader", L"HomeBuilding0026_6Material", TextureState::Al);
	createMaterial(L"HouseBrickWall02", L"DeferredShader", L"HomeBuilding0026_7Material", TextureState::Al);
	createMaterial(L"HouseIronFence00", L"DeferredShader", L"HomeBuilding0026_8Material", TextureState::AlNrRg);
	createMaterial(L"HousePaintedIron00", L"DeferredShader", L"HomeBuilding0026_9Material", TextureState::AlNr);
	createMaterial(L"WallConcreteTopflloor06", L"DeferredShader", L"HomeBuilding0026_10Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteTopVer01", L"DeferredShader", L"HomeBuilding0026_11Material", TextureState::AlNr);

#pragma endregion

#pragma region CityWorldHomeBuilding Material
	createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding005_0Material", TextureState::Al);
	createMaterial(L"BillEntrance00", L"DeferredShader", L"HomeBuilding005_1Material", TextureState::Em);
	createMaterial(L"GlassBuilding1F00", L"DeferredShader", L"HomeBuilding005_2Material", TextureState::Em);
	createMaterial(L"HouseAttachmentSet00", L"DeferredShader", L"HomeBuilding005_3Material", TextureState::AlEmMsMtNr);
	createMaterial(L"HouseAttachmentSet03", L"DeferredShader", L"HomeBuilding005_4Material", TextureState::Al);
	createMaterial(L"HouseAttachmentTop00", L"DeferredShader", L"HomeBuilding005_5Material", TextureState::AlNr);
	createMaterial(L"HouseBrickWall00", L"DeferredShader", L"HomeBuilding005_6Material", TextureState::AlNrRg);
	createMaterial(L"HouseIronFence01", L"DeferredShader", L"HomeBuilding005_7Material", TextureState::Al);
	createMaterial(L"HousePaintedIron00", L"DeferredShader", L"HomeBuilding005_8Material", TextureState::AlNr);
	createMaterial(L"WallConcreteTopflloor06", L"DeferredShader", L"HomeBuilding005_9Material", TextureState::AlNrRg);
	createMaterial(L"WallConcreteTopVer01", L"DeferredShader", L"HomeBuilding005_10Material", TextureState::AlNr);

#pragma endregion

#pragma region HomeStageGroundCollider Material
	createMaterial(L"SideWalk01", L"DeferredShader", L"StageGroundCollider_0Material", TextureState::AlNrRg);
	createMaterial(L"ConcreteWall00", L"DeferredShader", L"StageGroundCollider_1Material", TextureState::AlNrRg);
	createMaterial(L"RoadAsphaltGlossy00", L"DeferredShader", L"StageGroundCollider_2Material", TextureState::AlNrRg);

#pragma endregion

#pragma region NaviCollider Material
	createMaterial(L"BlueseatStepRepeat00", L"DeferredShader", L"NaviCollider_0Material", TextureState::Nr); //AlNr
	createMaterial(L"MetalScaffoldRepeat00", L"DeferredShader", L"NaviCollider_1Material", TextureState::Nr); //AlNrRg
	createMaterial(L"MetalScaffold00", L"DeferredShader", L"NaviCollider_2Material", TextureState::Nr); //AlNrRg
 	createMaterial(L"SideWalk00", L"DeferredShader", L"NaviCollider_3Material", TextureState::Nr); //AlNrRg
	createMaterial(L"GroundCityLogo00", L"DeferredShader", L"NaviCollider_4Material", TextureState::Nr); //AlMtNrRg
	createMaterial(L"GroundEarth00", L"DeferredShader", L"NaviCollider_5Material", TextureState::Nr); //AlMtNrRg
	createMaterial(L"GroundMoon00", L"DeferredShader", L"NaviCollider_6Material", TextureState::Nr); //AlNrRg
	createMaterial(L"GroundCityLogo02", L"DeferredShader", L"NaviCollider_7Material", TextureState::Nr); //AlNrRg
	createMaterial(L"RoadAsphaltGlossy00", L"DeferredShader", L"NaviCollider_8Material", TextureState::Nr); //AlMtNrRg
	createMaterial(L"GroundTile00", L"DeferredShader", L"NaviCollider_9Material", TextureState::Nr); //AlNrRg
	createMaterial(L"GroundTile01", L"DeferredShader", L"NaviCollider_10Material", TextureState::Nr);//AlNrRg
	createMaterial(L"MetalManhole01", L"DeferredShader", L"NaviCollider_11Material", TextureState::Nr); //AlMtNrRg
	createMaterial(L"GroundLawn00", L"DeferredShader", L"NaviCollider_12Material", TextureState::Nr); //AlNrRg
	createMaterial(L"GroundLawn01", L"DeferredShader", L"NaviCollider_13Material", TextureState::Nr); //AlNrRg
	createMaterial(L"GroundSoil00", L"DeferredShader", L"NaviCollider_14Material", TextureState::Nr); //AlNrRg
	createMaterial(L"SideWalk01", L"DeferredShader", L"NaviCollider_15Material", TextureState::Nr); //AlNrRg
	createMaterial(L"SideWalk02", L"DeferredShader", L"NaviCollider_16Material", TextureState::Nr); //AlNr
	createMaterial(L"ConcreteWall00", L"DeferredShader", L"NaviCollider_17Material", TextureState::Nr); //AlNrRg
	createMaterial(L"MetalManhole03", L"DeferredShader", L"NaviCollider_18Material", TextureState::Nr); //AlNrRg

#pragma endregion

#pragma region CityGround Material
	{
		createMaterial(L"AsphaltSeal00", L"DeferredShader", L"AsphaltSeal00Material", TextureState::AlNrRg);
		createMaterial(L"BaseAsphaltRoad01", L"DeferredShader", L"BaseAsphaltRoad01Material", TextureState::AlNrRg);
		createMaterial(L"BlueseatStepRepeat00", L"DeferredShader", L"BlueseatStepRepeat00Material", TextureState::AlNr);
		createMaterial(L"CityLitter00", L"DeferredShader", L"CityLitter00Material", TextureState::AlNrRg);
		createMaterial(L"GrassGround00", L"DeferredShader", L"GrassGround00Material", TextureState::Al);
		createMaterial(L"CityLitter01", L"DeferredShader", L"CityLitter01Material", TextureState::AlNrRg);
		createMaterial(L"ConcreteWall00", L"DeferredShader", L"ConcreteWall00Material", TextureState::AlNrRg);
		createMaterial(L"DrainageCover00", L"DeferredShader", L"DrainageCover00Material", TextureState::AlNrRg);
		createMaterial(L"Font_BaseRoadSign00", L"DeferredShader", L"Font_BaseRoadSign00Material", TextureState::Al);
		createMaterial(L"Font_BaseRoadSign01", L"DeferredShader", L"Font_BaseRoadSign01Material", TextureState::Al);
		createMaterial(L"GrassFlowerSet00", L"DeferredShader", L"GrassFlowerSet00Material", TextureState::AlNrRg);
		createMaterial(L"GroundCityLogo00", L"DeferredShader", L"GroundCityLogo00Material", TextureState::AlMtNrRg);
		createMaterial(L"GroundCityLogo02", L"DeferredShader", L"GroundCityLogo02Material", TextureState::AlNrRg);
		createMaterial(L"GroundEarth01", L"DeferredShader", L"GroundEarth01Material", TextureState::AlMtNrRg);
		createMaterial(L"GroundLawn00", L"DeferredShader", L"GroundLawn00Material", TextureState::AlNrRg);
		createMaterial(L"GroundLawn01", L"DeferredShader", L"GroundLawn01Material", TextureState::AlNrRg);
		createMaterial(L"GroundLight00", L"DeferredShader", L"GroundLight00Material", TextureState::AlEmMsNrRg);
		createMaterial(L"GroundMoon01", L"DeferredShader", L"GroundMoon01Material", TextureState::AlMtNrRg);
		createMaterial(L"GroundSoil00", L"DeferredShader", L"GroundSoil00Material", TextureState::AlNrRg);
		createMaterial(L"GroundTile00", L"DeferredShader", L"GroundTile00Material", TextureState::AlNrRg);
		createMaterial(L"GroundTile01", L"DeferredShader", L"GroundTile01Material", TextureState::AlNrRg);
		createMaterial(L"MetalManhole01", L"DeferredShader", L"MetalManhole01Material", TextureState::AlMtNrRg);
		createMaterial(L"MetalManhole03", L"DeferredShader", L"MetalManhole03Material", TextureState::AlMtNrRg);
		createMaterial(L"MetalScaffold00", L"DeferredShader", L"MetalScaffold00Material", TextureState::AlNrRg);
		createMaterial(L"MetalScaffoldRepeat00", L"DeferredShader", L"MetalScaffoldRepeat00Material", TextureState::AlNrRg);
		createMaterial(L"RoadAsphaltGlossy00", L"DeferredShader", L"RoadAsphaltGlossy00Material", TextureState::AlNrRg);
		createMaterial(L"RoadManhole00", L"DeferredShader", L"RoadManhole00Material", TextureState::AlNrRg);
		createMaterial(L"SideWalk00", L"DeferredShader", L"SideWalk00Material", TextureState::AlNrRg);
		createMaterial(L"SideWalk01", L"DeferredShader", L"SideWalk01Material", TextureState::AlNrRg);
		createMaterial(L"SideWalk02", L"DeferredShader", L"SideWalk02Material", TextureState::AlNr);
		createMaterial(L"WallFastener00", L"DeferredShader", L"WallFastener00Material", TextureState::AlNrRg);
		createMaterial(L"WallGrunge00", L"DeferredShader", L"WallGrunge00Material", TextureState::Al);
		createMaterial(L"WallGrunge01", L"DeferredShader", L"WallGrunge01Material", TextureState::Al);

	}

#pragma endregion

#pragma region CityWorldHomeBench000 Material
	createMaterial(L"BenchiWood00", L"DeferredShader", L"BenchiWood00Material", TextureState::AlMtNrRg);

#pragma endregion

#pragma region CityWorldBush Material
	createMaterial(L"CityBushLeaf", L"DeferredShader", L"CityWorldBush_0Material", TextureState::AlNrRg);
	createMaterial(L"CityBushBody", L"DeferredShader", L"CityWorldBush_1Material", TextureState::Al);

#pragma endregion

#pragma region CityWorldBushA Material
	createMaterial(L"LeafTree00", L"DeferredShader", L"CityWorldBushA_0Material", TextureState::AlNrRg);
	createMaterial(L"LeafTree01", L"DeferredShader", L"CityWorldBushA_1Material", TextureState::AlNrRg);

#pragma endregion

#pragma region CityWorldChairA Material
	createMaterial(L"PaintChair00", L"DeferredShader", L"CityWorldChairA_0Material", TextureState::AlNrRg);

#pragma endregion

#pragma region CityWorldChairB Material
	createMaterial(L"CityWorldChairB00", L"DeferredShader", L"CityWorldBushB_0Material", TextureState::AlNrRg);

#pragma endregion

#pragma region CheckpointFlag Material
	createMaterial(L"CheckpointFlagBody", L"DeferredShader", L"CheckpointFlag_0Material", TextureState::AlMtMsNrRg);
	createMaterial(L"CheckpointFlagMark0", L"DeferredShader", L"CheckpointFlag_1Material", TextureState::AlNrRg);
	createMaterial(L"CheckpointFlagMark1", L"DeferredShader", L"CheckpointFlag_2Material", TextureState::AlNrRg);
	createMaterial(L"CheckpointFlagScroll", L"DeferredShader", L"CheckpointFlag_3Material", TextureState::AlNr);

#pragma endregion

#pragma region CItyWorldFlag Material
	createMaterial(L"ClothFlagCity00", L"DeferredShader", L"CItyWorldFlag_0Material", TextureState::AlNr);
	createMaterial(L"MetalFlagPole00", L"DeferredShader", L"CItyWorldFlag_1Material", TextureState::AlNr);

#pragma endregion

#pragma region Coin Material
	createMaterial(L"coinbody00", L"DeferredShader", L"CoinObjectMaterial", TextureState::AlNrRg);

#pragma endregion

#pragma region BlockEmpty Material
	createMaterial(L"BlockEmptyBody", L"DeferredShader", L"BlockEmptyMaterial", TextureState::AlEmMtNrRg);

#pragma endregion

#pragma region BlockQuestion Material
	createMaterial(L"BlockQuestionBody", L"DeferredShader", L"BlockQuestionMaterial", TextureState::AlEmMtNrRg);

#pragma endregion

#pragma region BlockQuestion Material
	createMaterial(L"BlockQuestionBody", L"DeferredShader", L"BlockTransparentMaterial", TextureState::AlNr);

#pragma endregion

#pragma region BlockBrick Material
	createMaterial(L"BlockBrickBody", L"DeferredInstancedShader", L"BlockBrickMaterial", TextureState::AlEmMtNrRg);
	//createMaterial(L"BlockBrickBody", L"DeferredShader", L"BlockBrickMaterial", TextureState::AlEmMtNrRg);

#pragma endregion

#pragma region BlockBrickBreak Material
	createMaterial(L"BlockBrickBody", L"DeferredShader", L"BlockBrickBreakMaterial", TextureState::AlEmMtNrRg);

#pragma endregion

#pragma region CityWorldBarrel Material
	createMaterial(L"CityWorldBarrelBody", L"DeferredShader", L"CityWorldBarrelMaterial", TextureState::AlMtNrRg);

#pragma endregion

#pragma region Motorcycle Material
	createMaterial(L"motorcyclebody", L"DeferredShader", L"MotorcycleMaterial", TextureState::AlMtNrRg);

#pragma endregion

#pragma region TaxiCar Material
	createMaterial(L"carbody", L"DeferredShader", L"TaxiCar_0Material", TextureState::AlEmMtNrRg);
	createMaterial(L"carwindow", L"DeferredShader", L"TaxiCar_1Material", TextureState::AlNrRg);

#pragma endregion

#pragma region CarBreakable Material
	createMaterial(L"carbodyBreakable", L"DeferredShader", L"CarBreakableMaterial", TextureState::AlEmMtNrRg);

#pragma endregion

#pragma region CityWorldHomeFence000 Material
	createMaterial(L"MetalFence01", L"DeferredShader", L"Fence000_0Material", TextureState::AlNrRg);
	createMaterial(L"MetalFence00", L"DeferredShader", L"Fence000_1Material", TextureState::AlMtNrRg);
	createMaterial(L"ParkSign00", L"DeferredShader", L"Fence000_2Material", TextureState::AlNrRg);

#pragma endregion

#pragma region CityWorldHomeFence001 Material
	createMaterial(L"MetalFence02", L"DeferredShader", L"Fence001_0Material", TextureState::AlNrRg);
	//createMaterial(L"MetalFence02", L"DeferredShader", L"Fence001_0Material", TextureState::AlNrRg); //같은 마테리얼 2개

#pragma endregion

#pragma region CityWorldHomeFence002 Material
	createMaterial(L"MetalFence02", L"DeferredShader", L"Fence002_0Material", TextureState::AlNrRg);
	//createMaterial(L"MetalFence02", L"DeferredShader", L"Fence001_0Material", TextureState::AlNrRg); //같은 마테리얼 2개
	createMaterial(L"MetalFence03", L"DeferredShader", L"Fence002_1Material", TextureState::AlNr);

#pragma endregion

#pragma region CityWorldHomeFence003 Material
	createMaterial(L"MetalFence00", L"DeferredShader", L"Fence003_0Material", TextureState::AlMtNrRg);

#pragma endregion

#pragma region CityWorldHomeFence005 Material
	createMaterial(L"MetalFence00", L"DeferredShader", L"Fence005_0Material", TextureState::AlMtNrRg);

#pragma endregion

#pragma region CityWorldHomeFence006 Material
	createMaterial(L"MetalFence00", L"DeferredShader", L"Fence006_0Material", TextureState::AlMtNrRg);

#pragma endregion

#pragma region CityWorldHomeManhole000 Material
	createMaterial(L"MetalManhole01", L"DeferredShader", L"Manhole000_0Material", TextureState::AlMtNrRg);

#pragma endregion

#pragma region CityWorldHomeManhole001 Material
	createMaterial(L"MetalManhole01", L"DeferredShader", L"Manhole001_0Material", TextureState::AlMtNrRg);

#pragma endregion

#pragma region CityWorldHomeManhole002 Material
	createMaterial(L"MetalManhole02", L"DeferredShader", L"Manhole002_0Material", TextureState::AlMtNrRg);

#pragma endregion

#pragma region CityWorldHomeManhole003 Material
	createMaterial(L"MetalManhole03", L"DeferredShader", L"Manhole003_0Material", TextureState::AlMtNrRg);

#pragma endregion

#pragma region CityWorldHomeSign000 Material
	createMaterial(L"Font_RoadSign", L"DeferredShader", L"Sign000_0Material", TextureState::AlNrRg);
	createMaterial(L"LightShadow01", L"DeferredShader", L"Sign000_1Material", TextureState::Al);

#pragma endregion


#pragma region CityWorldHomeSign001 Material
	createMaterial(L"Font_RoadSign", L"DeferredShader", L"Sign001_0Material", TextureState::AlNrRg);
	createMaterial(L"LightShadow01", L"DeferredShader", L"Sign001_1Material", TextureState::Al);

#pragma endregion

#pragma region CityWorldHomeSign002 Material
	createMaterial(L"MetalSignLong00", L"DeferredShader", L"Sign002_0Material", TextureState::AlEmMtNrRg);

#pragma endregion

#pragma region CityWorldHomeSign003 Material
	createMaterial(L"MetalSignLong00", L"DeferredShader", L"Sign003_0Material", TextureState::AlEmMtNrRg);

#pragma endregion

#pragma region CityWorldHomeSign004 Material
	createMaterial(L"MetalSignBoradSmallC00", L"DeferredShader", L"Sign004_0Material", TextureState::AlEmMtNrRg); //2번

#pragma endregion

#pragma region CityWorldHomeSign007 Material
	createMaterial(L"MetalFrameSignBoard00", L"DeferredShader", L"Sign007_0Material", TextureState::AlNrRg);
	createMaterial(L"FrameLight00", L"DeferredShader", L"Sign007_1Material", TextureState::AlEmNr);
	createMaterial(L"MetalFrameSignboard02", L"DeferredShader", L"Sign007_2Material", TextureState::AlNr);
	createMaterial(L"SignBoardTopLight00", L"DeferredShader", L"Sign007_3Material", TextureState::Al);
	//MetalFrameSignBoard00 4번째 슬롯
	createMaterial(L"PosterWedding06", L"DeferredShader", L"Sign007_4Material", TextureState::AlNr);

#pragma endregion

#pragma region CityWorldHomeSign008 Material
	createMaterial(L"MetalFrameSignBoard00", L"DeferredShader", L"Sign008_0Material", TextureState::AlNrRg);
	createMaterial(L"FrameLight00", L"DeferredShader", L"Sign008_1Material", TextureState::AlEmNr);
	createMaterial(L"MetalFrameSignboard02", L"DeferredShader", L"Sign008_2Material", TextureState::AlNr);
	createMaterial(L"SignBoardTopLight00", L"DeferredShader", L"Sign008_3Material", TextureState::Al);
	//MetalFrameSignBoard00 4번째 슬롯
	createMaterial(L"PosterWedding02", L"DeferredShader", L"Sign008_4Material", TextureState::Al);

#pragma endregion

#pragma region CityWorldHomeSign009 Material
	createMaterial(L"MetalFrameSignboard02", L"DeferredShader", L"Sign009_0Material", TextureState::AlNr);
	createMaterial(L"SignBoardTopLight00", L"DeferredShader", L"Sign009_1Material", TextureState::Al);
	createMaterial(L"MetalFrameSignBoard00", L"DeferredShader", L"Sign009_2Material", TextureState::AlNrRg);
	createMaterial(L"FrameLight01", L"DeferredShader", L"Sign009_3Material", TextureState::AlEmNr); //.scr
	createMaterial(L"PosterWedding02", L"DeferredShader", L"Sign009_4Material", TextureState::Al);
	//MetalFrameSignBoard00 5번째 슬롯

#pragma endregion

#pragma region CityWorldHomeSign0010 Material
//없는 텍스쳐 파일?? 또는 중복되지만 이름이 다른 텍스쳐가 많으므로 확인 필요
	createMaterial(L"MetalFrameSignBoard00", L"DeferredShader", L"Sign0010_0Material", TextureState::AlNrRg);
	createMaterial(L"FrameLightFront00", L"DeferredShader", L"Sign0010_1Material", TextureState::Em);
	//FrameLightFront01
	createMaterial(L"MetalFrameSignboard02", L"DeferredShader", L"Sign0010_2Material", TextureState::AlNr);
	//createMaterial(L"MetalFrameSignBoard00", L"DeferredShader", L"Sign0010_3Material", TextureState::AlNrRg);
	//createMaterial(L"MetalFrameSignBoard00", L"DeferredShader", L"Sign0010_4Material", TextureState::AlNrRg);
	createMaterial(L"FrameLight01", L"DeferredShader", L"Sign0010_5Material", TextureState::AlEmNr); //.scr
	createMaterial(L"MetalFrameSignboard02", L"DeferredShader", L"Sign0010_6Material", TextureState::AlNr);
	createMaterial(L"PosterWedding01", L"DeferredShader", L"Sign0010_7Material", TextureState::AlNr);
	createMaterial(L"PosterWedding02", L"DeferredShader", L"Sign0010_8Material", TextureState::Al);
	createMaterial(L"MetalWallShop00", L"DeferredShader", L"Sign0010_9Material", TextureState::Al);
	createMaterial(L"FrameLight00", L"DeferredShader", L"Sign0010_10Material", TextureState::AlEmNr);
	//createMaterial(L"FrameLight01", L"DeferredShader", L"Sign0010_11Material", TextureState::AlEmNr); //FrameLight03 찾아야됨
	//createMaterial(L"FrameLightFront00", L"DeferredShader", L"Sign0010_12Material", TextureState::Em); /FrameLightFront01
	createMaterial(L"GlassWallShop00", L"DeferredShader", L"Sign0010_13Material", TextureState::AlEmNr);
	//createMaterial(L"GlassWallShop00", L"DeferredShader", L"Sign0010_14Material", TextureState::AlEmNr); //GlassWallShop01
	createMaterial(L"ShopEntrance00", L"DeferredShader", L"Sign0010_15Material", TextureState::Em);
	createMaterial(L"SignBoardTopLight00", L"DeferredShader", L"Sign0010_16Material", TextureState::Al);
	//createMaterial(L"FrameLight01", L"DeferredShader", L"Sign0010_17Material", TextureState::AlEmNr); //FrameLight08 찾아야됨
	createMaterial(L"FrameLightLogo00", L"DeferredShader", L"Sign0010_18Material", TextureState::Al);
	createMaterial(L"CityPosterLight00", L"DeferredShader", L"Sign0010_19Material", TextureState::Al); // FrameLight02??
	createMaterial(L"FrameLight04", L"DeferredShader", L"Sign0010_20Material", TextureState::AlNr); // FrameLight02??
	//createMaterial(L"FrameLight01", L"DeferredShader", L"Sign0010_21Material", TextureState::AlNr); // FrameLight05_scr
	createMaterial(L"ShopSign00", L"DeferredShader", L"Sign0010_22Material", TextureState::Al);
	createMaterial(L"CityShoprelf00", L"DeferredShader", L"Sign0010_22Material", TextureState::AlNrRg);
	createMaterial(L"ShopMat00", L"DeferredShader", L"Sign0010_23Material", TextureState::AlNrRg);
	//createMaterial(L"FrameLight04", L"DeferredShader", L"Sign0010_24Material", TextureState::AlNr); // FrameLight08 필요
	createMaterial(L"MetalFrameSignBoard04", L"DeferredShader", L"Sign0010_25Material", TextureState::Al);
	createMaterial(L"PosterWedding00", L"DeferredShader", L"Sign0010_26Material", TextureState::AlNr);

#pragma endregion

#pragma region CityWorldHomeSign012 Material
	createMaterial(L"MetalFrameSignBoard00", L"DeferredShader", L"Sign012_0Material", TextureState::AlNrRg);
	createMaterial(L"SignBoardTopLight00", L"DeferredShader", L"Sign012_1Material", TextureState::Al);
	createMaterial(L"MetalFrameSignboard02", L"DeferredShader", L"Sign012_2Material", TextureState::AlNr);
	createMaterial(L"MetalFrameSignBoard00", L"DeferredShader", L"Sign012_3Material", TextureState::AlNrRg);
	//createMaterial(L"MetalFrameSignBoard00", L"DeferredShader", L"Sign012_4Material", TextureState::AlNrRg);
	createMaterial(L"FrameLight00", L"DeferredShader", L"Sign012_5Material", TextureState::AlEmNr);
	createMaterial(L"PosterWedding01", L"DeferredShader", L"Sign012_6Material", TextureState::AlNr);

#pragma endregion

#pragma region CityWorldHomeSign013 Material
	//CityWorldHomeSign0010처럼 없는 텍스쳐 또는 중복되어 이름이 다른 텍스쳐가 많음

#pragma endregion

#pragma region CityWorldHomeSign016 Material
	createMaterial(L"FontSignBoadA00", L"DeferredShader", L"Sign016_0Material", TextureState::AlEmNrRg);
	createMaterial(L"MetalFrameSignBoard00", L"DeferredShader", L"Sign016_1Material", TextureState::AlNrRg);
	//createMaterial(L"FontSignBoadA00", L"DeferredShader", L"Sign016_2Material", TextureState::AlEmNrRg);
	createMaterial(L"FrameLight07", L"DeferredShader", L"Sign016_3Material", TextureState::Al);

	// FrameLight00 -> 07작성되어있음 임시로 07로 변경

#pragma endregion

#pragma region CityWorldHomeSign017 Material
	createMaterial(L"SignBoardTopLight00", L"DeferredShader", L"Sign017_0Material", TextureState::Al);
	createMaterial(L"MetalFrameSignBoard00", L"DeferredShader", L"Sign017_1Material", TextureState::AlNrRg);
	createMaterial(L"MetalFrameSignboard02", L"DeferredShader", L"Sign017_2Material", TextureState::AlNr);
	//createMaterial(L"MetalFrameSignBoard00", L"DeferredShader", L"Sign017_3Material", TextureState::AlNrRg);
	//createMaterial(L"MetalFrameSignBoard00", L"DeferredShader", L"Sign017_4Material", TextureState::AlNrRg);
	createMaterial(L"FrameLight00", L"DeferredShader", L"Sign017_5Material", TextureState::AlEmNr);
	createMaterial(L"PosterSign00", L"DeferredShader", L"Sign017_5Material", TextureState::AlNr);

#pragma endregion

#pragma region CityWorldHomeSign020 Material
	createMaterial(L"MetalFrameSignBoard00", L"DeferredShader", L"Sign020_0Material", TextureState::AlNrRg);
	createMaterial(L"FrameLight00", L"DeferredShader", L"Sign020_1Material", TextureState::AlEmNr);
	createMaterial(L"MetalFrameSignboard02", L"DeferredShader", L"Sign020_2Material", TextureState::AlNr);
	createMaterial(L"SignBoardTopLight00", L"DeferredShader", L"Sign020_3Material", TextureState::Al);
	createMaterial(L"MetalFrameSignBoard00", L"DeferredShader", L"Sign020_4Material", TextureState::AlNrRg);
	createMaterial(L"PosterSign00", L"DeferredShader", L"Sign020_5Material", TextureState::AlNr);
	createMaterial(L"PosterSign02", L"DeferredShader", L"Sign020_6Material", TextureState::AlNr);

#pragma endregion

#pragma region CityWorldHomeSign022 Material
	createMaterial(L"MetalFrameSignBoard00", L"DeferredShader", L"Sign022_0Material", TextureState::AlNrRg);
	createMaterial(L"FrameLight00", L"DeferredShader", L"Sign022_1Material", TextureState::AlEmNr);
	createMaterial(L"MetalFrameSignboard02", L"DeferredShader", L"Sign022_2Material", TextureState::AlNr);
	createMaterial(L"SignBoardTopLight00", L"DeferredShader", L"Sign022_3Material", TextureState::Al);
	//createMaterial(L"MetalFrameSignBoard00", L"DeferredShader", L"Sign020_4Material", TextureState::AlNrRg);
	createMaterial(L"PosterSign02", L"DeferredShader", L"Sign022_4Material", TextureState::AlNr);

#pragma endregion

#pragma region CityWorldHomeSign027 Material
	createMaterial(L"MetalSignBoradSmallB00", L"DeferredShader", L"Sign027_0Material", TextureState::AlEmMtNrRg);
	//createMaterial(L"MetalSignBoradSmallB00", L"DeferredShader", L"Sign027_1Material", TextureState::AlEmMtNrRg);

#pragma endregion

#pragma region CityWorldFenceA Material
	createMaterial(L"BarricadePlasticWhite00", L"DeferredShader", L"FenceA_0Material", TextureState::AlNrRg);

#pragma endregion

#pragma region CityWorldFenceA Material
	createMaterial(L"BarricadePlasticWhite00", L"DeferredShader", L"FenceB_0Material", TextureState::AlNrRg);

#pragma endregion


#pragma region CityWorldHomeDoor000 Material
	createMaterial(L"HouseAttachmentSet00", L"DeferredShader", L"Door000_0Material", TextureState::AlMtNrRg);
	createMaterial(L"EntranceLightSeal00", L"DeferredShader", L"Door000_1Material", TextureState::Em);

#pragma endregion

#pragma region CityWorldHomeDoor001 Material
	createMaterial(L"DoorGlassSteel00", L"DeferredShader", L"Door001_0Material", TextureState::AlEmNr);

#pragma endregion

#pragma region CityWorldHomeDoor002 Material
	createMaterial(L"WindowlGlassFlat01", L"DeferredShader", L"Door002_0Material", TextureState::AlEmMsNrRg);

#pragma endregion

#pragma region CityWorldHomeDoor003 Material
	createMaterial(L"DoorWoodBill00", L"DeferredShader", L"Door003_0Material", TextureState::AlNr);
	createMaterial(L"EntranceLightSeal00", L"DeferredShader", L"Door003_1Material", TextureState::Em);

#pragma endregion

#pragma region CityWorldHomeDoor004 Material
	createMaterial(L"EntranceLightSeal00", L"DeferredShader", L"Door004_0Material", TextureState::Em);
	createMaterial(L"WindowlGlassClaf01", L"DeferredShader", L"Door004_1Material", TextureState::AlMtMsNrRg);

#pragma endregion

#pragma region CityWorldHomeDoor005 Material
	createMaterial(L"DoorGlassSteel00", L"DeferredShader", L"Door005_0Material", TextureState::AlNr);

#pragma endregion

#pragma region CityWorldHomeDoor006 Material
	createMaterial(L"EntranceLightSeal00", L"DeferredShader", L"Door006_0Material", TextureState::Em);
	createMaterial(L"MetalWallMain00", L"DeferredShader", L"Door006_1Material", TextureState::AlNrRg);
	createMaterial(L"GlassWindowMain00", L"DeferredShader", L"Door006_2Material", TextureState::AlEmMsNrRg);

#pragma endregion

#pragma region CityWorldHomeDoor007 Material
	createMaterial(L"MetalWallShop00", L"DeferredShader", L"Door007_0Material", TextureState::AlNrRg);
	createMaterial(L"GlassWallShop00", L"DeferredShader", L"Door007_1Material", TextureState::AlEmNr);

#pragma endregion

#pragma region CityWorldHomeDoor008 Material
	createMaterial(L"EntranceLightSeal00", L"DeferredShader", L"Door008_0Material", TextureState::Em);
	createMaterial(L"MetalWallMain00", L"DeferredShader", L"Door008_1Material", TextureState::AlNrRg);
	createMaterial(L"GlassWindowMain00", L"DeferredShader", L"Door008_2Material", TextureState::AlEmMsNrRg);

#pragma endregion
}

void AsyncLoad::loadCityTexture()
{
#pragma region CityWorldHomeBuilding002
	textureLoad(L"GlassBuildingWall00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlEmMtNrRg);
	textureLoad(L"RoofConcrete00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"RoofConcrete01", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WallGlassPaintedSteel00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNr);

#pragma endregion

#pragma region CityWorldHomeBuilding003
	textureLoad(L"GroundLawn00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"MetalFence00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlMtNrRg);

#pragma endregion

#pragma region CityWorldHomeBuilding
	//DoorConcreteWallMain01 텍스쳐 찾아야됨


	textureLoad(L"GlassBuilding1F01", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::Em);
	textureLoad(L"ConcreteWallMain00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"ConcreteWallMain01", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlEmNrRg);
	textureLoad(L"ConcreteWallMain02", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"ConcreteWallMain03", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"ConcreteWallMain04", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"ConcreteWallMain05", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"ConcreteWallMain06", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"GlassWindowMain00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlEmMsNrRg);
	textureLoad(L"MainTowerFont00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"MetalfloorMain00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"MetalWallMain00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"MetalWallMain01", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlEmNrRg);
	textureLoad(L"MetalWallMain02", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"MetalWallMain03", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlMtNrRg);
	textureLoad(L"TopFloorMain00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WindowGlassEmn00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlEmMsNrRg);
	textureLoad(L"WindowGlassEmn01", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::Em);



	textureLoad(L"BillEntrance00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlEm);
	textureLoad(L"GlassBuilding1F00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::Em);
	textureLoad(L"HouseAttachmentSet00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlEmMsMtNr);
	textureLoad(L"HouseAttachmentSet03", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::Al);
	textureLoad(L"HouseAttachmentTop00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNr);
	textureLoad(L"HouseBrickWall00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"HouseIronFence01", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::Al);
	textureLoad(L"HousePaintedIron00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNr);
	textureLoad(L"WallConcreteTopflloor02", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNr);
	textureLoad(L"WallConcreteTopVer00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNr);

	textureLoad(L"BuildingBrickWall02_rep", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::Al);
	textureLoad(L"BuildingStoneParts01", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::Al);
	textureLoad(L"BuildingStoneWall00_rep", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"BuildingStoneWall02_rep", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::Al);
	textureLoad(L"BuildingWindowSet00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlEmNrRg);
	textureLoad(L"BuildingWindowSet01", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlEmMtNrRg);
	textureLoad(L"DoorWoodBill00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNr);
	textureLoad(L"WallConcreteFlat01", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNr);
	textureLoad(L"WallConcreteFlat02", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNr);
	textureLoad(L"WindowGlassConcrete02", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);

	textureLoad(L"WallConcreteTopflloor06", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);




	textureLoad(L"BuildingBrickWall00_rep", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"BuildingStoneParts00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"BuildingStoneWall01_rep", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::Al);
	textureLoad(L"BuildingWindowSet02", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::Al);

	textureLoad(L"DoorGlassSteel00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNr);

	textureLoad(L"HouseBrickWall02", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::Al);
	textureLoad(L"HouseIronFence00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);

	textureLoad(L"HouseBrickWall01", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::Al);
	textureLoad(L"HouseIronFence02", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::Al);

	textureLoad(L"HouseAttachmentTop01", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::Al);

	textureLoad(L"EntranceStreetLight00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WallConcreteClaf00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WallConcreteClaf01", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WallConcreteClaf02", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WallConcreteClaf03", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WallConcreteClaf04", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlMtNrRg);
	textureLoad(L"WallConcreteClaf05", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WallConcreteTopflloor07", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WindowlGlassClaf01", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlMtMsNrRg);
	textureLoad(L"WindowlGlassClaf02", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlEmNrRg);

	textureLoad(L"WallConcreteFlat04", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WallConcreteFlat05", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WallConcreteFlat06", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WallConcreteFlat07", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlMtNrRg);
	textureLoad(L"WallConcreteFlat08", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlMtNrRg);
	textureLoad(L"WallConcreteTopflloor02", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WindowlGlassFlat00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlEmMtNrRg);

	textureLoad(L"EntranceLightSeal00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::Em);
	textureLoad(L"WallConcreteFlat09", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WallConcreteFlat010", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WallConcreteFlat11", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WallConcreteFlat12", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WallConcreteFlat13", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WindowlGlassFlat01", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlEmMsNrRg);

	textureLoad(L"WallConcreteDepart00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WallConcreteDepart01", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WallConcreteDepart02", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WallTentDepart00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlNrRg);
	textureLoad(L"WindowGlassConcrete00", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlMtMsNrRg);
	textureLoad(L"WindowGlassConcrete01", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlEmNrRg);

	textureLoad(L"WindowGlassConcrete03", L"CityWorldObject/CityWorldHomeBuilding/Image", TextureState::AlEmMtNrRg);


#pragma endregion

#pragma region NaviCollider
	//NaviCollider
	textureLoad(L"GroundEarth00", L"CityWorldObject/NaviCollider/Image", TextureState::AlMtNrRg);
	textureLoad(L"GroundMoon00", L"CityWorldObject/NaviCollider/Image", TextureState::AlNrRg);

#pragma endregion

#pragma region CityGround
	//CityGround
	textureLoad(L"AsphaltSeal00", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"BaseAsphaltRoad01", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"BlueseatStepRepeat00", L"CityWorldObject/CityGround/Image", TextureState::AlNr);
	textureLoad(L"CityLitter00", L"CityWorldObject/CityGround/Image", TextureState::AlMtNrRg);
	textureLoad(L"GrassGround00", L"CityWorldObject/CityGround/Image", TextureState::Al);
	textureLoad(L"CityLitter01", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"ConcreteWall00", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"DrainageCover00", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"Font_BaseRoadSign00", L"CityWorldObject/CityGround/Image", TextureState::Al);
	textureLoad(L"Font_BaseRoadSign01", L"CityWorldObject/CityGround/Image", TextureState::Al);
	textureLoad(L"GrassFlowerSet00", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"GroundCityLogo00", L"CityWorldObject/CityGround/Image", TextureState::AlMtNrRg);
	textureLoad(L"GroundCityLogo02", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"GroundEarth01", L"CityWorldObject/CityGround/Image", TextureState::AlMtNrRg);
	textureLoad(L"GroundLawn00", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"GroundLawn01", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"GroundLight00", L"CityWorldObject/CityGround/Image", TextureState::AlEmMsNrRg);
	textureLoad(L"GroundMoon01", L"CityWorldObject/CityGround/Image", TextureState::AlMtNrRg);
	textureLoad(L"GroundSoil00", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"GroundTile00", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"GroundTile01", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"MetalManhole01", L"CityWorldObject/CityGround/Image", TextureState::AlMtNrRg);
	textureLoad(L"MetalManhole03", L"CityWorldObject/CityGround/Image", TextureState::AlMtNrRg);
	textureLoad(L"MetalScaffold00", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"MetalScaffoldRepeat00", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"RoadAsphaltGlossy00", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"RoadManhole00", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"SideWalk00", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"SideWalk01", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"SideWalk02", L"CityWorldObject/CityGround/Image", TextureState::AlNr);
	textureLoad(L"WallFastener00", L"CityWorldObject/CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"WallGrunge00", L"CityWorldObject/CityGround/Image", TextureState::Al);
	textureLoad(L"WallGrunge01", L"CityWorldObject/CityGround/Image", TextureState::Al);

#pragma endregion

#pragma region CityWorldHomeBench000
	//CityWorldHomeBench000
	textureLoad(L"BenchiWood00", L"CityWorldObject/CityWorldHomeBench000/Image", TextureState::AlMtNrRg);

#pragma endregion
	
#pragma region CityWorldBush
		textureLoad(L"CityBushLeaf", L"CityWorldObject/CityWorldBush/Image", TextureState::AlNrRg);
	    textureLoad(L"CityBushBody", L"CityWorldObject/CityWorldBush/Image", TextureState::Al);

#pragma endregion

#pragma region CityWorldBushA
		textureLoad(L"LeafTree00", L"CityWorldObject/CityWorldBushA/Image", TextureState::AlNrRg);
		textureLoad(L"LeafTree01", L"CityWorldObject/CityWorldBushA/Image", TextureState::AlNrRg);

#pragma endregion

#pragma region CityWorldChairA
		textureLoad(L"PaintChair00", L"CityWorldObject/CityWorldChairA/Image", TextureState::AlNrRg);

#pragma endregion

#pragma region CityWorldBushB
		textureLoad(L"CityWorldChairB00", L"CityWorldObject/CityWorldChairB/Image", TextureState::AlNrRg);

#pragma endregion

#pragma region CheckpointFlag
		textureLoad(L"CheckpointFlagBody", L"CheckpointFlag/Image", TextureState::AlMtMsNrRg);
		textureLoad(L"CheckpointFlagMark0", L"CheckpointFlag/Image", TextureState::AlNrRg);
		textureLoad(L"CheckpointFlagMark1", L"CheckpointFlag/Image", TextureState::AlNrRg);
		//textureLoad(L"CheckpointFlagMark2_mask", L"CheckpointFlag/Image", TextureState::AlNrRg);
		textureLoad(L"CheckpointFlagScroll", L"CheckpointFlag/Image", TextureState::AlNr);

#pragma endregion

#pragma region CItyWorldFlag
		textureLoad(L"ClothFlagCity00", L"CityWorldObject/CItyWorldFlag/Image", TextureState::AlNr);
		textureLoad(L"MetalFlagPole00", L"CityWorldObject/CItyWorldFlag/Image", TextureState::AlNr);

#pragma endregion

#pragma region Coin
		textureLoad(L"coinbody00", L"Coin/Image", TextureState::AlNrRg);

#pragma endregion

#pragma region BlockEmpty
		textureLoad(L"BlockEmptyBody", L"Block/BlockEmpty/Image", TextureState::AlEmMtNrRg);

#pragma endregion

#pragma region BlockQuestion
		textureLoad(L"BlockQuestionBody", L"Block/BlockQuestion/Image", TextureState::AlEmMtNrRg);

#pragma endregion

#pragma region BlockTransparent
		//textureLoad(L"BlockQuestionBody", L"Block/BlockTransparent/Image", TextureState::AlNr);

#pragma endregion

#pragma region BlockBrick
		textureLoad(L"BlockBrickBody", L"Block/BlockBrick/Image", TextureState::AlEmMtNrRg);

#pragma endregion

#pragma region BlockBrickBreak
		//textureLoad(L"BlockBrickBody", L"Block/BlockBrickBreak/Image", TextureState::AlEmMtNrRg);

#pragma endregion

#pragma region BlockBrickBreak
		textureLoad(L"CityWorldBarrelBody", L"CityWorldBarrel/Image", TextureState::AlMtNrRg);

#pragma endregion

#pragma region Motorcycle
		textureLoad(L"motorcyclebody", L"CityWorldObject/Motorcycle/Image", TextureState::AlMtNrRg);

#pragma endregion

#pragma region TaxiCar
		textureLoad(L"carbody", L"CityWorldObject/TaxiCar/Car/Image", TextureState::AlEmMtNrRg);
		textureLoad(L"carwindow", L"CityWorldObject/TaxiCar/Car/Image", TextureState::AlNrRg);

#pragma endregion

#pragma region CarBreakable
		textureLoad(L"carbodyBreakable", L"CityWorldObject/TaxiCar/CarBreakable/Image", TextureState::AlEmMtNrRg);

#pragma endregion


#pragma region MotorcycleParkingLot
		textureLoad(L"MetalScaffold00", L"CityWorldObject/MotorcycleParkingLot/Image", TextureState::AlNrRg);
		textureLoad(L"MetalScaffoldRepeat00", L"CityWorldObject/MotorcycleParkingLot/Image", TextureState::AlNr);

#pragma endregion

		#pragma region SouvenirCity1
		textureLoad(L"souvenircity1body", L"CityWorldObject/SouvenirCity1/Image", TextureState::AlMtNrRg);

#pragma endregion


#pragma region CityWorldHomeFence
		textureLoad(L"MetalFence01", L"CityWorldObject/CityWorldHomeFence/Image", TextureState::AlNrRg);
		textureLoad(L"MetalFence00", L"CityWorldObject/CityWorldHomeFence/Image", TextureState::AlMtNrRg);
		textureLoad(L"ParkSign00", L"CityWorldObject/CityWorldHomeFence/Image", TextureState::AlNrRg);

		textureLoad(L"MetalFence02", L"CityWorldObject/CityWorldHomeFence/Image", TextureState::AlNrRg);

		textureLoad(L"MetalFence03", L"CityWorldObject/CityWorldHomeFence/Image", TextureState::AlNr);

#pragma endregion

#pragma region CityWorldHomeManhole
		textureLoad(L"MetalManhole01", L"CityWorldObject/CityWorldHomeManhole/CityWorldHomeManhole000/Image", TextureState::AlMtNrRg);

		textureLoad(L"MetalManhole02", L"CityWorldObject/CityWorldHomeManhole/CityWorldHomeManhole002/Image", TextureState::AlMtNrRg);

		textureLoad(L"MetalManhole03", L"CityWorldObject/CityWorldHomeManhole/CityWorldHomeManhole003/Image", TextureState::AlMtNrRg);

#pragma endregion

#pragma region CityWorldHomeSign000 And CityWorldHomeSign001
		textureLoad(L"Font_RoadSign", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign000/Image", TextureState::AlNrRg);
		textureLoad(L"LightShadow01", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign000/Image", TextureState::Al);

#pragma endregion

#pragma region CityWorldHomeSign002 And CityWorldHomeSign003
		textureLoad(L"MetalSignLong00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign002/Image", TextureState::AlEmMtNrRg);

#pragma endregion

#pragma region CityWorldHomeSign004
		textureLoad(L"MetalSignBoradSmallC00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign004/Image", TextureState::AlEmMtNrRg);

#pragma endregion

#pragma region CityWorldHomeSign006
		textureLoad(L"MetalSignBoradSmallB00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign006/Image", TextureState::AlEmMtNrRg);

#pragma endregion

#pragma region CityWorldHomeSign007
		textureLoad(L"MetalFrameSignBoard00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign007/Image", TextureState::AlNrRg);
		textureLoad(L"FrameLight00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign007/Image", TextureState::AlEmNr); //.scr도있지만 기능이 없음
		textureLoad(L"MetalFrameSignboard02", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign007/Image", TextureState::AlNr);
		textureLoad(L"SignBoardTopLight00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign007/Image", TextureState::Al);
		textureLoad(L"PosterWedding06", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign007/Image", TextureState::AlNr);

#pragma endregion

#pragma region CityWorldHomeSign008
		textureLoad(L"PosterWedding02", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign008/Image", TextureState::Al);
		//이외 Sign007의 텍스쳐를 사용함

#pragma endregion

#pragma region CityWorldHomeSign008
		//textureLoad(L"PosterWedding02", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign007/Image", TextureState::Al);
		//이외 Sign007의 텍스쳐를 사용함

#pragma endregion

#pragma region CityWorldHomeSign009
		textureLoad(L"FrameLight01", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign009/Image", TextureState::AlEmNr);
		//이외 Sign007의 텍스쳐를 사용함

#pragma endregion

#pragma region CityWorldHomeSign010
		textureLoad(L"FrameLightFront00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign010/Image", TextureState::Em);
		//SignboardFont00 찾아야됨
		textureLoad(L"PosterWedding01", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign010/Image", TextureState::Al);
		textureLoad(L"MetalWallShop00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign010/Image", TextureState::AlNrRg);
		textureLoad(L"GlassWallShop00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign010/Image", TextureState::AlEmNr);
		textureLoad(L"ShopEntrance00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign010/Image", TextureState::Em);
		textureLoad(L"FrameLightLogo00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign010/Image", TextureState::Al);
		textureLoad(L"CityPosterLight00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign010/Image", TextureState::Al);
		textureLoad(L"FrameLight04", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign010/Image", TextureState::AlNr);
		textureLoad(L"ShopSign00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign010/Image", TextureState::Al);
		textureLoad(L"CityShoprelf00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign010/Image", TextureState::AlNrRg);
		textureLoad(L"ShopMat00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign010/Image", TextureState::AlNrRg);
		textureLoad(L"MetalFrameSignBoard04", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign010/Image", TextureState::Al);
		textureLoad(L"PosterWedding00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign010/Image", TextureState::AlNr);


#pragma endregion

#pragma region CityWorldHomeSign016
		textureLoad(L"FontSignBoadA00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign016/Image", TextureState::AlEmNrRg);
		textureLoad(L"FrameLight07", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign016/Image", TextureState::Al); // _color And _scr
		//이외 Sign007의 텍스쳐를 사용함

#pragma endregion

#pragma region CityWorldHomeSign017
		textureLoad(L"PosterSign00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign017/Image", TextureState::AlNr);
		//포스터3개중 하나만 선택함

#pragma endregion

#pragma region CityWorldHomeSign020
		textureLoad(L"PosterSign02", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign018/Image", TextureState::AlNr);
		//포스터3개중 하나만 선택함

#pragma endregion

#pragma region CityWorldHomeSign022
		textureLoad(L"PosterSign02", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign018/Image", TextureState::AlNr);
		//포스터3개중 하나만 선택함

#pragma endregion

#pragma region CityWorldHomeSign027
		textureLoad(L"MetalSignBoradSmallB00", L"CityWorldObject/CityWorldHomeSign/CityWorldHomeSign027/Image", TextureState::AlEmMtNrRg);

#pragma endregion

#pragma region CityWorldFenceA And CityWorldFenceB
		textureLoad(L"BarricadePlasticWhite00", L"CityWorldObject/CityWorldFence/CityWorldFenceA/Image", TextureState::AlNrRg);

#pragma endregion
}

void AsyncLoad::LoadSounds()
{
	GETSINGLE(ResourceMgr)->Load<AudioClip>(L"SuperMarioOdyssey", L"Sound/Title/SuperMarioOdyssey.wav");
	GETSINGLE(ResourceMgr)->Load<AudioClip>(L"TitleBGM", L"Sound/Title/TitleBGM.wav");

	GETSINGLE(ResourceMgr)->Load<AudioClip>(L"GameStartSoundEffect", L"Sound/UI/GameStartSoundEffect.wav");
	GETSINGLE(ResourceMgr)->Load<AudioClip>(L"GetCoin", L"Sound/UI/GetCoin.wav");
	GETSINGLE(ResourceMgr)->Load<AudioClip>(L"MoonSoundEffect", L"Sound/UI/MoonSoundEffect.wav");
	GETSINGLE(ResourceMgr)->Load<AudioClip>(L"TextSelectSoundEffect", L"Sound/UI/TextSelectSoundEffect.wav");
	GETSINGLE(ResourceMgr)->Load<AudioClip>(L"UIBarMove", L"Sound/UI/UIBarMove.wav");

	GETSINGLE(ResourceMgr)->Load<AudioClip>(L"GetMoonSoundEffect", L"Sound/GetMoon/YOU GOT A MOON_01.wav");

	mbSoundLoad = true;
}

void AsyncLoad::loadGoomba(std::wstring shaderName)
{
#pragma region goombaBody Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"goombaBody_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(L"goombaBody_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(L"goombaBody_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"goombaBodyMaterial", material);
	}
#pragma endregion
#pragma region goombaEye0 Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"goombaEye_alb0", eTextureSlot::Albedo);
		material->SetTextureByKey(L"goombaEye_nrm0", eTextureSlot::Normal);
		material->SetTextureByKey(L"goombaEye_rgh0", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"goombaEye0Material", material);
	}
#pragma endregion
#pragma region goombaEye1 Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"goombaEye_alb1", eTextureSlot::Albedo);
		material->SetTextureByKey(L"goombaEye_nrm1", eTextureSlot::Normal);
		material->SetTextureByKey(L"goombaEye_rgh1", eTextureSlot::Roughness);
		material->SetTextureByKey(L"goombaEye_emm", eTextureSlot::Emissive);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"goombaEye1Material", material);
	}
#pragma endregion
#pragma region goombaEye2 Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"goombaEye_alb2", eTextureSlot::Albedo);
		material->SetTextureByKey(L"goombaEye_nrm2", eTextureSlot::Normal);
		material->SetTextureByKey(L"goombaEye_rgh2", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"goombaEye2Material", material);
	}
#pragma endregion
}

void AsyncLoad::loadMario(std::wstring shaderName)
{
	// 마리오
#pragma region marioBody Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"marioBody_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(L"marioBody_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(L"marioBody_mtl", eTextureSlot::Metallic);
		material->SetTextureByKey(L"marioBody_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"marioBodyMaterial", material);
	}
#pragma endregion
#pragma region marioCap Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"marioCap_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(L"marioCap_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(L"marioCap_mtl", eTextureSlot::Metallic);
		material->SetTextureByKey(L"marioCap_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"marioCapMaterial", material);
	}
#pragma endregion
#pragma region marioeye Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"marioEye_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(L"marioEye_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"marioEyeMaterial", material);
	}
#pragma endregion
#pragma region marioeyePupil Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"marioEyePupil_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(L"marioEyePupil_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(L"marioEyePupil_mtl", eTextureSlot::Metallic);
		material->SetTextureByKey(L"marioEyePupil_rgh", eTextureSlot::Roughness);
		material->SetTextureByKey(L"marioEyePupil_emm", eTextureSlot::Emissive);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"marioEyePupilMaterial", material);
	}
#pragma endregion
#pragma region marioeyeBall Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"marioEyeBall_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(L"marioEyeBall_rgh", eTextureSlot::Roughness);
		material->SetTextureByKey(L"marioEyeBall_emm", eTextureSlot::Emissive);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"marioEyeBallMaterial", material);
	}
#pragma endregion
#pragma region marioface Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"marioFace_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(L"marioFace_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"marioFaceMaterial", material);
	}
#pragma endregion
#pragma region marioHairface Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"marioHairFace_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(L"marioHairFace_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(L"marioHairFace_rgh", eTextureSlot::Roughness);
		material->SetTextureByKey(L"marioHairFace_emm", eTextureSlot::Emissive);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"marioHairFaceMaterial", material);
	}
#pragma endregion
#pragma region marioHand Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"marioHand_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(L"marioHand_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(L"marioHand_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"marioHandMaterial", material);
	}
#pragma endregion
#pragma region marioHair Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"marioHair_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(L"marioHair_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(L"marioHair_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"marioHairMaterial", material);
	}
#pragma endregion
}

void AsyncLoad::createMaterial(std::wstring fileName, std::wstring shaderName, std::wstring materialName, TextureState state)
{
	Shader* shader = nullptr;
	Material* material = nullptr;

	switch (state)
	{
	case AsyncLoad::TextureState::Al:
		shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(fileName + L"_alb", eTextureSlot::Albedo);
		GETSINGLE(ResourceMgr)->Insert<Material>(materialName, material);
		break;
	case AsyncLoad::TextureState::Em:
		shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(fileName + L"_emm", eTextureSlot::Emissive);
		GETSINGLE(ResourceMgr)->Insert<Material>(materialName, material);
		break;
	case AsyncLoad::TextureState::Nr:
		shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(fileName + L"_nrm", eTextureSlot::Normal);
		GETSINGLE(ResourceMgr)->Insert<Material>(materialName, material);
		break;
	case AsyncLoad::TextureState::AlNr:
		shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(fileName + L"_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(fileName + L"_nrm", eTextureSlot::Normal);
		GETSINGLE(ResourceMgr)->Insert<Material>(materialName, material);
		break;
	case AsyncLoad::TextureState::AlRg:
		shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(fileName + L"_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(fileName + L"_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(materialName, material);
		break;
	case AsyncLoad::TextureState::AlEm:
		shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(fileName + L"_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(fileName + L"_emm", eTextureSlot::Emissive);
		GETSINGLE(ResourceMgr)->Insert<Material>(materialName, material);
		break;
	case AsyncLoad::TextureState::AlNrRg:
		shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(fileName + L"_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(fileName + L"_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(fileName + L"_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(materialName, material);
		break;
	case AsyncLoad::TextureState::AlMtNrRg:
		shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(fileName + L"_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(fileName + L"_mtl", eTextureSlot::Metallic);
		material->SetTextureByKey(fileName + L"_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(fileName + L"_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(materialName, material);
		break;
	case AsyncLoad::TextureState::AlEmNrRg:
		shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(fileName + L"_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(fileName + L"_emm", eTextureSlot::Emissive);
		material->SetTextureByKey(fileName + L"_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(fileName + L"_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(materialName, material);
		break;
	case AsyncLoad::TextureState::AlEmMtNrRg:
		shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(fileName + L"_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(fileName + L"_emm", eTextureSlot::Emissive);
		material->SetTextureByKey(fileName + L"_mtl", eTextureSlot::Metallic);
		material->SetTextureByKey(fileName + L"_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(fileName + L"_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(materialName, material);
		break;
	case AsyncLoad::TextureState::AlEmMsNrRg:
		shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(fileName + L"_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(fileName + L"_emm", eTextureSlot::Emissive);
		//material->SetTextureByKey(fileName + L"_msk", eTextureSlot::m); //.msk -> 텍스쳐의 회색 음영을 저장하는 파일
		material->SetTextureByKey(fileName + L"_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(fileName + L"_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(materialName, material);
		break;
	case AsyncLoad::TextureState::AlEmMsMtNr:
		shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(fileName + L"_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(fileName + L"_emm", eTextureSlot::Emissive);
		//material->SetTextureByKey(fileName + L"_msk", eTextureSlot::m); //
		material->SetTextureByKey(fileName + L"_mtl", eTextureSlot::Metallic);
		material->SetTextureByKey(fileName + L"_nrm", eTextureSlot::Normal);
		GETSINGLE(ResourceMgr)->Insert<Material>(materialName, material);
		break;
	case AsyncLoad::TextureState::AlMtMsNrRg:
		shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(fileName + L"_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(fileName + L"_emm", eTextureSlot::Metallic);
		//material->SetTextureByKey(fileName + L"_msk", eTextureSlot::m); //
		material->SetTextureByKey(fileName + L"_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(fileName + L"_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(materialName, material);
		break;
	default:
		break;
	}
}

void AsyncLoad::textureLoad(std::wstring fileName, std::wstring path, TextureState state)
{
	switch (state)
	{
	case AsyncLoad::TextureState::Al:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", path + L"/" + fileName + L"_alb.png");
		break;
	case AsyncLoad::TextureState::Em:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_emm", path + L"/" + fileName + L"_emm.png");
		break;
	case AsyncLoad::TextureState::AlNr:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", path + L"/" + fileName + L"_alb.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_nrm", path + L"/" + fileName + L"_nrm.png");
		break;
	case AsyncLoad::TextureState::AlRg:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", path + L"/" + fileName + L"_alb.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_rgh", path + L"/" + fileName + L"_rgh.png");
		break;
	case AsyncLoad::TextureState::AlEm:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", path + L"/" + fileName + L"_alb.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_emm", path + L"/" + fileName + L"_emm.png");
		break;
	case AsyncLoad::TextureState::AlNrRg:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", path + L"/" + fileName + L"_alb.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_nrm", path + L"/" + fileName + L"_nrm.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_rgh", path + L"/" + fileName + L"_rgh.png");
		break;
	case AsyncLoad::TextureState::AlMtNrRg:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", path + L"/" + fileName + L"_alb.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_mtl", path + L"/" + fileName + L"_mtl.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_nrm", path + L"/" + fileName + L"_nrm.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_rgh", path + L"/" + fileName + L"_rgh.png");
		break;
	case AsyncLoad::TextureState::AlEmNrRg:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", path + L"/" + fileName + L"_alb.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_emm", path + L"/" + fileName + L"_emm.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_nrm", path + L"/" + fileName + L"_nrm.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_rgh", path + L"/" + fileName + L"_rgh.png");
		break;
	case AsyncLoad::TextureState::AlEmMtNrRg:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", path + L"/" + fileName + L"_alb.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_emm", path + L"/" + fileName + L"_emm.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_mtl", path + L"/" + fileName + L"_mtl.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_nrm", path + L"/" + fileName + L"_nrm.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_rgh", path + L"/" + fileName + L"_rgh.png");
		break;
	case AsyncLoad::TextureState::AlEmMsNrRg:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", path + L"/" + fileName + L"_alb.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_emm", path + L"/" + fileName + L"_emm.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_msk", path + L"/" + fileName + L"_msk.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_nrm", path + L"/" + fileName + L"_nrm.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_rgh", path + L"/" + fileName + L"_rgh.png");
		break;
	case AsyncLoad::TextureState::AlMtMsNrRg:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", path + L"/" + fileName + L"_alb.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_mtl", path + L"/" + fileName + L"_mtl.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_msk", path + L"/" + fileName + L"_msk.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_nrm", path + L"/" + fileName + L"_nrm.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_rgh", path + L"/" + fileName + L"_rgh.png");
		break;
	case AsyncLoad::TextureState::AlEmMsMtNr:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", path + L"/" + fileName + L"_alb.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_emm", path + L"/" + fileName + L"_emm.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_msk", path + L"/" + fileName + L"_msk.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_mtl", path + L"/" + fileName + L"_mtl.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_nrm", path + L"/" + fileName + L"_nrm.png");
		break;
	default:
		break;
	}
}

