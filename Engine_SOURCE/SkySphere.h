#pragma once
#include "Engine.h"
#include "GameObj.h"


class SkySphere : public GameObj
{
public:
	SkySphere();
	virtual ~SkySphere();

	virtual void Initialize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();


};

