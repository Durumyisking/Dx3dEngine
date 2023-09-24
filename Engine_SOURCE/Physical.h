#pragma once
#include "Component.h"
#include "PhysicalProperties.h"
#include "SimpleMath.h"

using namespace enums;

struct Geometry
{
    Geometry(const Geometry& geom)
        : eGeomType(geom.eGeomType)
        , boxGeom(geom.boxGeom)
        , capsuleGeom(geom.capsuleGeom)
        , sphereGeom(geom.sphereGeom)
        , planeGeom(geom.planeGeom)
    {
       
    }
    Geometry()
        : eGeomType(eGeometryType::End)
    {

    }

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
            // RigidStatic�� ��,
            if (eGeometryType::Plane == geometryType)
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



using namespace math;

class Physical : public Component
{
public:
    Physical();
    virtual ~Physical();

public:
    virtual void Initialize();
    virtual void InitialDefaultProperties(eActorType actorType, eGeometryType geometryType, Vector3 geometrySize, MassProperties massProperties = MassProperties());
    virtual void Update();
    virtual void FixedUpdate();
    virtual void Render();

public:
    eActorType                  GetActorType()     const { return mActorType; }
    eGeometryType               GetGeometryType()  const { return mGeometryType; }
    PxShape*                    GetShape()         const { return mMainShape; }
    const Vector3&              GetGeometrySize()  const { return mSize; }
    std::shared_ptr<Geometry>   GetGeometries()    const { return mMainGeometry; }
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
    void ShapesPause();

    void SetGeometrySize(const Vector3& newSize);
    void SetActorType(eActorType type) { mActorType = type; }

    std::shared_ptr<PhysicalProperties> GetProperties() const { return mProperties; }

    void CreateMainShape();
    void CreateMainShape(Vector3 localPos);

    void CreateSubShape(Vector3 relativePos, eGeometryType geomType, Vector3 geomSize, PxShapeFlag::Enum shapeFlag);
    std::vector<PxShape*> GetSubShapes() { return mSubShapes; }

private:
    Geometry createBoxGeometry(eGeometryType geometryType, const Vector3& boxSize);
    Geometry createCapsuleGeometry(eGeometryType geometryType, float radius, float halfHeight);
    Geometry createPlaneGeometry(eGeometryType geometryType);
    Geometry createSphereGeometry(eGeometryType geometryType, float radius);

private:
    void createPhysicsProperties(const MassProperties& massProperties = MassProperties());
    Geometry createGeometry( eGeometryType geometryType, const Vector3& shapeSize); // ���ʹ� �޾��� ������Ʈ��(shape)
    void createUniversalShape(); // �������� ��� ������ ������Ʈ�� 
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

    PxShape*                        mMainShape;
    std::vector<PxShape*>           mSubShapes;


    std::shared_ptr<PhysicalProperties> mProperties;
    std::shared_ptr<Geometry>        mMainGeometry;
    std::vector<std::shared_ptr<Geometry> >           mSubGeometries;

};
