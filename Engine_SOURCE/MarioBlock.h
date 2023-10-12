#pragma once
#include "GameObj.h"

class MeshRenderer;
class MarioBlock : public GameObj
{

public:
	MarioBlock();
	virtual ~MarioBlock();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override;
	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerStay(GameObj* gameObject) override;
	virtual void OnTriggerExit(GameObj* gameObject) override;

private:

};

