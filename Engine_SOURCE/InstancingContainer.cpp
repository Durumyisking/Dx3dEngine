#include "InstancingContainer.h"
#include "InstancedMeshRenderer.h"
#include "BlockBrick.h"
#include "MeshRenderer.h"

InstancingContainer::InstancingContainer()
	:mObjects{}
	, mObjectsInstancingData{}
	, mRenderingMode(eRenderingMode::DeferredOpaque)
{
}

InstancingContainer::~InstancingContainer()
{
}

void InstancingContainer::Initialize()
{
	AddComponent<InstancedMeshRenderer>(eComponentType::MeshRenderer);

	GameObj::Initialize();
}

void InstancingContainer::Update()
{
	GetComponent<InstancedMeshRenderer>()->Update();
}

void InstancingContainer::FixedUpdate()
{
	// �� ������  mObjectsWorldMatrix�� ������Ʈ �ؾ��Ѵ�.
	// ��� 1 : ���⼭ for�����鼭 �� �ϱ�, ��� 2 : instantiativeObject����  �ڱ��� �ν��Ͻ� ��ȣ�� �˰� �ְ��ϰ�, �ش� �ε����� ��Ʈ���� ������Ʈ�ϱ�

	//GameObj::FixedUpdate();
	GetComponent<InstancedMeshRenderer>()->SetInstanceCount(static_cast<UINT>(mObjects.size()));
	GetComponent<InstancedMeshRenderer>()->SetModel(mObjects[0]->GetComponent<MeshRenderer>()->GetModel());

	GetComponent<InstancedMeshRenderer>()->FixedUpdate();
}

void InstancingContainer::Render()
{
	GetComponent<InstancedMeshRenderer>()->Render();
}

void InstancingContainer::PrevRender()
{
	GetComponent<InstancedMeshRenderer>()->PrevRender();
}

void InstancingContainer::PushObject(InstantiativeObject* obj)
{
	obj->SetContainer(this, static_cast<UINT>(mObjects.size()));
	mObjects.emplace_back(obj);
}
