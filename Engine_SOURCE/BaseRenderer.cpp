#include "BaseRenderer.h"
#include "Texture.h"
#include "Transform.h"
#include "GameObj.h"
#include "Material.h"
#include "Mesh.h"


BaseRenderer::BaseRenderer(eComponentType type)
	:Component(type)
	, mbIsAnim(false)
	, mbUseLOD(false)
	, mSpriteSize(Vector2::Zero)
	, mMesh (nullptr)
	, mMeshKey()
	, mModel (nullptr)
	, mMaterial (nullptr)
	, mMaterialKey()
{
	// 디폴트 매시 지정
	Mesh* mesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Cubemesh");

	SetMesh(mesh);
}
BaseRenderer::~BaseRenderer()
{
}
void BaseRenderer::Initialize()
{
}
void BaseRenderer::Update()
{
}
void BaseRenderer::FixedUpdate()
{

}

void BaseRenderer::Render()
{
	if (mbUseLOD)
	{
		//LOD();
	}
}

void BaseRenderer::SetMeshByKey(std::wstring key)
{
	mMesh = GETSINGLE(ResourceMgr)->Find<Mesh>(key);
	mMeshKey = key;
}

void BaseRenderer::SetMaterial(Material* material, UINT modelMeshSlot)
{
	if (mModel)
	{
		mModel->SetVariableMaterials(modelMeshSlot, material);
	}
	else
	{
		mMaterial = material;
	}
}

void BaseRenderer::SetMaterialByKey(std::wstring key, UINT modelMeshSlot)
{
	if (mModel)
	{
		mModel->SetVariableMaterialsByKey(modelMeshSlot, key);
		mMaterial = GETSINGLE(ResourceMgr)->Find<Material>(key);
		mMaterialKey = key;
	}
	else
	{
		mMaterial = GETSINGLE(ResourceMgr)->Find<Material>(key);
		mMaterialKey = key;
	}
}

void BaseRenderer::SetMaterial(std::wstring key)
{
	if (mModel)
	{
		mMaterial = GETSINGLE(ResourceMgr)->Find<Material>(key);
		mMaterialKey = key;
	}
	else
	{
		mMaterial = GETSINGLE(ResourceMgr)->Find<Material>(key);
		mMaterialKey = key;
	}
}

void BaseRenderer::ForceSetMaterial(Material* material)
{
	if (material == nullptr)
		return;

	mMaterial = material;
	mMaterialKey = material->GetName();
}

void BaseRenderer::ForceSetMaterialByKey(std::wstring key)
{
	Material* mt = GETSINGLE(ResourceMgr)->Find<Material>(key);

	if (mt == nullptr)
		return;

	mMaterial = mt;
	mMaterialKey = key;
}

void BaseRenderer::SetAnimMaterial(Material* material, Vector2 spriteSize)
{
	mMaterial = material;
	mbIsAnim = true;
	mSpriteSize = spriteSize;
}

void BaseRenderer::SetModelByKey(std::wstring key)
{
	mModel = GETSINGLE(ResourceMgr)->Find<Model>(key);
}

void BaseRenderer::SetModelByKey(std::wstring modelKey, std::wstring materialKey)
{
	mModel = GETSINGLE(ResourceMgr)->Find<Model>(modelKey);
	mMaterial = GETSINGLE(ResourceMgr)->Find<Material>(materialKey);
	mMaterialKey = materialKey;
}


void BaseRenderer::ChangeColor(Vector4 color)
{
	MulColor(Vector4::Zero);
	AddColor(color);
}

void BaseRenderer::MulColor(Vector4 color)
{
	mMaterial->SetData(eGPUParam::Vector4_1, &color);
}

void BaseRenderer::AddColor(Vector4 color)
{
	mMaterial->SetData(eGPUParam::Vector4_2, &color);
}

void BaseRenderer::LOD()
{
	Vector3 camPos = renderer::mainCamera->GetOwnerWorldPos();
	Vector3 objPos = GetOwnerWorldPos();

	float distance = Vector3::Distance(camPos, objPos);

	if (mMaterial)
	{
		if (distance > 200.f)
		{
			mMaterial->SetShaderByKey(L"FlatShader");
		}
		else
		{
			mMaterial->SetShaderByKey(L"PhongShader");
		}
	}
}
