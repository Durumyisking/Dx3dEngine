#pragma once
#include "GameObj.h"

class CityWorldBushA : public GameObj
{
public:
	CityWorldBushA();
	CityWorldBushA(const CityWorldBushA& Obj);
	virtual ~CityWorldBushA();

	virtual CityWorldBushA* Clone() const;
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override;
	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerPersist(GameObj* gameObject) override;
	virtual void OnTriggerExit(GameObj* gameObject) override;
};