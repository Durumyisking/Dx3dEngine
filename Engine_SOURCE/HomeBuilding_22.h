#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuilding_22 : public GameObj
{
public:
	HomeBuilding_22();
	HomeBuilding_22(const HomeBuilding_22& Obj);
	virtual ~HomeBuilding_22();


	virtual HomeBuilding_22* Clone() const;
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