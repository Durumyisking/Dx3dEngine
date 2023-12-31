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
    void OnTriggerPersist(PhysXCollider* otherCollider);
    void OnTriggerExit(PhysXCollider* otherCollider);

    bool Raycast(const Vector3& origin, const Vector3& dir, GameObj* gameObject, float maxDistance);
    Vector3 ComputePenetration(GameObj* gameObject);
    Vector3 ComputePenetration_Direction(GameObj* gameObject);


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

    PxVec3   mPenetDir;
    float    mPenetDepth;



};

