#include "FootSmokeParticle.h"
#include "ParticleSystem.h"
#include "GameObj.h"
#include "Transform.h"
#include "TimeMgr.h"

#define OFFSET_TIME 0.05f
FootSmokeParticle::FootSmokeParticle(int maxCount, eParticleType type)
	: ParticleFormat(maxCount, type, eAccessType::CPU)
	, mActiveOffset(OFFSET_TIME)
{
	mAccType = eAccessType::CPU;
	mParticleCB.maxLifeTime = (OFFSET_TIME * static_cast<float>(maxCount + 1));

	Initalize();

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
	for (size_t i = 0; i < mParticleData.size(); ++i)
	{
		mParticleData[i].startScale = mParticleCB.startSize.XYZ();
		mParticleData[i].endScale = mParticleCB.endSize.XYZ();

		mParticleData[i].q_startRotation = mParticleCB.startAngle;
		mParticleData[i].q_endRotation = mParticleCB.endAngle;

		mParticleData[i].lifeTime = 0.2f;
		mParticleData[i].active = 0;
	}
}

void FootSmokeParticle::Calculator()
{
	if (mParticleSystem == nullptr)
		return;

	std::function<void(Particle& particle)> resetParameter = [this](Particle& particle)
		{
			mActiveOffset = OFFSET_TIME;
			particle.active = 1;

			ParticleSystem* system = mParticleSystem;
			Transform* transform = mParticleSystem->GetOwner()->GetTransform();
			Vector3 Pos = transform->GetPhysicalPosition();

			particle.position = Vector4(Pos, 1.0f);
		};

	Vector3 pos = mParticleSystem->GetOwner()->GetTransform()->GetPhysicalPosition();
	for (size_t i = 0; i < mParticleData.size(); ++i)
	{
		if (mParticleData[i].active == 0)
		{
			if (mActiveOffset <= 0.f)
			{
				resetParameter(mParticleData[i]);
			}
			break;
		}

		if (mParticleData[i].elapsedTime > mParticleData[i].lifeTime)
		{
			mParticleData[i].elapsedTime = 0.f;
			resetParameter(mParticleData[i]);
		}

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

void FootSmokeParticle::Reset()
{
	SUPER::Reset();

	for (size_t i = 0; i < mParticleData.size(); ++i)
	{
		mParticleData[i].elapsedTime = 0.f;
		mParticleData[i].active = 0;
		mParticleData[i].particleWorld = math::Matrix::Identity;
		mParticleData[i].particleWorld._11 = 0.f;
		mParticleData[i].particleWorld._22 = 0.f;
		mParticleData[i].particleWorld._33 = 0.f;
	}
}
