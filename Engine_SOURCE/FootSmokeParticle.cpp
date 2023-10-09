#include "FootSmokeParticle.h"
#include "ParticleSystem.h"
#include "GameObj.h"
#include "Transform.h"
#include "TimeMgr.h"

#define OFFSET 0.1f
FootSmokeParticle::FootSmokeParticle(int maxCount, eParticleType type)
	: ParticleFormat(maxCount, type)
	, mActiveOffset(OFFSET)
{
	mAccType = eAccessType::CPU;
}

FootSmokeParticle::~FootSmokeParticle()
{

}

void FootSmokeParticle::Update()
{
	mActiveOffset -= DT;
	Calculator();
	SUPER::Update();
}

void FootSmokeParticle::Initalize()
{
	SUPER::Initalize();

	for (size_t i = 0; i < mParticleData.size(); ++i)
	{
		mParticleData[i].startScale = mParticleCB.startSize.XYZ();
		mParticleData[i].endScale = mParticleCB.endSize.XYZ();

		mParticleData[i].q_startRotation = mParticleCB.startAngle;
		mParticleData[i].q_endRotation = mParticleCB.endAngle;


		mParticleData[i].active = 0;
	}
}

void FootSmokeParticle::Calculator()
{
	if (mParticleSystem == nullptr)
		return;

	Vector3 pos = mParticleSystem->GetOwner()->GetTransform()->GetPhysicalPosition();
	for (size_t i = 0; i < mParticleData.size(); ++i)
	{
		if (mParticleData[i].active == 0)
		{
			if (mActiveOffset <= 0.f)
			{
				mActiveOffset = OFFSET;
				mParticleData[i].active = 1;

				ParticleSystem* system = mParticleSystem;
				Vector3 Pos = mParticleSystem->GetOwner()->GetTransform()->GetPhysicalPosition();

				mParticleData[i].position = Vector4(Pos, 1.0f);
			}
			break;
		}

		if (mParticleData[i].elapsedTime > mParticleData[i].lifeTime)
			continue;

		mParticleData[i].elapsedTime += DT;

		
		// Scale
		Vector3 sacle = math::Interpolation<Vector3>(0.f, mParticleData[i].lifeTime
													, mParticleData[i].elapsedTime
													, mParticleData[i].startScale
													, mParticleData[i].endScale);

		// Rotation
		Quaternion q_startRotation(mParticleData[i].q_startRotation);
		Quaternion q_endRotation(mParticleData[i].q_startRotation);
		Quaternion q_result = Quaternion::Slerp(q_startRotation, q_endRotation, mParticleData[i].elapsedTime / mParticleData[i].lifeTime);
		Vector3 e_rotation = q_result.ToEuler();

		math::Matrix sacleMatrix = math::Matrix::CreateScale(sacle);
		math::Matrix rotationMatrix = math::Matrix::CreateRotationX(e_rotation.x);
		rotationMatrix *= math::Matrix::CreateRotationY(e_rotation.y);
		rotationMatrix *= math::Matrix::CreateRotationZ(e_rotation.z);

		// S * R
		mParticleData[i].particleWorld = sacleMatrix * rotationMatrix;

		// Translation
		mParticleData[i].particleWorld._41 = mParticleData[i].position.x;
		mParticleData[i].particleWorld._42 = mParticleData[i].position.y;
		mParticleData[i].particleWorld._43 = mParticleData[i].position.z;
	}
}
