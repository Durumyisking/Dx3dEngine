#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuilding_11 : public GameObj
{
public:
	HomeBuilding_11();
	HomeBuilding_11(const HomeBuilding_11& Obj);
	virtual ~HomeBuilding_11();


	virtual HomeBuilding_11* Clone() const;
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