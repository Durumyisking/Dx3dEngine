#include "InstancedMeshRenderer.h"
#include "GameObj.h"
#include "Transform.h"
#include "Model.h"
#include "Mesh.h"
#include "BoneAnimator.h"
#include "InstancingContainer.h"
#include "InstantiativeObject.h"

InstancedMeshRenderer::InstancedMeshRenderer()
	: BaseRenderer(eComponentType::MeshRenderer)
	, mBoneAnimator(nullptr)
	, mInstanceCount(0)
	, mContainer{}
{
}

InstancedMeshRenderer::~InstancedMeshRenderer()
{
}

void InstancedMeshRenderer::Initialize()
{
	BaseRenderer::Initialize();
	BoneAnimator* animator = GetOwner()->GetComponent<BoneAnimator>();
	if (animator)
	{
		mBoneAnimator = animator;
	}
	if (eLayerType::ObjectsContainer == GetOwner()->GetLayerType())
	{
		mContainer = GetOwner_Template<InstancingContainer>();
	}
}

void InstancedMeshRenderer::Update()
{
	BaseRenderer::Update();
}

void InstancedMeshRenderer::FixedUpdate()
{
	// model내 메쉬들의 instancebuffer를 업데이트 시켜야 한다.

	if (GetModel() != nullptr)
	{
		std::vector<Mesh*> meshes = GetModel()->GetMeshes();

		for (size_t i = 0; i < meshes.size(); i++)
		{
			meshes[i]->UpdateInstanceBuffer(mContainer->GetObjectsWorldMatrix());
		}
	}

	BaseRenderer::FixedUpdate();
}

void InstancedMeshRenderer::PrevRender()
{
	/*Material* material = GETSINGLE(ResourceMgr)->Find<Material>(L"ShadowMaterial");
	material->Bind();

	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	if (GetModel() != nullptr)
	{
		GetModel()->Bind_Render(false);
	}
	else
	{
		GetMesh()->BindBuffer();
		GetMesh()->Render();
	}
	material->Clear();*/
}

void InstancedMeshRenderer::Render()
{
	BaseRenderer::Render();

	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	// 여기의 모델은 이 렌더러 컴포넌트의 오너(container)가 담고있는 0번지 objects의 meshrenderer에 들어있는 model이다.
	if (GetModel() != nullptr)
	{
		if (mBoneAnimator)
		{
			GetModel()->SetFrameAnimationVector(&(mBoneAnimator->GetFrameAnimationData()));
		}
		GetModel()->Bind_RenderInstance(mInstanceCount);
	}
	else
	{
		GetMaterial()->Bind();
		GetMesh()->BindBuffer();
		GetMesh()->RenderInstanced(mInstanceCount);
		GetMaterial()->Clear();
	}
}
