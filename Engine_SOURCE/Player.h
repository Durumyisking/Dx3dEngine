#pragma once
#include "GameObj.h"
#include "DynamicObject.h"



class Player :
	public DynamicObject
{

public:
	Player();
	virtual ~Player();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;
	virtual void FontRender() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override;
	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerExit(GameObj* gameObject) override;
	virtual void BoneInitialize();


private:

public:


};
