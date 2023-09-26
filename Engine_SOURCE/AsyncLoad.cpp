#include "AsyncLoad.h"
#include "ResourceMgr.h"
#include "Texture.h"
#include "Material.h"

AsyncLoad::AsyncLoad()
	: mbLoadFinish(false)
	, mbTextureLoadFinish (nullptr)
	, mbMarioLoadFinish (nullptr)
	, mbMonsterLoadFinish (nullptr)
	, mbMapLoadFinish (nullptr)
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
		mbMapLoadFinish )
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


#pragma region goombaBody Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PBRShader");
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
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PBRShader");
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
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PBRShader");
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
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PBRShader");
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"goombaEye_alb2", eTextureSlot::Albedo);
		material->SetTextureByKey(L"goombaEye_nrm2", eTextureSlot::Normal);
		material->SetTextureByKey(L"goombaEye_rgh2", eTextureSlot::Roughness);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"goombaEye2Material", material);
	}
#pragma endregion
#pragma region MarioMustache Material
	{
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PBRShader");
		Material* material = new Material();
		material->SetShader(shader);
		material->SetTextureByKey(L"goombaHairFace_alb", eTextureSlot::Albedo);
		material->SetTextureByKey(L"goombaHairFace_nrm", eTextureSlot::Normal);
		material->SetTextureByKey(L"goombaHairFace_rgh", eTextureSlot::Roughness);
		material->SetTextureByKey(L"goombaHairFace_emm", eTextureSlot::Emissive);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"MarioMustacheMaterial", material);
	}
#pragma endregion

	renderer::CreateUIMaterial();
}
