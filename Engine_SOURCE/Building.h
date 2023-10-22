#pragma once
#include "GameObj.h"

class MeshRenderer;
class Building : public GameObj
{

public:
	Building();
	Building(const Building& Obj);
	virtual ~Building();

	virtual Building* Clone() const;
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