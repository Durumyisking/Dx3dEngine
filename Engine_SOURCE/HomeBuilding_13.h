#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuilding_13 : public GameObj
{
public:
	HomeBuilding_13();
	HomeBuilding_13(const HomeBuilding_13& Obj);
	virtual ~HomeBuilding_13();


	virtual HomeBuilding_13* Clone() const;
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