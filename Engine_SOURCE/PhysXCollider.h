#pragma once
#include "Component.h"
#include "Physical.h"
#include "PxEventCallback.h"



class PhysXCollider :
    public Component
{
public:
    PhysXCollider();
    virtual ~PhysXCollider();

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void FixedUpdate() override;
    virtual void Render() override;


//        virtual void OnCollision();

    void OnCollisionEnter(PhysXCollider* otherCollider);
    void OnCollisionExit(PhysXCollider* otherCollider);

    void OnTriggerEnter(PhysXCollider* otherCollider);
    void OnTriggerStay(PhysXCollider* otherCollider);
    void OnTriggerExit(PhysXCollider* otherCollider);

    bool Raycast(const Vector3& origin, const Vector3& dir, GameObj* gameObject, float maxDistance);


    PxFilterData& GetFilterData()  { return mFilterData; }

private:
    void createDebugGeometry(std::shared_ptr<Geometry> geometries);
    void createDebugBox(math::Vector3 halfSize);
    void createDebugCapsule(float radius, float halfHeight);
    void createDebugSphere(float radius);

public:
    PxEventCallback* mCallback;
    Physical* mPhysical;

    PxTransform mPxTransform;

    PxRaycastHit     mRaycastHit;
    PxSweepHit       mSweepHit;
    PxOverlapHit     mOverlapHit;
    PxFilterData     mFilterData;

    float  mRayMaxDist;
    UINT   mRayMaxHit;


};

