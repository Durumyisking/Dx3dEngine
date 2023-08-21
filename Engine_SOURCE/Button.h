#pragma once
#include "UIBase.h"

class Button : public UIBase
{
public:
	Button();
	Button(eUIType type);
	virtual ~Button();

	virtual void OnInit() override;
	virtual void OnActive() override;
	virtual void OnInActive() override;
	virtual void OnUpdate() override;
	virtual void OnFixedUpdate() override;
	virtual void OnRender() override;
	virtual void OnFontRender() override;
	virtual void OnClear() override;
	
	virtual void Click();

private:
	Event mOnClick;

	Vector3 mScreenPos;
};