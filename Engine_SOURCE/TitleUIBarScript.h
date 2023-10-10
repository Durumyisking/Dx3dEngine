#pragma once
#include "UIScript.h"
#include "Panal.h"

class TitleUIBarScript : public UIScript
{
	enum class State
	{
		Idle,
		ScaleChange,
	};

public:
	TitleUIBarScript();
	virtual ~TitleUIBarScript();


	virtual void Initialize() override;
	virtual void Update() override;

	void Idle();
	void ScaleChange();
	void Select();
private:
	Panal* mTitleTextPanal;

	float mCurrentTime;
	State mState;
	Vector3 mOriginScale;
	Vector3 mChangeSize;

	int mCount;
};