#pragma once
#include "Script.h"

class CoinUIScript : public Script
{
public:
	CoinUIScript();
	virtual ~CoinUIScript();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

private:

};

