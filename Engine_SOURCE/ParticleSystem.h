#pragma once
#include "BaseRenderer.h"
#include "ParticleShader.h"
#include "Renderer.h"



using namespace renderer;
class ParticleSystem : public BaseRenderer
{
	enum class eSimulationSpace
	{
		Local,
		World,
	};
public:
	ParticleSystem();
	virtual ~ParticleSystem();

	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;
	virtual void Render() final;

	void ParticleRender();

	void MakeParticleBufferData(Vector4 startPosition, UINT maxParticleCount, float minLifeTime, float maxLifeTime, float speed, float radian, UINT active);

	void MakeConstantBufferData(std::wstring shaderName, ParticleSystemCB CB);

	void SetStartPosition(Vector3 startPos) { mStartPosition = Vector4(startPos.x, startPos.y, startPos.z, 1.f); }
	void SetStartScale(Vector3 startScale) { mStartScale= Vector4(startScale.x, startScale.y, startScale.z, 1.f); }

	void SetMaxElapsedTime(float value) { mMaxElapsedTime = value; }

	void SetMaxParticleCount(UINT maxParticleCount) { mMaxParticles = maxParticleCount; }
	void SetStartColor(Vector4 color) { mStartColor = color; }
	void SetEndColor(Vector4 color) { mEndColor = color; }
	void SetStartSpeed(float startSpeed) { mStartSpeed = startSpeed; }
	void SetEndSpeed(float endSpeed) { mEndSpeed = endSpeed; }
	void SetMaxLifeTime(float maxLifeTime) { mMaxLifeTime = maxLifeTime; }
	void SetMinLifeTime(float minLifeTime) { mMinLifeTime = minLifeTime; }
	void SetRadius(float _Radius) { mRadius = _Radius; }

	void SetParticleDirection(const Vector3& dir);
	void SetParticleCountInFrame(UINT count) { mParticleCountInFrame = count; }

	void SetFrequency(float frequency) { mFrequency = frequency; }

	void UseSwitchOn() { mbUseSwitch = true; }
	void UseSwitchOff() { mbUseSwitch = false; }
	void SwitchReset() { mbTimerSwitch = false; }

private:
	Particle* mParticle;

	
	class StructedBuffer* mBuffer;
	class StructedBuffer* mSharedBuffer;
	ParticleShader* mCS;
	ParticleSystemCB mCBData;

	Vector4 mStartPosition;
	Vector4 mStartScale;
	Vector4 mStartColor;
	Vector4 mEndColor;

	UINT mMaxParticles;
	eSimulationSpace mSimulationSpace;
	float mRadius;

	float mStartSpeed;
	float mEndSpeed;
	float mMaxLifeTime;
	float mMinLifeTime;

	float mStartAngle;
	float mEndAngle;
	float mElapsedTime;
	float mGravity;

	float mForce;
	float mRadian;

	float mFrequency;

	float mMaxElapsedTime;

	bool mbUseSwitch;
	bool mbTimerSwitch;
	UINT mParticleCountInFrame;

};

