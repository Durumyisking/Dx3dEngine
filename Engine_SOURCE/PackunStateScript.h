#pragma once
#include "MonsterStateScript.h"


class PackunStateScript : public MonsterStateScript
{
public:
	PackunStateScript();
	virtual ~PackunStateScript();

	virtual void Update() final;

public:
	virtual void Idle() final;
	virtual void Move() final;
	virtual void Jump() final {};
	virtual void Attack() final;
	virtual void SpecialSituation() final;
	virtual void Hit() final;
	virtual void Groggy() final;
	virtual void Die() final;

private:

};

