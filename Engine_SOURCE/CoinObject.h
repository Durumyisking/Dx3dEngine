#pragma once
#include "GameObj.h"

class MeshRenderer;
class CoinObject : public GameObj
{
public:
	CoinObject();
	CoinObject(const CoinObject& Obj);
	virtual ~CoinObject();

	virtual CoinObject* Clone() const;
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