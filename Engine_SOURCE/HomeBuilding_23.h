#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuilding_23 : public GameObj
{
public:
	HomeBuilding_23();
	HomeBuilding_23(const HomeBuilding_23& Obj);
	virtual ~HomeBuilding_23();


	virtual HomeBuilding_23* Clone() const;
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