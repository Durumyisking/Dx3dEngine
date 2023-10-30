#pragma once
#include "Engine.h"
#include "GameObj.h"


class SkySphere : public GameObj
{
public:
	SkySphere();
	SkySphere(const SkySphere& Obj);
	virtual ~SkySphere();

	virtual SkySphere* Clone() const override;
	virtual void Save(FILE* File) override;
	virtual void Load(FILE* File) override;

	virtual void Initialize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();


};

