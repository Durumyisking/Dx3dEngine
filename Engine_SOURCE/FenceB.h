#pragma once
#include "GameObj.h"

class MeshRenderer;
class FenceB : public GameObj
{
public:
	FenceB();
	FenceB(const FenceB& Obj);
	virtual ~FenceB();


	virtual FenceB* Clone() const;
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