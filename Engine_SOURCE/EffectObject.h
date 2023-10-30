#pragma once
#include "GameObj.h"

class EffectObject : public GameObj
{
public:
	EffectObject();
	virtual ~EffectObject();

	virtual void Initialize() override;
	virtual void Update() override;

	virtual void OnTriggerEnter(GameObj* gameObject) final;
private:

};

