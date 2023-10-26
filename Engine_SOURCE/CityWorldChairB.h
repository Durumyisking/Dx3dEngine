#pragma once
#include "GameObj.h"

class CityWorldChairB : public GameObj
{
public:
	CityWorldChairB();
	CityWorldChairB(const CityWorldChairB& Obj);
	virtual ~CityWorldChairB();

	virtual CityWorldChairB* Clone() const;
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override;
	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerPersist(GameObj* gameObject) override;
	virtual void OnTriggerExit(GameObj* gameObject) override;

};