#pragma once
#include "GameObj.h"

class MeshRenderer;
class HomeBuildingEight : public GameObj
{
public:
	HomeBuildingEight();
	virtual ~HomeBuildingEight();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override;
	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerPersist(GameObj* gameObject) override;
	virtual void OnTriggerExit(GameObj* gameObject) override;

	void CreateBuilding(const std::wstring& modelKey, int count, const std::wstring& ...);

private:

};