#pragma once
#include  "UIBase.h"

class LifeUI : public UIBase
{
public:
	LifeUI();
	LifeUI(const std::wstring& key, eUIType type);
	~LifeUI();

	virtual void OnInit() override;
	virtual void OnActive() override;
	virtual void OnInActive() override;
	virtual void OnUpdate() override;
	virtual void OnFixedUpdate() override;
	virtual void OnRender() override;
	virtual void OnFontRender() override;
	virtual void OnClear() override;


	void SetColor(Vector4 color, bool isColor) override;
	void SetUIActive() { mActivate = true; }
	void SetUIInActive() { mActivate = false; }
private:

	float mCurrentTime;
	bool mActivate;
};