#pragma once
#include "ParticleFormat.h"
class FootSmokeParticle : public ParticleFormat
{
public:
	FootSmokeParticle(int maxCount, eParticleType type);
	virtual ~FootSmokeParticle();

	virtual void Update() override;
	virtual void Initalize() override;
	virtual void Calculator() override;

	virtual void Reset() override;
private:
	float mActiveOffset;
};

