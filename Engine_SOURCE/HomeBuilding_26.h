#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuilding_26 : public GameObj
{
public:
	HomeBuilding_26();
	HomeBuilding_26(const HomeBuilding_26& Obj);
	virtual ~HomeBuilding_26();


	virtual HomeBuilding_26* Clone() const;
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override;
	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerPersist(GameObj* gameObject) override;
	virtual void OnTriggerExit(GameObj* gameObject) override;

private:
};