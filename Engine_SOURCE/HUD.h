#pragma once
#include "UIBase.h"

class HUD : public UIBase
{
public:
	HUD();
	HUD(eUIType type);
	~HUD();

	virtual void OnInit() final;
	virtual void OnActive() final;
	virtual void OnInActive() final;
	virtual void OnTick() final;
	virtual void OnRender() final;
	virtual void OnClear() final;

private:

};

