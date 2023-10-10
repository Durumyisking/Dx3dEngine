#pragma once
#include "Script.h"

class UIScript : public Script
{
public:
	UIScript();
	virtual ~UIScript();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

	void Run() { isRunning = true; }
	void Stop() { isRunning = false; }

	bool GetIsRunning() { return isRunning; }

protected:
	bool isRunning;

};