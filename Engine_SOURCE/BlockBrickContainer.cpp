#include "BlockBrickContainer.h"
#include "InstancedMeshRenderer.h"
#include "BlockBrick.h"
#include "MeshRenderer.h"

BlockBrickContainer::BlockBrickContainer()
	:mObjects{}
	, mRenderingMode(eRenderingMode::DeferredOpaque)
{
}

BlockBrickContainer::~BlockBrickContainer()
{
}

void BlockBrickContainer::Initialize()
{
	AddComponent<InstancedMeshRenderer>(eComponentType::MeshRenderer);

}

void BlockBrickContainer::Update()
{
	//GameObj::Update();
}

void BlockBrickContainer::FixedUpdate()
{
	//GameObj::FixedUpdate();
	GetComponent<InstancedMeshRenderer>()->SetInstanceCount(mObjects.size());
	GetComponent<InstancedMeshRenderer>()->SetModel(mObjects[0]->GetComponent<MeshRenderer>()->GetModel());
}

void BlockBrickContainer::Render()
{
	GetComponent<InstancedMeshRenderer>()->Render();
}

void BlockBrickContainer::PrevRender()
{
	GetComponent<InstancedMeshRenderer>()->PrevRender();
}

//void BlockBrickContainer::PushBlock(BlockBrick* block)
//{
//	mBlockBricks.emplace_back(block);
//	block->SetContainer(this);
//
//}
