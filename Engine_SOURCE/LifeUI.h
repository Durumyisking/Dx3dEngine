#pragma once
#include  "UIBase.h"

class LifeUI : public UIBase
{
	enum class State
	{
		None,
		Hit,
		Move,
		Wait,
		Back,
	};

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
	void SetTargetPos(Vector3 pos) { mTargetPos = pos; }

	void InitColor(Vector4 color) { mOriginColor = color; SetColor(color, true); }
	void PlayerHIt(float amplitude, float frequency);


	void Hit() 
	{
		mState = State::Hit; 
		mbShake = true;
	}
private:
	void ReactionHit();
	void MoveUI(float amplitude, float frequency);
	void BackUI();
	

	float mCurrentTime;
	float mShakeTime;
	float mAmplitude;   // 흔들림 크기
	float mFrequency;   // 코사인 함수 주기
	float mPositionX;  // 객체의 현재 X 위치

	Vector3 mOriginPos;
	Vector3 mTargetPos;
	Vector4 mOriginColor;

	bool mActivate;
	bool mbShake;
	bool mbBack;
	int mCount;

	State mState;
};