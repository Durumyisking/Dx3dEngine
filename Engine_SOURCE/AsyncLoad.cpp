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
	
	std::thread thread2([this]()
	{
		GETSINGLE(ResourceMgr)->LoadModel_Monster(&mbMonsterLoadFinish);
	   // GETSINGLE(ResourceMgr)->LoadModel_CityWorld(&mbMapLoadFinish);
	});
	thread2.detach();

	// 쓰레드를 너무 많이 돌리면 제대로 동작을 안하나봄? 똑같은 코드 LoadModel_Monster 여기서 돌리면 잘 됨
	//std::thread thread3([this]()
	//{
	//	GETSINGLE(ResourceMgr)->LoadModel_CityWorld(&mbMapLoadFinish);
	//});
	//thread3.detach();

}

void AsyncLoad::LoadTextures()
{
	GETSINGLE(ResourceMgr)->Load<Texture>(L"BrickBlockBody_alb", L"brick/BlockBrickBody_alb.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"BrickBlockBody_nrm", L"brick/BlockBrickBody_nrm.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"BrickBlockBody_mtl", L"brick/BlockBrickBody_mtl.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"BrickBlockBody_rgh", L"brick/BlockBrickBody_rgh.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"BrickBlockBody_emm", L"brick/BlockBrickBody_emm.png");

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

	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaHairFace_alb", L"goomba/Image/KuriboHairFace_alb.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaHairFace_nrm", L"goomba/Image/KuriboHairFace_nrm.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaHairFace_rgh", L"goomba/Image/KuriboHairFace_rgh.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"goombaHairFace_emm", L"goomba/Image/KuriboHairFace_emm.png");

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


	GETSINGLE(ResourceMgr)->Load<Texture>(L"SkySphereTexture", L"SkyCityNightStar_color.png");

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

	//loadCityTexture();
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
	//loadCityObjectMaterial();

	renderer::CreateUIMaterial();
}

void AsyncLoad::loadCityObjectMaterial()
{
#pragma region CityWorldHomeBuilding002 Material
	createMaterial(L"GlassBuildingWall00", L"DeferredShader", L"GlassBuildingWallMaterial", TextureState::AlEmMtNrRg);
	createMaterial(L"RoofConcrete00", L"DeferredShader", L"RoofConcrete00Material", TextureState::AlNrRg);
	createMaterial(L"RoofConcrete01", L"DeferredShader", L"RoofConcrete01Material", TextureState::AlNrRg);
	createMaterial(L"WallGlassPaintedSteel00", L"DeferredShader", L"WallGlassPaintedSteelMaterial", TextureState::AlNr);

#pragma endregion

#pragma region CityWorldHomeBuilding001 Material
	createMaterial(L"GlassBuildingWall00", L"DeferredShader", L"HomeBuilding001_0Material", TextureState::AlEmMtNrRg);
	createMaterial(L"RoofConcrete00", L"DeferredShader", L"HomeBuilding001_1Material", TextureState::AlNrRg);
	createMaterial(L"RoofConcrete01", L"DeferredShader", L"HomeBuilding001_2Material", TextureState::AlNrRg);
	createMaterial(L"WallGlassPaintedSteel00", L"DeferredShader", L"HomeBuilding001_3Material", TextureState::AlNr);

#pragma endregion
#pragma region CityWorldHomeBuilding003 Material
	createMaterial(L"GroundLawn00", L"DeferredShader", L"HomeBuilding003_0Material", TextureState::AlNrRg);
	createMaterial(L"MetalFence00", L"DeferredShader", L"HomeBuilding003_1Material", TextureState::AlMtNrRg);

#pragma endregion

#pragma region HomeStageGroundCollider Material
	createMaterial(L"SideWalk01", L"DeferredShader", L"StageGroundCollider_0Material", TextureState::AlNrRg);
	createMaterial(L"ConcreteWall00", L"DeferredShader", L"StageGroundCollider_1Material", TextureState::AlNrRg);
	createMaterial(L"RoadAsphaltGlossy00", L"DeferredShader", L"StageGroundCollider_2Material", TextureState::AlNrRg);

#pragma endregion

#pragma region NaviCollider Material
	createMaterial(L"BlueseatStepRepeat00", L"DeferredShader", L"NaviCollider_0Material", TextureState::AlNr);
	createMaterial(L"MetalScaffoldRepeat00", L"DeferredShader", L"NaviCollider_1Material", TextureState::AlNrRg);
	createMaterial(L"MetalScaffold00", L"DeferredShader", L"NaviCollider_2Material", TextureState::AlNrRg);
	createMaterial(L"SideWalk00", L"DeferredShader", L"NaviCollider_3Material", TextureState::AlNrRg);
	createMaterial(L"GroundCityLogo00", L"DeferredShader", L"NaviCollider_3Material", TextureState::AlMtNrRg);
	createMaterial(L"GroundEarth00", L"DeferredShader", L"NaviCollider_4Material", TextureState::AlMtNrRg);
	createMaterial(L"GroundMoon00", L"DeferredShader", L"NaviCollider_5Material", TextureState::AlNrRg);
	createMaterial(L"GroundCityLogo02", L"DeferredShader", L"NaviCollider_6Material", TextureState::AlNrRg);
	createMaterial(L"MetalManhole03", L"DeferredShader", L"NaviCollider_7Material", TextureState::AlMtNrRg);
	createMaterial(L"GroundTile00", L"DeferredShader", L"NaviCollider_8Material", TextureState::AlNrRg);
	createMaterial(L"GroundTile01", L"DeferredShader", L"NaviCollider_9Material", TextureState::AlNrRg);
	createMaterial(L"MetalManhole01", L"DeferredShader", L"NaviCollider_10Material", TextureState::AlMtNrRg);
	createMaterial(L"GroundLawn00", L"DeferredShader", L"NaviCollider_11Material", TextureState::AlNrRg);
	createMaterial(L"GroundLawn01", L"DeferredShader", L"NaviCollider_12Material", TextureState::AlNrRg);
	createMaterial(L"GroundSoil00", L"DeferredShader", L"NaviCollider_13Material", TextureState::AlNrRg);
	createMaterial(L"SideWalk01", L"DeferredShader", L"NaviCollider_14Material", TextureState::AlNrRg);
	createMaterial(L"SideWalk02", L"DeferredShader", L"NaviCollider_15Material", TextureState::AlNr);
	createMaterial(L"ConcreteWall00", L"DeferredShader", L"NaviCollider_16Material", TextureState::AlNrRg);
	createMaterial(L"RoadAsphaltGlossy00", L"DeferredShader", L"NaviCollider_17Material", TextureState::AlNrRg);


#pragma endregion

#pragma region CityGround Material
	{
		createMaterial(L"AsphaltSeal00", L"DeferredShader", L"AsphaltSeal00Material", TextureState::AlNrRg);
		createMaterial(L"BaseAsphaltRoad01", L"DeferredShader", L"BaseAsphaltRoad01Material", TextureState::AlNrRg);
		createMaterial(L"BlueseatStepRepeat00", L"DeferredShader", L"BlueseatStepRepeat00Material", TextureState::AlNr);
		createMaterial(L"CityLitter00", L"DeferredShader", L"CityLitter00Material", TextureState::AlNrRg);
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
	createMaterial(L"coinbody00", L"DeferredShader", L"CoinMaterial", TextureState::AlNr);

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
	createMaterial(L"BlockBrickBody", L"DeferredShader", L"BlockBrickMaterial", TextureState::AlEmMtNrRg);

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
}

void AsyncLoad::loadCityTexture()
{
#pragma region CityWorldHomeBuilding002
	//CityWorldHomeBuilding002
	textureLoad(L"GlassBuildingWall00", L"CityWorldHomeBuilding002/Image", TextureState::AlEmMtNrRg);
	textureLoad(L"RoofConcrete00", L"CityWorldHomeBuilding002/Image", TextureState::AlNrRg);
	textureLoad(L"RoofConcrete01", L"CityWorldHomeBuilding002/Image", TextureState::AlNrRg);
	textureLoad(L"WallGlassPaintedSteel00", L"CityWorldHomeBuilding002/Image", TextureState::AlNr);

#pragma endregion
	//CityWorldHomeBuilding001

#pragma region CityWorldHomeBuilding003
	//CityWorldHomeBuilding003
	textureLoad(L"GroundLawn00", L"CityWorldHomeBuilding003/Image", TextureState::AlNrRg);
	textureLoad(L"MetalFence00", L"CityWorldHomeBuilding003/Image", TextureState::AlMtNrRg);

#pragma endregion

#pragma region NaviCollider
	//NaviCollider
	textureLoad(L"GroundEarth00", L"NaviCollider/Image", TextureState::AlMtNrRg);
	textureLoad(L"GroundMoon00", L"NaviCollider/Image", TextureState::AlNrRg);

#pragma endregion

#pragma region CityGround
	//CityGround
	textureLoad(L"AsphaltSeal00", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"BaseAsphaltRoad01", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"BlueseatStepRepeat00", L"CityGround/Image", TextureState::AlNr);
	textureLoad(L"CityLitter00", L"CityGround/Image", TextureState::AlMtNrRg);
	textureLoad(L"CityLitter01", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"ConcreteWall00", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"DrainageCover00", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"Font_BaseRoadSign00", L"CityGround/Image", TextureState::Al);
	textureLoad(L"Font_BaseRoadSign01", L"CityGround/Image", TextureState::Al);
	textureLoad(L"GrassFlowerSet00", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"GroundCityLogo00", L"CityGround/Image", TextureState::AlMtNrRg);
	textureLoad(L"GroundCityLogo02", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"GroundEarth01", L"CityGround/Image", TextureState::AlMtNrRg);
	textureLoad(L"GroundLawn00", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"GroundLawn01", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"GroundLight00", L"CityGround/Image", TextureState::AlEmMsNrRg);
	textureLoad(L"GroundMoon01", L"CityGround/Image", TextureState::AlMtNrRg);
	textureLoad(L"GroundSoil00", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"GroundTile00", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"GroundTile01", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"MetalManhole01", L"CityGround/Image", TextureState::AlMtNrRg);
	textureLoad(L"MetalManhole03", L"CityGround/Image", TextureState::AlMtNrRg);
	textureLoad(L"MetalScaffold00", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"MetalScaffoldRepeat00", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"RoadAsphaltGlossy00", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"RoadManhole00", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"SideWalk00", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"SideWalk01", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"SideWalk02", L"CityGround/Image", TextureState::AlNr);
	textureLoad(L"WallFastener00", L"CityGround/Image", TextureState::AlNrRg);
	textureLoad(L"WallGrunge00", L"CityGround/Image", TextureState::Al);
	textureLoad(L"WallGrunge01", L"CityGround/Image", TextureState::Al);

#pragma endregion

#pragma region CityWorldHomeBench000
	//CityWorldHomeBench000
	textureLoad(L"BenchiWood00", L"CityWorldHomeBench000/Image", TextureState::AlMtNrRg);

#pragma endregion
	
#pragma region CityWorldBush
		textureLoad(L"CityBushLeaf", L"CityWorldBush/Image", TextureState::AlNrRg);
	    textureLoad(L"CityBushBody", L"CityWorldBush/Image", TextureState::Al);

#pragma endregion

#pragma region CityWorldBushA
		textureLoad(L"LeafTree00", L"CityWorldBushA/Image", TextureState::AlNrRg);
		textureLoad(L"LeafTree01", L"CityWorldBushA/Image", TextureState::AlNrRg);

#pragma endregion

#pragma region CityWorldChairA
		textureLoad(L"PaintChair00", L"CityWorldChairA/Image", TextureState::AlNrRg);

#pragma endregion

#pragma region CityWorldBushB
		textureLoad(L"CityWorldChairB00", L"CityWorldChairB/Image", TextureState::AlNrRg);

#pragma endregion

#pragma region CheckpointFlag
		textureLoad(L"CheckpointFlagBody", L"CheckpointFlag/Image", TextureState::AlMtMsNrRg);
		textureLoad(L"CheckpointFlagMark0", L"CheckpointFlag/Image", TextureState::AlNrRg);
		textureLoad(L"CheckpointFlagMark1", L"CheckpointFlag/Image", TextureState::AlNrRg);
		//textureLoad(L"CheckpointFlagMark2_mask", L"CheckpointFlag/Image", TextureState::AlNrRg);
		textureLoad(L"CheckpointFlagScroll", L"CheckpointFlag/Image", TextureState::AlNr);

#pragma endregion

#pragma region CItyWorldFlag
		textureLoad(L"ClothFlagCity00", L"CItyWorldFlag/Image", TextureState::AlNr);
		textureLoad(L"MetalFlagPole00", L"CItyWorldFlag/Image", TextureState::AlNr);

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
		textureLoad(L"motorcyclebody", L"Motorcycle/Image", TextureState::AlMtNrRg);

#pragma endregion

#pragma region TaxiCar
		textureLoad(L"carbody", L"TaxiCar/Car/Image", TextureState::AlEmMtNrRg);
		textureLoad(L"carwindow", L"TaxiCar/Car/Image", TextureState::AlNrRg);

#pragma endregion

#pragma region CarBreakable
		textureLoad(L"carbodyBreakable", L"TaxiCar/CarBreakable/Image", TextureState::AlEmMtNrRg);

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

	switch (state
)
	{
	case AsyncLoad::TextureState::Al:
		shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(fileName + L"_alb", eTextureSlot::Albedo);
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
		//material->SetTextureByKey(fileName + L"_msk", eTextureSlot::m); //.msk가 무엇인지 알 필요가  있음
		material->SetTextureByKey(fileName + L"_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(fileName + L"_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(materialName, material);
		break;
	case AsyncLoad::TextureState::AlMtMsNrRg:
		shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
		material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(fileName + L"_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(fileName + L"_emm", eTextureSlot::Metallic);
		//material->SetTextureByKey(fileName + L"_msk", eTextureSlot::m); //.msk가 무엇인지 알 필요가  있음
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
	case AsyncLoad::TextureState::AlNr:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", path + L"/" + fileName + L"_alb.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_nrm", path + L"/" + fileName + L"_nrm.png");
		break;
	case AsyncLoad::TextureState::AlRg:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", path + L"/" + fileName + L"_alb.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_rgh", path + L"/" + fileName + L"_rgh.png");
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
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_mtl", path + L"/" + fileName + L"_emm.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_msk", path + L"/" + fileName + L"_msk.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_nrm", path + L"/" + fileName + L"_nrm.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_rgh", path + L"/" + fileName + L"_rgh.png");
		break;
	default:
		break;
	}
}

//  void AsyncLoad::loadBuildings(std::wstring shaderName)
//{
//
//	{
//		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
//		Material* material = new Material();
//		material->SetShader(shader);
//		material->SetTextureByKey(L"GlassBuildingWall00_alb", eTextureSlot::Albedo);
//		material->SetTextureByKey(L"GlassBuildingWall00_emm", eTextureSlot::Emissive);
//		material->SetTextureByKey(L"GlassBuildingWall00_mtl", eTextureSlot::Metallic);
//		material->SetTextureByKey(L"GlassBuildingWall00_nrm", eTextureSlot::Normal);
//		material->SetTextureByKey(L"GlassBuildingWall00_rgh", eTextureSlot::Roughness);
//		GETSINGLE(ResourceMgr)->Insert<Material>(L"GlassBuildingWallMaterial", material);
//	}
//
//	{
//		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
//		Material* material = new Material();
//		material->SetShader(shader);
//		material->SetTextureByKey(L"RoofConcrete00_alb", eTextureSlot::Albedo);
//		material->SetTextureByKey(L"RoofConcrete00_nrm", eTextureSlot::Normal);
//		material->SetTextureByKey(L"RoofConcrete00_rgh", eTextureSlot::Roughness);
//		GETSINGLE(ResourceMgr)->Insert<Material>(L"RoofConcrete00Material", material);
//	}
//
//	{
//		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
//		Material* material = new Material();
//		material->SetShader(shader);
//		material->SetTextureByKey(L"RoofConcrete01_alb", eTextureSlot::Albedo);
//		material->SetTextureByKey(L"RoofConcrete01_nrm", eTextureSlot::Normal);
//		material->SetTextureByKey(L"RoofConcrete01_rgh", eTextureSlot::Roughness);
//		GETSINGLE(ResourceMgr)->Insert<Material>(L"RoofConcrete01Material", material);
//	}
//
//	{
//		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderName);
//		Material* material = new Material();
//		material->SetShader(shader);
//		material->SetTextureByKey(L"WallGlassPaintedSteel00_alb", eTextureSlot::Albedo);
//		material->SetTextureByKey(L"WallGlassPaintedSteel00_nrm", eTextureSlot::Normal);
//		GETSINGLE(ResourceMgr)->Insert<Material>(L"WallGlassPaintedSteelMaterial", material);
//
//	}
//}
