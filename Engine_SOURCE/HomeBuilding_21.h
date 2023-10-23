#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuilding_21 : public GameObj
{
public:
	HomeBuilding_21();
	HomeBuilding_21(const HomeBuilding_21& Obj);
	virtual ~HomeBuilding_21();


	virtual HomeBuilding_21* Clone() const;
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