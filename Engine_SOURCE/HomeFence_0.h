#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeFence_0 : public GameObj
{
public:
	HomeFence_0();
	HomeFence_0(const HomeFence_0& Obj);
	virtual ~HomeFence_0();


	virtual HomeFence_0* Clone() const;
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