#include "PhysXCollider.h"
#include "GameObj.h"
#include "PhysicsMgr.h"

namespace dru
{
	PhysXCollider::PhysXCollider()
		: Component(eComponentType::Collider)
		, mCallback(nullptr)
		, mPhysical(nullptr)
	{
	}
	PhysXCollider::~PhysXCollider()
	{
	}
	void PhysXCollider::Initialize()
	{
		// 렌더링할 물체의 모양을 가진 콜라이더
		mPhysical = GetOwner()->GetComponent<Physical>();

		if (mPhysical)
		{
			createDebugGeometry(mPhysical->GetGeometries());
		}

		mCallback = PhysicsMgr::GetInstance()->GetDispatcher()->GetSimulationCallback();
	}
	void PhysXCollider::update()
	{
	}
	void PhysXCollider::fixedUpdate()
	{
	}
	void PhysXCollider::render()
	{
	}
	void PhysXCollider::OnCollision()
	{
	}
	void PhysXCollider::createDebugGeometry(std::shared_ptr<Geometries> geometries)
	{
		switch (geometries->eGeomType)
		{
		case eGeometryType::BOX:
		{
			const PxBoxGeometry& boxGeom = static_cast<const PxBoxGeometry&>(geometries->boxGeom);
			Vector3 vHalfSize = convert::PxVec3ToVector3(boxGeom.halfExtents);
			createDebugBox(vHalfSize);
		}
		break;

		case eGeometryType::CAPSULE:
		{
			const PxCapsuleGeometry& capsuleGeom = static_cast<const PxCapsuleGeometry&>(geometries->capsuleGeom);
			float fRadius = capsuleGeom.radius;
			float fHalfHeight = capsuleGeom.halfHeight;
			createDebugCapsule(fRadius, fHalfHeight);
		}
		break;

		case eGeometryType::PLANE:
			break;
		}
	}
	void PhysXCollider::createDebugBox(math::Vector3 halfSize)
	{
		//std::shared_ptr<Mesh> mesh = Resources::Find<Mesh>(L"Cubemesh");

		//std::shared_ptr<Material> material = Resources::Find<Material>(L"DebugGeometryMaterial");

		//GetDebugRenderer()->SetMaterial(material);
		//GetDebugRenderer()->SetMesh(mesh);
	}
	void PhysXCollider::createDebugCapsule(float radius, float halfHeight)
	{
		//auto [vVertices, vIndices] = Vertex::CreateHemisphereVerticesAndIndices(radius, halfHeight);
		//shared_ptr<Mesh> pMesh = make_shared<Mesh>();
		//pMesh->Init(vVertices, vIndices);
		//shared_ptr<Material> pMaterial = GET_SINGLE(Resources)->Get<Material>(L"DebugGeometry");

		//GetDebugRenderer()->SetMaterial(pMaterial);
		//GetDebugRenderer()->SetMesh(pMesh);	
	}
}