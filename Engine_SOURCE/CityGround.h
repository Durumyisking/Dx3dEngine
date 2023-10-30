#pragma once
#include "GameObj.h"

class MeshRenderer;
class CityGround : public GameObj
{

public:
	CityGround();
	CityGround(const CityGround& Obj);
	virtual ~CityGround();

	virtual CityGround* Clone() const;
	virtual void Save(FILE* File) override;
	virtual void Load(FILE* File) override;

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