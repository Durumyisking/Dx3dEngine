#pragma once
#include "MonsterStateScript.h"

class GoombaStateScript : public MonsterStateScript
{
public:
	GoombaStateScript();
	virtual ~GoombaStateScript();

	virtual void Update() final;
public:
	virtual void Idle() final;
	virtual void Move() final;
	virtual void Jump() final;
	virtual void SpecialSituation() final;
	virtual void Hit() final;
	virtual void Groggy() final;
	virtual void Die() final;
private:

};

