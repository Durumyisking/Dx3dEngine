#pragma once
#include "UIBase.h"

class Panal : public UIBase
{
public:
	Panal();
	Panal(eUIType type);
	virtual ~Panal();

	virtual void OnInit() final;
	virtual void OnActive() final;
	virtual void OnInActive() final;
	virtual void OnTick() final;
	virtual void OnRender() final;
	virtual void OnClear() final;


private:

};