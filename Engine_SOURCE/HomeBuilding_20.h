#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuilding_20 : public GameObj
{
public:
	HomeBuilding_20();
	HomeBuilding_20(const HomeBuilding_20& Obj);
	virtual ~HomeBuilding_20();


	virtual HomeBuilding_20* Clone() const;
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