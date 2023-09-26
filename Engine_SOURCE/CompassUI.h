#pragma once
#include  "UIBase.h"

class CompassUI : public UIBase
{

public:
	CompassUI();
	CompassUI(const std::wstring& key, eUIType type);
	~CompassUI();

	virtual void OnInit() override;
	virtual void OnActive() override;
	virtual void OnInActive() override;
	virtual void OnUpdate() override;
	virtual void OnFixedUpdate() override;
	virtual void OnRender() override;
	virtual void OnFontRender() override;
	virtual void OnClear() override;

private:

	bool mActivate;
};