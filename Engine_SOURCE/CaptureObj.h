#pragma once
#include "Engine.h"

class GameObj;

class CaptureObj
{
public:
	CaptureObj() = default;
	virtual ~CaptureObj();

	void Update();
	virtual void Divide();
	virtual void CaptureEvent() = 0;

	void OnCapture() { mbCapture = true; }
	void OffCapture() { mbCapture = false; }
public:
	GETSET(bool, mbCapture, Capture)
	GETSET(GameObj*, mPlayer, Player)

	bool IsCapture() { return mbCapture; }
private:
	GameObj* mPlayer	= nullptr;
	bool mbCapture		= false;
};

