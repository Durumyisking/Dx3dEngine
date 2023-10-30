#include "ModelObj.h"
#include "Physical.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

ModelObj::ModelObj()
	: GameObj()
	, mModelName{}
	, mbPhysical(false)
	, mGeomType(eGeometryType::End)
	, mActorType(eActorType::End)
	, mPhysicalScale{}
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);

	mObjectTypeName = "ModelObj";
}

ModelObj::ModelObj(const ModelObj& Obj)
	: GameObj(Obj)
	, mModelName(Obj.mModelName)
	, mbPhysical(Obj.mbPhysical)
	, mGeomType(Obj.mGeomType)
	, mActorType(Obj.mActorType)
	, mPhysicalScale(Obj.mPhysicalScale)
{
	this->AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
}

ModelObj::~ModelObj()
{
}

ModelObj* ModelObj::Clone() const
{
	return new ModelObj(*this);
}

void ModelObj::Save(FILE* File)
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
		mGeomType = physical->GetGeometryType();
		mActorType = physical->GetActorType();
		mPhysicalScale = physical->GetGeometrySize();
	}
	
	// 이름 저장
	int numWChars = (int)mModelName.length();

	fwrite(&numWChars, sizeof(int), 1, File);
	fwrite(mModelName.c_str(), sizeof(wchar_t), numWChars, File);

	fwrite(&mbPhysical, sizeof(bool), 1, File);
	fwrite(&mGeomType, sizeof(eGeometryType), 1, File);
	fwrite(&mActorType, sizeof(eActorType), 1, File);
	fwrite(&mPhysicalScale, sizeof(math::Vector3), 1, File);
}

void ModelObj::Load(FILE* File)
{
	GameObj::Load(File);

	int numWChars = 0;

	fread(&numWChars, sizeof(int), 1, File);

	mModelName.resize(numWChars);
	fread(&mModelName[0], sizeof(wchar_t), numWChars, File);

	fread(&mbPhysical, sizeof(bool), 1, File);
	fread(&mGeomType, sizeof(eGeometryType), 1, File);
	fread(&mActorType, sizeof(eActorType), 1, File);
	fread(&mPhysicalScale, sizeof(math::Vector3), 1, File);
}

void ModelObj::Initialize()
{
	if (!mModelName.empty())
	{
		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(mModelName);

		if (model == nullptr)
			return;

		MeshRenderer* mr = GetComponent<MeshRenderer>();
		mr->ForceSetMaterial(model->GetLastMaterial());
		mr->SetModel(model);

		this->GetComponent<Transform>()->SetOffsetScale(0.005f);
	}

	if (mbPhysical)
	{
		Physical* physical = AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(mActorType, mGeomType, mPhysicalScale);
		Transform* tr = GetComponent<Transform>();
		tr->SetPhysicalRotation(tr->GetRotation());

		PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
		rigid->RemoveGravity();

		AddComponent<PhysXCollider>(eComponentType::Collider);
	}


	GameObj::Initialize();
}

void ModelObj::Update()
{
	GameObj::Update();
}

void ModelObj::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void ModelObj::Render()
{
	GameObj::Render();
}
