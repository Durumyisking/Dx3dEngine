#pragma once
#include "Script.h"

class BlockBrickScript : public Script
{
public:
	enum class eState : short
	{
		Idle,
		Distroy,
		Die,
	};

	BlockBrickScript();
	virtual ~BlockBrickScript();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

	void GetHit();

private:
	eState mState;
};
