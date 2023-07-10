#pragma once
#include "Component.h"

using namespace dru::math;

namespace dru
{
    class RigidBody :
        public Component
    {
		// 오른쪽 기준
#define DEGREE_90 dru::math::Vector3(0.f, 1.f, 0.f)
#define DEGREE_45 dru::math::Vector3(0.707f, 0.707f, 0.f)
#define DEGREE_30 dru::math::Vector3(0.866f, 0.5f, 0.f)
#define DEGREE_15 dru::math::Vector3(0.966f, 0.259f, 0.f)
#define DEGREE_0 dru::math::Vector3(1.f, 0.f, 0.f)


    public:
        RigidBody();
        ~RigidBody();

        virtual void Initialize();
        virtual void update();
        virtual void fixedUpdate();
        virtual void render();


	public:
		void AddForce(Vector3 value) { mForce += value; }
		void AddForceX(float value) { mForce.x += value; }
		void AddForceY(float value) { mForce.y += value; }

		void SetMass(float value) { mMass = value; }
		float GetMass() const { return mMass; }

		void SetForce(Vector3 value) { mForce = value; }
		Vector3 GetForce() const { return mForce; }

		void SetVelocity(Vector3 value) { mVelocity = value; }
		void SetVelocityX(float value) { mVelocity.x = value; }
		void SetVelocityY(float value) { mVelocity.y = value; }

		Vector3 GetVelocity() const { return mVelocity; }

		void AddVelocity(Vector3 value) { mVelocity += value; }
		void AddVelocityX(float value) { mVelocity.x += value; }
		void AddVelocityY(float value) { mVelocity.y += value; }

		void SetMaxVelocity(Vector3 value) { mMaxSpeed = value; }
		Vector3 GetMaxVelocity() const { return  mMaxSpeed; }

		void SetFricCoeff(float value) { mFricCoeff = value; }
		float GetFricCoeff() const { return mFricCoeff; }

		void SetGround() { mbOnAir = false; }
		void SetAir() { mbOnAir = true; }

		bool IsOnAir() { return mbOnAir; }

		void AffectedGravityOn() { mbAffectedGravity = true; }
		void AffectedGravityOff() { mbAffectedGravity = false; }


		void SetAccelFromForce();
		void CalculateFriction();
		void MaxVelocityCheck();

		void SetGravity(float value) { mOriginGravity.y = value; }

		void SwitchOn() { mbSwitch = true; }
		void SwitchOff() { mbSwitch = false; }

	private:
		void objMove();

	private:
		Vector3		mForce;		// 크기, 방향
		Vector3		mAccel;		// 가속도
		float		mMass;		// 질량

		Vector3		mVelocity;	// 속도 (속력 + 방향)
		Vector3		mMaxSpeed;	// 최대 속력

		Vector3		mCurrentGravity;
		Vector3		mOriginGravity;

		float		mFricCoeff;	// 마찰계수

		bool		mbOnAir;

		bool		mbAffectedGravity;

		bool		mbSwitch;
    };

}
