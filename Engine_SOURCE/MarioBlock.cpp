#include "MarioBlock.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"

MarioBlock::MarioBlock()
	:GameObj()
{

}

MarioBlock::~MarioBlock()
{

}

void MarioBlock::Initialize()
{

	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));


	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CityWorldHomeBuilding002");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"GlassBuildingWallMaterial",0);
	mr->SetMaterialByKey(L"RoofConcrete00Material",1);
	mr->SetMaterialByKey(L"RoofConcrete01Material",2);
	mr->SetMaterialByKey(L"WallGlassPaintedSteelMaterial",4);


	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	GameObj::Initialize();
}

void MarioBlock::Update()
{
	GameObj::Update();
}

void MarioBlock::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void MarioBlock::OnCollisionEnter(GameObj* gameObject)
{
}

void MarioBlock::OnTriggerEnter(GameObj* gameObject)
{
}

void MarioBlock::OnTriggerStay(GameObj* gameObject)
{
}

void MarioBlock::OnTriggerExit(GameObj* gameObject)
{
}
