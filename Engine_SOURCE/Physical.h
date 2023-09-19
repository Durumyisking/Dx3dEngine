#pragma once
#include "Component.h"
#include "PhysicalProperties.h"
#include "SimpleMath.h"

using namespace enums;

struct Geometry
{
    Geometry(eGeometryType geometryType, math::Vector3 boxHalfSize)
        : eGeomType(eGeometryType::Box)
    {
        if (eGeometryType::Box == geometryType)
        {
            boxGeom = PxBoxGeometry(convert::Vector3ToPxVec3(boxHalfSize));
        }
    }

    Geometry(eGeometryType geometryType, float radius, float halfHeight)
        : eGeomType(eGeometryType::Capsule)
    {
        if (eGeometryType::Capsule == geometryType)
        {
            capsuleGeom = PxCapsuleGeometry(radius, halfHeight);
        }
    }

    Geometry(eGeometryType geometryType, float radius)
        : eGeomType(eGeometryType::Sphere)
    {
        if (eGeometryType::Sphere == geometryType)
        {
            sphereGeom = PxSphereGeometry(radius);
        }
    }

    Geometry(eGeometryType geometryType)
        : eGeomType(eGeometryType::Plane)
    {
        // RigidStatic일 떄,
        if (eGeometryType::Plane == geometryType)
        {
            planeGeom = PxPlaneGeometry();
        }
    }

    Geometry(eGeometryType geometryType, PxConvexMesh* convexMesh, math::Vector3 scale)
        : eGeomType(eGeometryType::ConvexMesh)
    {
        if (eGeometryType::ConvexMesh == geometryType)
        {
            PxMeshScale meshScale(PxVec3(scale.x, scale.y, scale.z));
            convexMeshGeom = PxConvexMeshGeometry(convexMesh, meshScale);
        }
    }

        PxBoxGeometry boxGeom;
        PxCapsuleGeometry capsuleGeom;
        PxSphereGeometry sphereGeom;
        PxPlaneGeometry planeGeom;
        PxConvexMeshGeometry convexMeshGeom;
        eGeometryType eGeomType;


};



using namespace math;

class Physical : public Component
{
public:
    Physical();
    virtual ~Physical();

public:
    virtual void Initialize();
    virtual void InitialDefaultProperties(eActorType actorType, eGeometryType geometryType, Vector3 geometrySize, MassProperties massProperties = MassProperties());
    virtual void InitialConvexMeshProperties(eActorType actorType, Vector3 geometrySize, MassProperties massProperties = MassProperties());
    PxConvexMesh* MakeConvexObject();
    virtual void Update();
    virtual void FixedUpdate();
    virtual void Render();

public:
    eActorType                  GetActorType()     const { return mActorType; }
    eGeometryType               GetGeometryType()  const { return mGeometryType; }
    PxShape*                    GetShape()         const { return mShape; }
    const Vector3&              GetGeometrySize()  const { return mSize; }
    std::shared_ptr<Geometry>   GetGeometries()    const { return mGeometry; }
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

    void SetGeometrySize(const Vector3& newSize);
    void SetActorType(eActorType type) { mActorType = type; }

private:
    void createBoxGeometry(eGeometryType geometryType, const Vector3& boxSize);
    void createCapsuleGeometry(eGeometryType geometryType, float radius, float halfHeight);
    void createPlaneGeometry(eGeometryType geometryType);
    void createSphereGeometry(eGeometryType geometryType, float radius);
    void createConvexMeshGeometry(eGeometryType geometryType, PxConvexMesh* convexMesh, const Vector3& mScale);

private:
    void createPhysicsProperties(const MassProperties& massProperties = MassProperties());
    void createGeometry(eGeometryType geometryType, const Vector3& shapeSize); // 액터당 단일로 달아줄 지오메트리(shape)
    void createUniversalShape(); // 공용으로 사용 가능한 지오메트리 
    void createShape();
    void createConvexShape(PxConvexMeshGeometry convexMesh);
    void createActor();
    void initializeActor();



private:
    eActorType                      mActorType;
    eGeometryType                   mGeometryType;

    Vector3                         mSize;
    PxActor*                        mActor;
    // PxActor은 물리엔진 시뮬레이션을 적용할 수 있는 객체들이다.
    // 얘네들은 PxScene에서 물리 시뮬레이션에 참여한다.
    /*
        주요 상속 클래스

        PxRigidActor
        - 우리가 생각하는 강체와 같다, 물리 시뮬레이터 안에서 질량, 속도, 회전, 중력등에 영향을 받아 움직인다.
            RigidDynamic - 이동 가능한 동적 객체이다.
            RigidStatic - 정적인 객체 (벽, 바닥) 이다.

        PxArticulation
        - 관절 기반의 복잡한 물체

        PxParticleFluid
        - 입자 기반의 유체 시뮬레이션 수행        
    */

    PxShape*                        mShape;

    std::shared_ptr<PhysicalProperties> mProperties;
    std::shared_ptr<Geometry>        mGeometry;

};
