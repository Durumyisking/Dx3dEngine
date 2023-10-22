#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuildingFive : public GameObj
{
public:
	HomeBuildingFive();
	HomeBuildingFive(const HomeBuildingFive& Obj);
	virtual ~HomeBuildingFive();


	virtual HomeBuildingFive* Clone() const;
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