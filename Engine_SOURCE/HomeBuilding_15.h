#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuilding_15 : public GameObj
{
public:
	HomeBuilding_15();
	HomeBuilding_15(const HomeBuilding_15& Obj);
	virtual ~HomeBuilding_15();


	virtual HomeBuilding_15* Clone() const;
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