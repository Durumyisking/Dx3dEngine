#include "RigidBody.h"
#include "TimeMgr.h"
#include "GameObj.h"
#include "Player.h"

namespace dru
{
	RigidBody::RigidBody()
		: Component(eComponentType::RigidBody)
		, mForce(Vector3::Zero)
		, mAccel(Vector3::Zero)
		, mVelocity(Vector3::Zero)
		, mMass(1.f)
		, mMaxSpeed(Vector3(5.f, 7.f, 0.f))
		, mFricCoeff(20.f)
		, mbOnAir(true)
		, mbAffectedGravity(true)
		, mbSwitch(true)
		, mCurrentGravity(Vector3(0.f, -25.f, 0.f))
		, mOriginGravity(Vector3(0.f, -25.f, 0.f))
	{
	}

	RigidBody::~RigidBody()
	{
	}

	void RigidBody::Initialize()
	{
	}

	void RigidBody::update()
	{
		if (mbSwitch)
		{
			SetAccelFromForce();

			
			if (mbOnAir && mbAffectedGravity)
			{
				mAccel += mCurrentGravity;
			}

			mVelocity += mAccel * DT;

			CalculateFriction();

			MaxVelocityCheck();

			objMove();

			mForce = Vector3(0.f, 0.f, 0.f);
			mAccel = Vector3(0.f, 0.f, 0.f);
			mCurrentGravity = mOriginGravity;
		}
	}

	void RigidBody::fixedUpdate()
	{
	}

	void RigidBody::render()
	{
	}


	void RigidBody::SetAccelFromForce()
	{
		float fForce = mForce.Length();

		if (0.f != fForce)
		{
			mForce.Normalize();

			float Accel = fForce / mMass;

			mAccel = mForce * Accel;
		}
	}

	void RigidBody::CalculateFriction()
	{
		if (mVelocity != Vector3::Zero && !mbOnAir)
		{
			Vector3 FricDir = -mVelocity;
			FricDir.Normalize();

			Vector3 Friction = FricDir * mFricCoeff * DT;

			mAccel += Friction;

			if (mVelocity.Length() <= Friction.Length())
			{
				mVelocity = Vector3(0.f, 0.f, 0.f);
			}
			else
			{
				mVelocity += Friction;
			}
		}
	}

	void RigidBody::MaxVelocityCheck()
	{
		if (mMaxSpeed.x < fabs(mVelocity.x))
		{
			mVelocity.x /= fabs(mVelocity.x);
			mVelocity.x *= mMaxSpeed.x;
		}
		if (mMaxSpeed.y < fabs(mVelocity.y))
		{
			mVelocity.y /= fabs(mVelocity.y);
			mVelocity.y *= mMaxSpeed.y;
		}
	}

	void RigidBody::objMove()
	{
		float Speed = mVelocity.Length();

		if (0.f != Speed)
		{
			// 이동방향
			Vector3 Dir = mVelocity;
			
			Dir.Normalize();

			Vector3 Pos = GetOwner()->GetPos();
			
			Pos.x += mVelocity.x * DT;

			if (!(!mbOnAir && mVelocity.y < 0.f))
			{
				Pos.y += mVelocity.y * DT;
			}

			GetOwner()->SetPos(Pos);
		}
	}

}
