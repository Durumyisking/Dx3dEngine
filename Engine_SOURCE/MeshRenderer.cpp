#include "MeshRenderer.h"
#include "GameObj.h"
#include "Transform.h"
#include "Model.h"
#include "Mesh.h"



MeshRenderer::MeshRenderer()
	: BaseRenderer(eComponentType::MeshRenderer)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Initialize()
{
	//t.loadFromFile("E:/Dev/3d-my/Dx3dEngine/Resources/environment.hdr");

	BaseRenderer::Initialize();
}

void MeshRenderer::Update()
{
	BaseRenderer::Update();
}

void MeshRenderer::FixedUpdate()
{
	BaseRenderer::FixedUpdate();
}

void MeshRenderer::PrevRender()
{
	Material* material = GETSINGLE(ResourceMgr)->Find<Material>(L"ShadowMaterial");

	material->Bind();

	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	//BoneAnimatior* animator
	//	= GetOwner()->GetComponent<BoneAnimatior>();

	//UINT subSetCount = GetMesh()->GetSubSetCount();
	for (size_t i = 0; i < subSetCount; i++)
	{
		if (animator)
		{
			animator->Binds();
			material->SetAnimation(true);
			UINT boneCount = animator->GetBoneCount();
			material->SetBoneCount(boneCount);
		}

		GetMesh()->BindBuffer(i);
		GetMesh()->Render(i);

		material->Clear();
	}

	if (animator)
	{
		animator->ClearData();
	}
}

void MeshRenderer::Render()
{
	BaseRenderer::Render();

	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	if (GetModel() != nullptr)
	{
		GetModel()->Bind_Render();
	}
	else
	{
		GetMaterial()->Bind();
		GetMesh()->BindBuffer();
		GetMesh()->Render();
		GetMaterial()->Clear();
	}
}
