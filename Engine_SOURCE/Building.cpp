#include "Building.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

Building::Building()
	: GameObj()
	, mModelName(L"CityWorldHomeBuilding002")
	, mbPhysical(true)
	, mPhysicalScale(10.f, 50.f, 10.f)
	, mGeometryType(eGeometryType::Box)
	, mActorType(eActorType::Static)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mObjectTypeName = "Building";
}

Building::Building(const Building& Obj)
	:GameObj(Obj)
	, mModelName(Obj.mModelName)
	, mbPhysical(Obj.mbPhysical)
	, mPhysicalScale(Obj.mPhysicalScale)
	, mGeometryType(Obj.mGeometryType)
	, mActorType(Obj.mActorType)
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);

}

Building::~Building()
{

}

Building* Building::Clone() const
{
	return new Building(*this);
}

void Building::Save(FILE* File)
{
	GameObj::Save(File);

	MeshRenderer* mrd = GetComponent<MeshRenderer>();

	if (mrd->GetModel() != nullptr)
	{
		mModelName = mrd->GetModel()->GetName();
	}

	if (GetComponent<Physical>() != nullptr)
	{
		mbPhysical = true;
		Physical* physical = GetComponent<Physical>();
		mGeometryType = physical->GetGeometryType();
		mActorType = physical->GetActorType();
		mPhysicalScale = physical->GetGeometrySize();
	}

	// 이름 저장
	int numWChars = (int)mModelName.length();

	fwrite(&numWChars, sizeof(int), 1, File);
	fwrite(mModelName.c_str(), sizeof(wchar_t), numWChars, File);

	fwrite(&mbPhysical, sizeof(bool), 1, File);
	fwrite(&mGeometryType, sizeof(eGeometryType), 1, File);
	fwrite(&mActorType, sizeof(eActorType), 1, File);
	fwrite(&mPhysicalScale, sizeof(math::Vector3), 1, File);
}

void Building::Load(FILE* File)
{
	GameObj::Load(File);

	int numWChars = 0;

	fread(&numWChars, sizeof(int), 1, File);

	mModelName.resize(numWChars);
	fread(&mModelName[0], sizeof(wchar_t), numWChars, File);

	fread(&mbPhysical, sizeof(bool), 1, File);
	fread(&mGeometryType, sizeof(eGeometryType), 1, File);
	fread(&mActorType, sizeof(eActorType), 1, File);
	fread(&mPhysicalScale, sizeof(math::Vector3), 1, File);
}

void Building::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(mModelName);

	if (model == nullptr)
		return;

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->ForceSetMaterial(model->GetLastMaterial());
	mr->SetModel(model);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	if (mbPhysical)
	{
		Physical* physical = AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(mActorType, mGeometryType, mPhysicalScale);
		Transform* tr = GetComponent<Transform>();
		tr->SetPhysicalRotation(tr->GetRotation());

		PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
		rigid->RemoveGravity();

		AddComponent<PhysXCollider>(eComponentType::Collider);
	}

	GameObj::Initialize();
}

void Building::Update()
{
	GameObj::Update();
}

void Building::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void Building::Render()
{
	GameObj::Render();
}

void Building::OnCollisionEnter(GameObj* gameObject)
{
}

void Building::OnTriggerEnter(GameObj* gameObject)
{
}

void Building::OnTriggerPersist(GameObj* gameObject)
{
}

void Building::OnTriggerExit(GameObj* gameObject)
{
}
