#pragma once
#include "UIBase.h"

class HUD : public UIBase
{

	HUDState mState;

public:
	HUD();
	HUD(eUIType type);
	~HUD();

	virtual void OnInit() override;
	virtual void OnActive() override;
	virtual void OnInActive() override;
	virtual void OnUpdate() override;
	virtual void OnFixedUpdate() override;
	virtual void OnRender() override;
	virtual void OnFontRender() override;
	virtual void OnClear() override;

	void SetSpeed(float speed) { mSpeed = speed; }
	void SetState(HUDState state) { mState = state; mActivate = true; }
	void SetActive(bool isActive) { mActivate = isActive; }

	void MoveBlink();
	void MoveTowards();
	void Rotate();
	void Size();
	void GoAndReturn();

	void PlayAnimation();

private:

	Vector3 mOriginPos;
	Vector3 mOriginScale;
	float mSpeed;
	float mCurrentTime;
	bool mActivate;
};