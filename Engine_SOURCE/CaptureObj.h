#pragma once
#include "Engine.h"

class GameObj;
class MarioCap;
class Player;

class CaptureObj
{
public:
	CaptureObj() = default;
	virtual ~CaptureObj();

	void Update();
	virtual void Divide();
	virtual void CaptureEnter(MarioCap* cap) {};
	virtual void DivideEvent() {};
	virtual void CaptureEvent() = 0;

	void OnCapture() { mbCapture = true; }
	void OffCapture() { mbCapture = false; }

public:
	GETSET(bool, mbCapture, Capture)
	GETSET(MarioCap*, mCap, Cap)
	GETSET(Player*, mPlayer, Player)

	bool IsCapture() { return mbCapture; }

	void CopyCaptureData(CaptureObj* other, bool copyCap = true);
	void ClearCaptureData(bool copyCap);

private:
	MarioCap* mCap    = nullptr;
	Player* mPlayer		= nullptr;
	bool mbCapture		= false;
};

