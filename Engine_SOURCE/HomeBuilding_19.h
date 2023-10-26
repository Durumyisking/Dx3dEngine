#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuilding_19 : public GameObj
{
public:
	HomeBuilding_19();
	HomeBuilding_19(const HomeBuilding_19& Obj);
	virtual ~HomeBuilding_19();


	virtual HomeBuilding_19* Clone() const;
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