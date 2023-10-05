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
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
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

	// �̸� ����
	int Length = (int)(mModelName.length() * sizeof(wchar_t));

	fwrite(&Length, sizeof(int), 1, File);
	fwrite(mModelName.c_str(), sizeof(wchar_t), Length, File);

	fwrite(&mbPhysical, sizeof(bool), 1, File);
	fwrite(&mGeomType, sizeof(eGeometryType), 1, File);
	fwrite(&mActorType, sizeof(eActorType), 1, File);
	fwrite(&mPhysicalScale, sizeof(math::Vector3), 1, File);
}

void ModelObj::Load(FILE* File)
{
	GameObj::Load(File);

	int	Length = 0;
	wchar_t	Name[256] = {};

	fread(&Length, sizeof(int), 1, File);
	if (Length > 0)
	{
		int numWChars = Length / sizeof(wchar_t);

		mModelName.resize(numWChars);
		fread(&mModelName[0], sizeof(wchar_t), numWChars, File);
	}
	else
	{
		mModelName.clear();
	}

	fread(&mbPhysical, sizeof(bool), 1, File);
	fread(&mGeomType, sizeof(eGeometryType), 1, File);
	fread(&mActorType, sizeof(eActorType), 1, File);
	fread(&mPhysicalScale, sizeof(math::Vector3), 1, File);
}

void ModelObj::Initialize()
{
	GameObj::Initialize();

	if (mModelName.empty())
		return;

	// SetModel
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(mModelName);
	MeshRenderer* mrd = GetComponent<MeshRenderer>();
	Material* mt = model->GetMaterial(0);

	if (mt == nullptr)
		mt = GETSINGLE(ResourceMgr)->Find<Material>(L"PhongMaterial");

	mrd->SetModel(model, mt);

	if (!mbPhysical)
		return;


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

void ModelObj::FontRender()
{
	GameObj::FontRender();
}
