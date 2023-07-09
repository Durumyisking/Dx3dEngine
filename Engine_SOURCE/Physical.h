#pragma once
#include "Component.h"
#include "PhysicalProperties.h"

using namespace dru::enums;

struct Geometries
{
    Geometries(eGeometryType geometryType, dru::math::Vector3 boxHalfSize)
        : eGeomType(eGeometryType::BOX)
    {
        if (eGeometryType::BOX == geometryType)
        {
            boxGeom = PxBoxGeometry(dru::convert::Vector3ToPxVec3(boxHalfSize));
        }
    }

    Geometries(eGeometryType geometryType, float radius, float halfHeight)
        : eGeomType(eGeometryType::CAPSULE)
    {
        if (eGeometryType::CAPSULE == geometryType)
        {
            capsuleGeom = PxCapsuleGeometry(radius, halfHeight);
        }
    }

    Geometries(eGeometryType geometryType)
        : eGeomType(eGeometryType::PLANE)
    {
        // RigidStatic¿œ ãö,
        if (eGeometryType::PLANE == geometryType)
        {
            planeGeom = PxPlaneGeometry();
        }
    }

    PxBoxGeometry boxGeom;
    PxCapsuleGeometry capsuleGeom;
    PxPlaneGeometry planeGeom;
    eGeometryType eGeomType;
};

namespace dru
{

    class Physical : public dru::Component
    {
    public:
        Physical();
        virtual ~Physical();

    public:
        virtual void Initialize();
        virtual void InitialPhysics(eActorType actorType, eGeometryType geometryType, dru::math::Vector3 geometrySize, MassProperties massProperties = MassProperties());
        virtual void update();
        virtual void fixedUpdate();
        virtual void render();

    public:
        PxController* GetController() { return mController; }
        PxActor* GetActor() { return mActor; }
        eActorType             GetActorType() { return mActorType; }
        eGeometryType          GetGeometryType() { return mGeometryType; }

        std::shared_ptr<Geometries> GetGeometries() { return mGeometries; }

    private:
        void          CreateBoxGeometry(eGeometryType geometryType, dru::math::Vector3 boxSize);
        void          CreateCapsuleGeometry(eGeometryType geometryType, float radius, float halfHeight);
        void          CreatePlaneGeometry(eGeometryType geometryType);

    private:
        void          CreatePhysicsProperties(MassProperties massProperties = MassProperties());
        void          CreateGeometry(eGeometryType geometryType, dru::math::Vector3 shapeSize);
        void          CreateShape();
        void          CreateActor();
        void          InitializeActor();
        void          CreateController();
        void          AddActor(PxActor* actor);

        void          ApplyShapeScale();



    private:
        eActorType                    mActorType;
        eGeometryType                 mGeometryType;

        dru::math::Vector3            mSize;

        PxActor* mActor;
        PxShape* mShape;
        PxController* mController;

        std::shared_ptr<PhysicalProperties> mProperties;
        std::shared_ptr<Geometries>        mGeometries;

        dru::Transform* mTransform;
    };
}

