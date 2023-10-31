#pragma once
#include "Script.h"
#include "PowerMoonObject.h"

class Panal;
class MoonScript : public Script
{
public:
	enum class ePowerMoonState
	{
		None,
		Appear,
		Down,
		Wait,
	};

	MoonScript();
	virtual ~MoonScript();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

	void PowerMoonAppears();
	void HitPayer();

	void GetPowerMoon();
	ePowerMoonState GetState() { return mState; }

private:
	void Appear();
	void Down();
	void Wait();

	ePowerMoonState mState;

	float mCurrentTime;
	float mMaxRotZ;
	Vector3 mOriginPos;

	Panal* mPowerMoonPanal;
	Transform* tr;

	PowerMoonObject* mOwner;
};