#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuilding_12 : public GameObj
{
public:
	HomeBuilding_12();
	HomeBuilding_12(const HomeBuilding_12& Obj);
	virtual ~HomeBuilding_12();


	virtual HomeBuilding_12* Clone() const;
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