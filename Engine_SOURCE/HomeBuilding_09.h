#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuilding_09 : public GameObj
{
public:
	HomeBuilding_09();
	HomeBuilding_09(const HomeBuilding_09& Obj);
	virtual ~HomeBuilding_09();


	virtual HomeBuilding_09* Clone() const;
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