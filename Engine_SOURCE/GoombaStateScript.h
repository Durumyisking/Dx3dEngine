#pragma once
#include "MonsterStateScript.h"

class GoombaStateScript : public MonsterStateScript
{
public:
	GoombaStateScript();
	virtual ~GoombaStateScript();

	virtual void Update() override;
public:
	virtual void Idle() override;
	virtual void Move() override;
	virtual void Jump() override;
	virtual void SpecialCast() override;
	virtual void Hit() override;
	virtual void Groggy() override;
	virtual void Die() override;
private:

};

