#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuilding_16 : public GameObj
{
public:
	HomeBuilding_16();
	HomeBuilding_16(const HomeBuilding_16& Obj);
	virtual ~HomeBuilding_16();


	virtual HomeBuilding_16* Clone() const;
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