#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuilding_18 : public GameObj
{
public:
	HomeBuilding_18();
	HomeBuilding_18(const HomeBuilding_18& Obj);
	virtual ~HomeBuilding_18();


	virtual HomeBuilding_18* Clone() const;
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