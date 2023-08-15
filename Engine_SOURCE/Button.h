#pragma once
#include "UIBase.h"

class Button : public UIBase
{
public:
	Button();
	~Button();

	virtual void OnInit() final;
	virtual void OnActive() final;
	virtual void OnInActive() final;
	virtual void OnTick() final;
	virtual void OnRender() final;
	virtual void OnClear() final;

};