#pragma once
#include "UIScript.h"

class WorldMapScript : public UIScript
{
public:
	WorldMapScript();
	virtual ~WorldMapScript();

	virtual void Update() override;

private:

	float mCurrentTime;
};