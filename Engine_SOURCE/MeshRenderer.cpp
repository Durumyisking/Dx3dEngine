#include "MeshRenderer.h"
#include "GameObj.h"
#include "Transform.h"
#include "Model.h"
#include "Mesh.h"
#include "BoneAnimator.h"



MeshRenderer::MeshRenderer()
	: BaseRenderer(eComponentType::MeshRenderer)
	, mBoneAnimator(nullptr)
	, mbRenderShadow(true)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Initialize()
{
	BaseRenderer::Initialize();
	BoneAnimator* animator = GetOwner()->GetComponent<BoneAnimator>();
	if (animator)
	{
		mBoneAnimator = animator;
	}
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
	if (mbRenderShadow)
	{
		Material* material = GETSINGLE(ResourceMgr)->Find<Material>(L"ShadowMaterial");
		material->Bind();

		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

		if (GetModel() != nullptr)
		{
			GetModel()->SetWorldMatrix(GetTransform()->GetWorldMatrix());
			GetModel()->Bind_Render(false);
		}
		else
		{
			GetMesh()->SetWorldMatrix(GetTransform()->GetWorldMatrix());
			GetMesh()->BindBuffer();
			GetMesh()->Render();
		}
		material->Clear();
	}
}

void MeshRenderer::Render()
{
	BaseRenderer::Render();

	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	if (GetModel() != nullptr)
	{
		if (mBoneAnimator)
		{
			GetModel()->SetFrameAnimationVector(&(mBoneAnimator->GetFrameAnimationData()));
		}
		GetModel()->SetWorldMatrix(GetTransform()->GetWorldMatrix());
		
		GetModel()->Bind_Render();
	}
	else
	{
		if (GetMaterial() == nullptr)
			return;

		GetMaterial()->Bind();
		GetMesh()->SetWorldMatrix(GetTransform()->GetWorldMatrix());
		GetMesh()->BindBuffer();
		GetMesh()->Render();
		GetMaterial()->Clear();
	}
}

