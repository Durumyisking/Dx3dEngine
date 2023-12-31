#pragma once
#include "GameObj.h"

class MeshRenderer;
class Building : public GameObj
{

public:
	Building();
	Building(const Building& Obj);
	virtual ~Building();

	virtual Building* Clone() const override;
	virtual void Save(FILE* File) override;
	virtual void Load(FILE* File) override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override;
	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerPersist(GameObj* gameObject) override;
	virtual void OnTriggerExit(GameObj* gameObject) override;

private:
	std::wstring mModelName;
	bool mbPhysical;
	Vector3 mPhysicalScale;
	eGeometryType mGeometryType;
	eActorType mActorType;
};