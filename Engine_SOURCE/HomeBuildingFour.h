#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuildingFour : public GameObj
{
public:
	HomeBuildingFour();
	HomeBuildingFour(const HomeBuildingFour& Obj);
	virtual ~HomeBuildingFour();

	virtual HomeBuildingFour* Clone() const;
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