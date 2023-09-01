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
}

void BaseRenderer::SetMaterial(Material* material)
{
	mMaterial = material;
}

void BaseRenderer::SetMaterialByKey(std::wstring key)
{
	mMaterial = GETSINGLE(ResourceMgr)->Find<Material>(key);
}

void BaseRenderer::SetAnimMaterial(Material* material, Vector2 spriteSize)
{
	mMaterial = material;
	mbIsAnim = true;
	mSpriteSize = spriteSize;
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
