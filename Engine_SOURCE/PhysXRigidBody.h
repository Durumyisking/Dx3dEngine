#pragma once
#include "Component.h"



class Physical;
class Transform;
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
	FORCEINLINE bool        IsGravityApplied() const { return mbGravityApplied; }
	FORCEINLINE const math::Vector3& GetGravityAccel()  const { return mGravityAccel; }
	FORCEINLINE const math::Vector3& GetVelocity() const { return mVelocity; }
	FORCEINLINE void        ApplyGravity() { mbGravityApplied = true; }
	FORCEINLINE void        RemoveGravity() { mbGravityApplied = false; }
	FORCEINLINE void        AddVelocity(const math::Vector3& velocity) { mVelocity += velocity; }
	FORCEINLINE void        SetVelocity(const math::Vector3& velocity) { mVelocity = velocity; }
	void                    SetVelocity(AXIS axis, const math::Vector3& velocity);


  FORCEINLINE void        AddForce(const math::Vector3& force) { mForce += force; }
  FORCEINLINE math::Vector3        GetForce() const { return mForce; }
  FORCEINLINE math::Vector3        SetMaxVelocity(const math::Vector3& velocity) { mMaxVelocity = velocity; }
  FORCEINLINE void                 SetMaxVelocity(const float& velocity) { mMaxVelocity = math::Vector3(velocity); }
  FORCEINLINE void                 SetMaxVelocity_Y(const float& velocityY) { mMaxVelocity.y = velocityY; }
  FORCEINLINE void                 SetMaxVelocity_XZ(const math::Vector2& velocityXZ) { mMaxVelocity.x = velocityXZ.x; mMaxVelocity.z = velocityXZ.y; }

  FORCEINLINE void					SetAirOn() { mbAirborn = true; }
  FORCEINLINE void					SetAirOff() { mbAirborn = false; }
  FORCEINLINE bool					IsOnAir() const { return mbAirborn ; }

public:
    // for kinematic actors
    float GetVelocity(AXIS eAxis);
    void  SetVelocity(AXIS eAxis, float velocity);
    void  AddVelocity(AXIS eAxis, float velocity);
    //void  ReserveSpeedForSeconds(const math::Vector3& velocity, float duration);


public:
	// for dynamic actors
	void SetMassForDynamic(float mass);
	void SetLinearVelocityForDynamic(const math::Vector3& linearVelocity);
	void SetLinearVelocityForDynamic(AXIS eAxis, float valVelocity);
	void SetAngularVelocityForDynamic(const math::Vector3& angularVelocity);
	void AddForceForDynamic(const math::Vector3& force, PxForceMode::Enum eForceMode);
	void SetLinearDamping(float damping);
	void SetAngularDamping(float damping);
	void SetLinearMaxVelocityForDynamic(float maxVelocity);
	void SetAngularMaxVelocityForDynamic(float maxVelocity);
	void ApplyGravityForDynamic();
	void RemoveGravityForDynamic();
	void SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum flag, bool value);

	void AddTorqueForDynamic(const math::Vector3& torque);
	void AddTorqueXForDynamic(const float& torque);
	void AddTorqueYForDynamic(const float& torque);
	void AddTorqueZForDynamic(const float& torque);


private:
	Physical* mPhysical;

	bool mbGravityApplied;
	bool mbAirborn;

	math::Vector3 mFriction;
	float mFricCoeff;
	math::Vector3 mGravityAccel;
	math::Vector3 mForce;
	math::Vector3 mVelocity;
	math::Vector3 mMaxVelocity;
	math::Vector3 mAccelation;

	math::Vector3 mReserveVelocity;
	Timer mReserveTimer;

	float mMass;
	Transform* mOwnerTransform;
};
