#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuilding_24 : public GameObj
{
public:
	HomeBuilding_24();
	HomeBuilding_24(const HomeBuilding_24& Obj);
	virtual ~HomeBuilding_24();


	virtual HomeBuilding_24* Clone() const;
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