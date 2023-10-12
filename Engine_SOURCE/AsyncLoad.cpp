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
		mbMapLoadFinish &&
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
	std::thread thread2([this]()
	{
		GETSINGLE(ResourceMgr)->LoadModel_Monster(&mbMonsterLoadFinish);
	});
	std::thread thread3([this]()
	{
		GETSINGLE(ResourceMgr)->LoadModel_CityWorld(&mbMapLoadFinish);
	});
	thread1.detach();
	thread2.detach();
	thread3.detach();

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


	//Building
	GETSINGLE(ResourceMgr)->Load<Texture>(L"GlassBuildingWall00_alb", L"CityWorldHomeBuilding002/Image/GlassBuildingWall00_alb.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"GlassBuildingWall00_emm", L"CityWorldHomeBuilding002/Image/GlassBuildingWall00_emm.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"GlassBuildingWall00_mtl", L"CityWorldHomeBuilding002/Image/GlassBuildingWall00_mtl.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"GlassBuildingWall00_nrm", L"CityWorldHomeBuilding002/Image/GlassBuildingWall00_nrm.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"GlassBuildingWall00_rgh", L"CityWorldHomeBuilding002/Image/GlassBuildingWall00_rgh.png");

	GETSINGLE(ResourceMgr)->Load<Texture>(L"RoofConcrete00_alb", L"CityWorldHomeBuilding002/Image/RoofConcrete00_alb.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"RoofConcrete00_nrm", L"CityWorldHomeBuilding002/Image/RoofConcrete00_nrm.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"RoofConcrete00_rgh", L"CityWorldHomeBuilding002/Image/RoofConcrete00_rgh.png");

	GETSINGLE(ResourceMgr)->Load<Texture>(L"RoofConcrete01_alb", L"CityWorldHomeBuilding002/Image/RoofConcrete01_alb.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"RoofConcrete01_nrm", L"CityWorldHomeBuilding002/Image/RoofConcrete01_nrm.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"RoofConcrete01_rgh", L"CityWorldHomeBuilding002/Image/RoofConcrete01_rgh.png");

	GETSINGLE(ResourceMgr)->Load<Texture>(L"WallGlassPaintedSteel00_alb", L"CityWorldHomeBuilding002/Image/WallGlassPaintedSteel00_alb.png");
	GETSINGLE(ResourceMgr)->Load<Texture>(L"WallGlassPaintedSteel00_nrm", L"CityWorldHomeBuilding002/Image/WallGlassPaintedSteel00_nrm.png");


	//CityGround
	//GETSINGLE(ResourceMgr)->Load<Texture>(L"AsphaltSeal00_alb", L"CityGround/Image/AsphaltSeal00_alb.png");
	//GETSINGLE(ResourceMgr)->Load<Texture>(L"AsphaltSeal00_nrm", L"CityGround/Image/AsphaltSeal00_nrm.png");
	//GETSINGLE(ResourceMgr)->Load<Texture>(L"AsphaltSeal00_rgh", L"CityGround/Image/AsphaltSeal00_rgh.png");

	//GETSINGLE(ResourceMgr)->Load<Texture>(L"BaseAsphaltRoad01_alb", L"CityGround/Image/BaseAsphaltRoad01_alb.png");
	//GETSINGLE(ResourceMgr)->Load<Texture>(L"BaseAsphaltRoad01_nrm", L"CityGround/Image/BaseAsphaltRoad01_nrm.png");
	//GETSINGLE(ResourceMgr)->Load<Texture>(L"BaseAsphaltRoad01_rgh", L"CityGround/Image/BaseAsphaltRoad01_rgh.png");

	//GETSINGLE(ResourceMgr)->Load<Texture>(L"BlueseatStepRepeat00_alb", L"CityGround/Image/BlueseatStepRepeat00_alb.png");
	//GETSINGLE(ResourceMgr)->Load<Texture>(L"BlueseatStepRepeat00_nrm", L"CityGround/Image/BlueseatStepRepeat00_nrm.png");

	//GETSINGLE(ResourceMgr)->Load<Texture>(L"CityLitter00_alb", L"CityGround/Image/CityLitter00_alb.png");
	//GETSINGLE(ResourceMgr)->Load<Texture>(L"CityLitter00_mtl", L"CityGround/Image/CityLitter00_mtl.png");
	//GETSINGLE(ResourceMgr)->Load<Texture>(L"CityLitter00_nrm", L"CityGround/Image/CityLitter00_nrm.png");
	//GETSINGLE(ResourceMgr)->Load<Texture>(L"CityLitter00_rgh", L"CityGround/Image/CityLitter00_rgh.png");

	//GETSINGLE(ResourceMgr)->Load<Texture>(L"CityLitter01_alb", L"CityGround/Image/CityLitter01_alb.png");
	//GETSINGLE(ResourceMgr)->Load<Texture>(L"CityLitter01_nrm", L"CityGround/Image/CityLitter01_nrm.png");
	//GETSINGLE(ResourceMgr)->Load<Texture>(L"CityLitter01_rgh", L"CityGround/Image/CityLitter01_rgh.png");

	//GETSINGLE(ResourceMgr)->Load<Texture>(L"ConcreteWall00_alb", L"CityGround/Image/ConcreteWall00_alb.png");
	//GETSINGLE(ResourceMgr)->Load<Texture>(L"ConcreteWall00_nrm", L"CityGround/Image/ConcreteWall00_nrm.png");
	//GETSINGLE(ResourceMgr)->Load<Texture>(L"ConcreteWall00_rgh", L"CityGround/Image/ConcreteWall00_rgh.png");

	//GETSINGLE(ResourceMgr)->Load<Texture>(L"DrainageCover00_alb", L"CityGround/Image/DrainageCover00_alb.png");
	//GETSINGLE(ResourceMgr)->Load<Texture>(L"DrainageCover00_nrm", L"CityGround/Image/DrainageCover00_nrm.png");
	//GETSINGLE(ResourceMgr)->Load<Texture>(L"DrainageCover00_rgh", L"CityGround/Image/DrainageCover00_rgh.png");

	//GETSINGLE(ResourceMgr)->Load<Texture>(L"Font_BaseRoadSign00_alb", L"CityGround/Image/Font_BaseRoadSign00_alb.png");

	//GETSINGLE(ResourceMgr)->Load<Texture>(L"Font_BaseRoadSign01_alb", L"CityGround/Image/Font_BaseRoadSign01_alb.png");

	//GETSINGLE(ResourceMgr)->Load<Texture>(L"GrassFlowerSet00_alb", L"CityGround/Image/GrassFlowerSet00_alb.png");
	//GETSINGLE(ResourceMgr)->Load<Texture>(L"GrassFlowerSet00_nrm", L"CityGround/Image/GrassFlowerSet00_nrm.png");
	//GETSINGLE(ResourceMgr)->Load<Texture>(L"GrassFlowerSet00_rgh", L"CityGround/Image/GrassFlowerSet00_rgh.png");

	renderer::CreateUITexture();

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

#pragma region GlassBuilding Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DeferredShader");
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"GlassBuildingWall00_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(L"GlassBuildingWall00_emm", eTextureSlot::Emissive);
		material->SetTextureByKey(L"GlassBuildingWall00_mtl", eTextureSlot::Metallic);
		material->SetTextureByKey(L"GlassBuildingWall00_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(L"GlassBuildingWall00_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"GlassBuildingWallMaterial", material);
	}
	
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DeferredShader");
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"RoofConcrete00_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(L"RoofConcrete00_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(L"RoofConcrete00_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"RoofConcrete00Material", material);
	}

	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DeferredShader");
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"RoofConcrete01_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(L"RoofConcrete01_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(L"RoofConcrete01_rgh", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"RoofConcrete01Material", material);
	}

	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DeferredShader");
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"WallGlassPaintedSteel00_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(L"WallGlassPaintedSteel00_nrm", eTextureSlot::Normal);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"WallGlassPaintedSteelMaterial", material);
	}


#pragma endregion

	loadGoomba(L"DeferredShader");
	loadMario( L"DeferredShader");

	renderer::CreateUIMaterial();
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
	// ¸¶¸®¿À
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

void AsyncLoad::CreateMaterial()
{

}

void AsyncLoad::TextureLoad(std::wstring& fileName, std::wstring& _PathAndUntilfileName, TextureState state)
{
	switch (state)
	{
	case AsyncLoad::TextureState::Al:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", _PathAndUntilfileName + L"_alb");
		break;
	case AsyncLoad::TextureState::AlNr:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", _PathAndUntilfileName + L"_alb");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_nrm", _PathAndUntilfileName + L"_nrm");
		break;
	case AsyncLoad::TextureState::AlNrRg:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", _PathAndUntilfileName + L"_alb");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_nrm", _PathAndUntilfileName + L"_nrm");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_rgh", _PathAndUntilfileName + L"_rgh");
		break;
	case AsyncLoad::TextureState::AlMtNrRg:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", _PathAndUntilfileName + L"_alb");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_mtl", _PathAndUntilfileName + L"_mtl");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_nrm", _PathAndUntilfileName + L"_nrm");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_rgh", _PathAndUntilfileName + L"_rgh");
		break;
	case AsyncLoad::TextureState::AlEmMtNrRg:
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_alb", _PathAndUntilfileName + L"_alb");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_emm", _PathAndUntilfileName + L"_emm");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_mtl", _PathAndUntilfileName + L"_mtl");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_nrm", _PathAndUntilfileName + L"_nrm");
		GETSINGLE(ResourceMgr)->Load<Texture>(fileName + L"_rgh", _PathAndUntilfileName + L"_rgh");
		break;
	default:
		break;
	}
}
