#pragma once
#include "UIBase.h"

class HUD : public UIBase
{

	eHUDState mState;

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
	void SetState(eHUDState state) { mState = state; mActivate = true; }
	void SetActive(bool isActive) { mActivate = isActive; }
	void SetChangeSize(Vector3 size) { mChangeSize = size; }
	void SetTargetPos(Vector3 pos) { mTargetPos = pos; }
	void SetCount(int count) { mCount = count; }

	void MoveBlink(Vector3 changeSize);
	void MoveTowards();
	void Rotate();
	void Size();
	void TitleCapMove();
	void PlayAnimation();


	void SetColor(Vector4 color, bool isColor) override;
	void SetStop() { mbStop = true; }
	void SetStart() { mbStop = false; }

protected:
	Vector3 mOriginPos;
	Vector3 mOriginScale;
	Vector3 mChangeSize;
	Vector3 mTargetPos;
	float mSpeed;
	float mCurrentTime;
	bool mActivate;
	bool mbStop;
	bool mbGoAndReturn;

	int mCount;

private:
};