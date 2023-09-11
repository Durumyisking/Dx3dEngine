#include "PhysXCollider.h"
#include "GameObj.h"
#include "PhysicsMgr.h"
#include "MeshRenderer.h"
#include "Transform.h"


PhysXCollider::PhysXCollider()
	: Component(eComponentType::Collider)
	, mCallback(nullptr)
	, mPhysical(nullptr)
	, mPxTransform{}
	, mRaycastHit{}
	, mSweepHit	 {}
	, mOverlapHit{}
	, mFilterData{}
	, mRayMaxDist(5.f)
	, mRayMaxHit (1)
{
}
PhysXCollider::~PhysXCollider()
{
}
void PhysXCollider::Initialize()
{
	mPxTransform = GetOwner()->GetComponent<Transform>()->GetPxTransform();
	mFilterData.word0 = 1 << static_cast<unsigned __int32>(GetOwner()->GetLayerType()); // 충돌 레이어 정해주는듯

	// 렌더링할 물체의 모양을 가진 콜라이더
	mPhysical = GetOwner()->GetComponent<Physical>();

	if (mPhysical)
	{
		createDebugGeometry(mPhysical->GetGeometries());
		mPhysical->GetShape()->setSimulationFilterData(mFilterData);
	}
	//mCallback = PhysicsMgr::GetInstance()->GetDispatcher()->GetSimulationCallback();
}
void PhysXCollider::Update()
{
	if (mPhysical)
	{
		mPhysical->GetShape()->setSimulationFilterData(mFilterData);
	}
}
void PhysXCollider::FixedUpdate()
{
	
	PxShape* shape = mPhysical->GetShape();
	PxGeometryType::Enum geoType = shape->getGeometryType();
	eColliderType colType = eColliderType::End;
	PxVec3 scale = {};

	switch (geoType)
	{
	case physx::PxGeometryType::eSPHERE:
		scale.x = shape->getGeometry().sphere().radius;
		colType = eColliderType::Sphere;
		break;
	case physx::PxGeometryType::ePLANE:
		break;
	case physx::PxGeometryType::eCAPSULE:
		scale.x = shape->getGeometry().capsule().radius;
		scale.y = shape->getGeometry().capsule().halfHeight;
		scale.z = shape->getGeometry().capsule().radius;
		colType = eColliderType::Capsule;
		break;
	case physx::PxGeometryType::eBOX:
		scale = shape->getGeometry().box().halfExtents;
		colType = eColliderType::Box;
		break;
	case physx::PxGeometryType::eCONVEXMESH:
		break;
	case physx::PxGeometryType::eTRIANGLEMESH:
		break;
	case physx::PxGeometryType::eHEIGHTFIELD:
		break;
	case physx::PxGeometryType::eGEOMETRY_COUNT:
		break;
	case physx::PxGeometryType::eINVALID:
		break;
	default:
		break;
	}
	scale *= 2.f;
	scale += PxVec3(0.00001f, 0.00001f, 0.00001f);
	Vector3 position = GetOwner()->GetComponent<Transform>()->GetWorldPosition();

	DebugMesh meshAttribute = {};
	meshAttribute.position = Vector3(position.x, position.y, position.z);
	meshAttribute.scale = convert::PxVec3ToVector3(scale);
	meshAttribute.type = colType;

	renderer::debugMeshes.push_back(meshAttribute);
}
void PhysXCollider::Render()
{
}
void PhysXCollider::OnCollisionEnter(PhysXCollider* otherCollider)
{
	GetOwner()->OnCollisionEnter(otherCollider->GetOwner());

	if (otherCollider->GetOwner()->GetLayerType() == eLayerType::Platforms)
	{
		// 이부분에서 dynamicObject 의 Idle() 함수를 호출하면된다.
		// DynamicObject 의 Idle함수는 가상함수로 상속받은 객체 내부에서 구현하면된다.
	}
}

void PhysXCollider::OnCollisionExit(PhysXCollider* otherCollider)
{
}

void PhysXCollider::OnTriggerEnter(PhysXCollider* otherCollider)
{
	GetOwner()->OnTriggerEnter(otherCollider->GetOwner());
}
void PhysXCollider::OnTriggerStay(PhysXCollider* otherCollider)
{
}
void PhysXCollider::OnTriggerExit(PhysXCollider* otherCollider)
{
	GetOwner()->OnTriggerExit(otherCollider->GetOwner());
}

bool PhysXCollider::Raycast(const Vector3& origin, const Vector3& dir, GameObj* gameObject, float maxDistance)
{
	Physical* physical = gameObject->GetComponent<Physical>();
	if (!physical)
	{
		return false;
	}
	eGeometryType geometryType = physical->GetGeometryType();
	PxTransform pxTransform = gameObject->GetComponent<Transform>()->GetPxTransform();

	switch (geometryType)
	{
	case eGeometryType::Box:
	{
		PxBoxGeometry boxGeom = physical->GetGeometries()->boxGeom;
		bool bResult = PxGeometryQuery::raycast(
			convert::Vector3ToPxVec3(origin),
			convert::Vector3ToPxVec3(dir),
			boxGeom, pxTransform,
			maxDistance,
			PxHitFlag::ePOSITION | PxHitFlag::eDEFAULT,
			mRayMaxHit,
			&mRaycastHit);

		return bResult;
	}
	break;

	case eGeometryType::Capsule:
	{
		PxCapsuleGeometry capsuleGeom = physical->GetGeometries()->capsuleGeom;
		bool bResult = PxGeometryQuery::raycast(
			convert::Vector3ToPxVec3(origin),
			convert::Vector3ToPxVec3(dir),
			capsuleGeom, pxTransform,
			maxDistance,
			PxHitFlag::ePOSITION | PxHitFlag::eDEFAULT,
			mRayMaxHit,
			&mRaycastHit);

		return bResult;
	}
	break;

	case eGeometryType::Sphere:
	{
		PxSphereGeometry sphereGeom = physical->GetGeometries()->sphereGeom;

		bool bResult = PxGeometryQuery::raycast(
			convert::Vector3ToPxVec3(origin),
			convert::Vector3ToPxVec3(dir),
			sphereGeom, pxTransform,
			maxDistance,
			PxHitFlag::ePOSITION | PxHitFlag::eDEFAULT,
			mRayMaxHit,
			&mRaycastHit);

		return bResult;
	}
	break;
	}

	return false;
}

void PhysXCollider::createDebugGeometry(std::shared_ptr<Geometry> geometries)
{
	switch (geometries->eGeomType)
	{
	case eGeometryType::Box:
	{
		const PxBoxGeometry& boxGeom = static_cast<const PxBoxGeometry&>(geometries->boxGeom);
		Vector3 HalfSize = convert::PxVec3ToVector3(boxGeom.halfExtents);
		createDebugBox(HalfSize);
	}
	break;

	case eGeometryType::Capsule:
	{
		const PxCapsuleGeometry& capsuleGeom = static_cast<const PxCapsuleGeometry&>(geometries->capsuleGeom);
		float Radius = capsuleGeom.radius;
		float HalfHeight = capsuleGeom.halfHeight;
		createDebugCapsule(Radius, HalfHeight);
	}
	break;

	case eGeometryType::Sphere:
	{
		const PxSphereGeometry& sphereGeom = static_cast<const PxSphereGeometry&>(geometries->sphereGeom);
		float Radius = sphereGeom.radius;
		createDebugSphere(Radius);
	}
	break;

	case eGeometryType::Plane:
		break;
	}
}
void PhysXCollider::createDebugBox(math::Vector3 halfSize)
{		
	if (!GetOwner()->GetComponent<MeshRenderer>()->GetMesh())
	{
		Mesh* mesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Cubemesh");
		GetOwner()->GetComponent<MeshRenderer>()->SetMesh(mesh);
	}

	if (!GetOwner()->GetComponent<MeshRenderer>()->GetMaterial())
	{
		Material* material = GETSINGLE(ResourceMgr)->Find<Material>(L"DebugGeometryMaterial");
		GetOwner()->GetComponent<MeshRenderer>()->SetMaterial(material);
	}

}
void PhysXCollider::createDebugCapsule(float radius, float halfHeight)
{
	//auto [vVertices, vIndices] = Vertex::CreateHemisphereVerticesAndIndices(radius, halfHeight);
	//shared_ptr<GUIMesh> pMesh = make_shared<GUIMesh>();
	//pMesh->Initialize(vVertices, vIndices);
	//shared_ptr<GUIMaterial> pMaterial = GET_SINGLE(Resources)->Get<GUIMaterial>(L"DebugGeometry");

	//GetDebugRenderer()->SetMaterial(pMaterial);
	//GetDebugRenderer()->SetMesh(pMesh);	
}
void PhysXCollider::createDebugSphere(float radius)
{
	if (!GetOwner()->GetComponent<MeshRenderer>()->GetMesh())
	{
		Mesh* mesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Spheremesh");
		GetOwner()->GetComponent<MeshRenderer>()->SetMesh(mesh);
	}

	if (!GetOwner()->GetComponent<MeshRenderer>()->GetMaterial())
	{
		Material* material = GETSINGLE(ResourceMgr)->Find<Material>(L"DebugGeometryMaterial");
		GetOwner()->GetComponent<MeshRenderer>()->SetMaterial(material);
	}
}