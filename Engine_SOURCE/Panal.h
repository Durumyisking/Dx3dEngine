#pragma once
#include "UIBase.h"

class Panal : public UIBase
{
public:
	Panal();
	Panal(eUIType type);
	virtual ~Panal();

	virtual void OnInit() override;
	virtual void OnActive() override;
	virtual void OnInActive() override;
	virtual void OnUpdate() override;
	virtual void OnFixedUpdate() override;
	virtual void OnRender() override;
	virtual void OnFontRender() override;
	virtual void OnClear() override;

	UIBase* GetOwner() { return this; }

private:

};