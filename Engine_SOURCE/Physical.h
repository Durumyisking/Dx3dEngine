#pragma once
#include "Component.h"
#include "PhysicalProperties.h"
#include "SimpleMath.h"

using namespace dru::enums;

struct Geometry
{
    Geometry(eGeometryType geometryType, dru::math::Vector3 boxHalfSize)
        : eGeomType(eGeometryType::BOX)
    {
        if (eGeometryType::BOX == geometryType)
        {
            boxGeom = PxBoxGeometry(dru::convert::Vector3ToPxVec3(boxHalfSize));
        }
    }

    Geometry(eGeometryType geometryType, float radius, float halfHeight)
        : eGeomType(eGeometryType::CAPSULE)
    {
        if (eGeometryType::CAPSULE == geometryType)
        {
            capsuleGeom = PxCapsuleGeometry(radius, halfHeight);
        }
    }

    Geometry(eGeometryType geometryType, float radius)
        : eGeomType(eGeometryType::SPHERE)
    {
        if (eGeometryType::SPHERE == geometryType)
        {
            sphereGeom = PxSphereGeometry(radius);
        }
    }


    Geometry(eGeometryType geometryType)
        : eGeomType(eGeometryType::PLANE)
    {
        // RigidStatic�� ��,
        if (eGeometryType::PLANE == geometryType)
        {
            planeGeom = PxPlaneGeometry();
        }
    }

    PxBoxGeometry boxGeom;
    PxCapsuleGeometry capsuleGeom;
    PxSphereGeometry sphereGeom;
    PxPlaneGeometry planeGeom;
    eGeometryType eGeomType;
};

namespace dru
{
    using namespace math;

    class Physical : public dru::Component
    {
    public:
        Physical();
        virtual ~Physical();

    public:
        virtual void Initialize();
        virtual void InitialPhysics(eActorType actorType, eGeometryType geometryType, Vector3 geometrySize, MassProperties massProperties = MassProperties());
        virtual void Update();
        virtual void FixedUpdate();
        virtual void Render();

    public:
        eActorType                  GetActorType()     const { return mActorType; }
        eGeometryType               GetGeometryType()  const { return mGeometryType; }
        PxShape*                    GetShape()         const { return mShape; }
        const Vector3&              GetGeometrySize()  const { return mSize; }
        std::shared_ptr<Geometry> GetGeometries()    const { return mGeometry; }
        PxActor*                    GetActor()         const { return mActor; }

        template<typename T>
        inline T* GetActor() const
        {
            T* actor = mActor->is<T>();
            assert(actor);
            return actor;
        }

        void AddActorToPxScene();
        void RemoveActorToPxScene();

    private:
        void createBoxGeometry(eGeometryType geometryType, const Vector3& boxSize);
        void createCapsuleGeometry(eGeometryType geometryType, float radius, float halfHeight);
        void createPlaneGeometry(eGeometryType geometryType);
        void createSphereGeometry(eGeometryType geometryType, float radius);

    private:
        void createPhysicsProperties(const MassProperties& massProperties = MassProperties());
        void createGeometry(eGeometryType geometryType, const Vector3& shapeSize); // ���ʹ� ���Ϸ� �޾��� ������Ʈ��(shape)
        void createUniversalShape(); // �������� ��� ������ ������Ʈ�� 
        void createShape();
        void createActor();
        void initializeActor();



    private:
        eActorType                      mActorType;
        eGeometryType                   mGeometryType;

        Vector3                         mSize;
        PxActor*                        mActor;
        // PxActor�� �������� �ùķ��̼��� ������ �� �ִ� ��ü���̴�.
        // ��׵��� PxScene���� ���� �ùķ��̼ǿ� �����Ѵ�.
        /*
            �ֿ� ��� Ŭ����

            PxRigidActor
            - �츮�� �����ϴ� ��ü�� ����, ���� �ùķ����� �ȿ��� ����, �ӵ�, ȸ��, �߷µ ������ �޾� �����δ�.
                RigidDynamic - �̵� ������ ���� ��ü�̴�.
                RigidStatic - ������ ��ü (��, �ٴ�) �̴�.

            PxArticulation
            - ���� ����� ������ ��ü

            PxParticleFluid
            - ���� ����� ��ü �ùķ��̼� ����        
        */

        PxShape*                        mShape;

        std::shared_ptr<PhysicalProperties> mProperties;
        std::shared_ptr<Geometry>        mGeometry;

    };
}

