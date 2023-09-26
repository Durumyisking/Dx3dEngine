#pragma once
#include "ProjectileObj.h"

class PackunPostionBall : public ProjectileObj
{
public:
	PackunPostionBall();
	virtual ~PackunPostionBall();

	virtual void Initialize() override;
	virtual void Update() override;

	virtual void OnTriggerEnter(GameObj* gameObject) final;
private:

};

