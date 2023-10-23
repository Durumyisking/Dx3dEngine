#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuilding_10 : public GameObj
{
public:
	HomeBuilding_10();
	HomeBuilding_10(const HomeBuilding_10& Obj);
	virtual ~HomeBuilding_10();


	virtual HomeBuilding_10* Clone() const;
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