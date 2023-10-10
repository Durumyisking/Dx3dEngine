#pragma once
#include "UIScript.h"
#include "Player.h"

class CompassNeedleScript : public UIScript
{
public:
	CompassNeedleScript();
	virtual ~CompassNeedleScript();

	virtual void Initialize() override;
	virtual void Update() override;


	void SetPlayer(Player* player) { mPlayer = player; }
private:
	Player* mPlayer;
};