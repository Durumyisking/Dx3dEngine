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
			mGeomType = physical->GetGeometryType();
			mActorType = physical->GetActorType();
			mPhysicalScale = GetComponent<Transform>()->GetScale();
		}
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
		// SetModel
		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(mModelName);
		MeshRenderer* mrd = GetComponent<MeshRenderer>();
		Material* mt = model->GetMaterial(0);

		if (mt == nullptr)
			mt = GETSINGLE(ResourceMgr)->Find<Material>(L"PhongMaterial");

		mrd->SetModel(model, mt);
	}

	if (mbPhysical)
	{
		Physical* physical = AddComponent<Physical>(eComponentType::Physical);

		switch (mGeomType)
		{
		case enums::eGeometryType::Box:
			physical->InitialDefaultProperties(mActorType, mGeomType, mPhysicalScale);
			break;
		case enums::eGeometryType::Capsule:
			physical->InitialDefaultProperties(mActorType, mGeomType, mPhysicalScale);
			break;
		case enums::eGeometryType::Sphere:
			physical->InitialDefaultProperties(mActorType, mGeomType, mPhysicalScale);
			break;
		case enums::eGeometryType::Plane:
			physical->InitialDefaultProperties(mActorType, mGeomType, mPhysicalScale);
			break;
		case enums::eGeometryType::ConvexMesh:
			physical->InitialConvexMeshProperties(mActorType, mPhysicalScale);
			break;
		case enums::eGeometryType::TriangleMesh:
			physical->InitialTriangleMeshProperties(mPhysicalScale);
			break;
		case enums::eGeometryType::End:
			break;
		default:
			break;
		}

		AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
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
