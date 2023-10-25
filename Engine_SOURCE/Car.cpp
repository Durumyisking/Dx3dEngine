#include "Car.h"
#include "Physical.h"
#include "Model.h"
#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"

Car::Car()
	: GameObj()
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
	mObjectTypeName = "Car";
}

Car::Car(const Car& Obj)
	:GameObj(Obj)
{
	assert(AddComponent<MeshRenderer>(eComponentType::MeshRenderer));
}

Car::~Car()
{
}

Car* Car::Clone() const
{
	return new Car(*this);
}

void Car::Initialize()
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"Car");
	assert(model);

	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->SetModel(model);

	mr->SetMaterialByKey(L"TaxiCar_0Material", 0);
	mr->SetMaterialByKey(L"TaxiCar_0Material", 1);
	mr->SetMaterialByKey(L"TaxiCar_0Material", 2);
	mr->SetMaterialByKey(L"TaxiCar_0Material", 3);
	mr->SetMaterialByKey(L"TaxiCar_1Material", 4);
	mr->SetMaterialByKey(L"TaxiCar_0Material", 5);
	mr->SetMaterialByKey(L"TaxiCar_0Material", 6);
	mr->SetMaterialByKey(L"TaxiCar_0Material", 7);
	mr->SetMaterialByKey(L"TaxiCar_0Material", 8);


	this->GetComponent<Transform>()->SetOffsetScale(0.005f);

	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Box, { 10.f, 50.f, 10.f });

	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	rigid->RemoveGravity();

	AddComponent<PhysXCollider>(eComponentType::Collider);

	this->GetComponent<Transform>()->SetPhysicalRotation(Vector3(90.f, 0.f, 0.f));

	GameObj::Initialize();
}

void Car::Update()
{
	GameObj::Update();
}

void Car::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void Car::OnCollisionEnter(GameObj* gameObject)
{
}

void Car::OnTriggerEnter(GameObj* gameObject)
{
}

void Car::OnTriggerPersist(GameObj* gameObject)
{
}

void Car::OnTriggerExit(GameObj* gameObject)
{
}
