#pragma once
#include "Component.h"

namespace dru
{
    class Physical;
    class PhysXRigidBody :
        public Component
    {
public:
    PhysXRigidBody();
    virtual ~PhysXRigidBody();

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void FixedUpdate() override;
    virtual void Render() override;

    public:
        // for kinematic actors
        FORCEINLINE bool        IsAccelerating() const { return fabs(mVelocity.Length()) > 0.f; }
        FORCEINLINE bool        IsGravityApplied() const { return mGravityApplied; }
        FORCEINLINE const math::Vector3& GetGravityAccel()  const { return mGravityAccel; }
        FORCEINLINE const math::Vector3& GetVelocity() const { return mVelocity; }
        FORCEINLINE void        ApplyGravity() { mGravityApplied = true; }
        FORCEINLINE void        RemoveGravity() { mGravityApplied = false; }
        FORCEINLINE void        AddVelocity(const math::Vector3& velocity) { mVelocity += velocity; }
        FORCEINLINE void        SetVelocity(const math::Vector3& velocity) { mVelocity = velocity; }


    public:
        // for kinematic actors
        float GetVelocity(AXIS eAxis);
        void  SetVelocity(AXIS eAxis, float velocity);
        void  AddVelocity(AXIS eAxis, float velocity);
        void  ReserveSpeedForSeconds(const math::Vector3& velocity, float duration);

    public:
        // for dynamic actors
        void SetMassForDynamic(float mass);
        void SetLinearVelocityForDynamic(const PxVec3& linearVelocity);
        void SetLinearVelocityForDynamic(AXIS eAxis, float valVelocity);
        void SetAngularVelocityForDynamic(const PxVec3& angularVelocity);
        void AddForceForDynamic(const PxVec3& force, PxForceMode::Enum eForceMode);
        void SetLinearDamping(float damping);
        void SetAngularDamping(float damping);
        void SetLinearMaxVelocityForDynamic(float maxVelocity);
        void SetAngularMaxVelocityForDynamic(float maxVelocity);
        void ApplyGravityForDynamic();
        void RemoveGravityForDynamic();
        void SetRotationZForDynamic(float rotationZ);

    private:
        Physical* mPhysical;

        bool mGravityApplied;
        math::Vector3 mGravityAccel;
        math::Vector3 mVelocity;
        math::Vector3 mMaxVelocity;

        math::Vector3 mReserveVelocity;
        Timer mReserveTimer;
    };
}
