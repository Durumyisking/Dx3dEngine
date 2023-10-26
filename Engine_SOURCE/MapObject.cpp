#include "MapObject.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

MapObject::MapObject(std::wstring modelName, Vector3 physicalScale, Vector3 colliderOffectPos)
	: GameObj()
	, mModelName(modelName)
	, mbPhysical(true)
	, mPhysicalScale(physicalScale)
	, mGeometryType(eGeometryType::Box)
	, mActorType(eActorType::Static)
	, mColliderOffsetPos(mColliderOffsetPos)
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
	mObjectTypeName = "MapObject";
}

MapObject::MapObject(const MapObject& Obj)
	:GameObj(Obj)
	, mModelName(Obj.mModelName)
	, mbPhysical(Obj.mbPhysical)
	, mPhysicalScale(Obj.mPhysicalScale)
	, mGeometryType(Obj.mGeometryType)
	, mActorType(Obj.mActorType)
	, mColliderOffsetPos(Obj.mColliderOffsetPos)
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
}

MapObject::~MapObject()
{
}

MapObject* MapObject::Clone() const
{
	return new MapObject(*this);
}

void MapObject::Save(FILE* File)
{
	GameObj::Save(File);


	MeshRenderer* mrd = GetComponent<MeshRenderer>();

	if (mModelName.empty())
	{
		if (mrd->GetModel() != nullptr)
		{
			mModelName = mrd->GetModel()->GetName();
		}
	}

	if (!mbPhysical)
	{
		if (GetComponent<Physical>() != nullptr)
		{
			mbPhysical = true;
			Physical* physical = GetComponent<Physical>();
			mActorType = physical->GetActorType();
			mPhysicalScale = GetComponent<Transform>()->GetScale();
		}
	}

	// 이름 저장
	int numWChars = (int)mModelName.length();

	fwrite(&numWChars, sizeof(int), 1, File);
	fwrite(mModelName.c_str(), sizeof(wchar_t), numWChars, File);

	fwrite(&mbPhysical, sizeof(bool), 1, File);
	fwrite(&mActorType, sizeof(eActorType), 1, File);
	fwrite(&mPhysicalScale, sizeof(math::Vector3), 1, File);
}

void MapObject::Load(FILE* File)
{
	GameObj::Load(File);

	int numWChars = 0;

	fread(&numWChars, sizeof(int), 1, File);

	mModelName.resize(numWChars);
	fread(&mModelName[0], sizeof(wchar_t), numWChars, File);

	fread(&mbPhysical, sizeof(bool), 1, File);
	fread(&mActorType, sizeof(eActorType), 1, File);
	fread(&mPhysicalScale, sizeof(math::Vector3), 1, File);
}

void MapObject::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(mModelName);
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetMaterial(model->GetMaterial());
	mr->SetModel(model);

	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	if (mbPhysical)
	{
		Physical* physical = AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(mActorType, mGeometryType, mPhysicalScale);

		PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
		rigid->RemoveGravity();

		AddComponent<PhysXCollider>(eComponentType::Collider);
	}

	GameObj::Initialize();
}

void MapObject::Update()
{
	GameObj::Update();
}

void MapObject::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void MapObject::Render()
{
	GameObj::Render();
}

void MapObject::OnCollisionEnter(GameObj* gameObject)
{
}

void MapObject::OnTriggerEnter(GameObj* gameObject)
{
}

void MapObject::OnTriggerPersist(GameObj* gameObject)
{
}

void MapObject::OnTriggerExit(GameObj* gameObject)
{
}
