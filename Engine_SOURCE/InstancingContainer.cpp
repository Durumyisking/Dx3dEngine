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
	// 매 프레임  mObjectsWorldMatrix를 업데이트 해야한다.
	// 방법 1 : 여기서 for문돌면서 다 하기, 방법 2 : instantiativeObject에서  자기의 인스턴스 번호를 알고 있게하고, 해당 인덱스의 매트릭스 업데이트하기

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
