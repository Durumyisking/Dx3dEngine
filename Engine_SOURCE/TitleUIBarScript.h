#pragma once
#include "UIScript.h"
#include "Panal.h"

class TitleUIBarScript : public UIScript
{
	enum class State
	{
		Idle,
		ScaleChange,
		SelectUIButton,
		ChangeScene,
	};

public:
	TitleUIBarScript();
	virtual ~TitleUIBarScript();


	virtual void Initialize() override;
	virtual void Update() override;


	void SelectUI() { mState = State::SelectUIButton; }
private:
	void Idle();
	void ScaleChange();
	void SelectUIButton();
	void ChangeScene();

	Panal* mTitleTextPanal;

	float mCurrentTime;
	State mState;
	Vector3 mOriginScale;
	Vector3 mChangeSize;

	int mCount;
};