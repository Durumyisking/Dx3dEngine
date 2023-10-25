#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuilding_17 : public GameObj
{
public:
	HomeBuilding_17();
	HomeBuilding_17(const HomeBuilding_17& Obj);
	virtual ~HomeBuilding_17();


	virtual HomeBuilding_17* Clone() const;
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