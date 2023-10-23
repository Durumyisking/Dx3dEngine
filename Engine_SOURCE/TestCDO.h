#pragma once
#include "GameObj.h"
class TestCDO :
    public GameObj
{
	TestCDO();
	TestCDO(const TestCDO& Obj);
	virtual ~TestCDO();

	virtual TestCDO* Clone() const;
	virtual void Save(FILE* File) override;
	virtual void Load(FILE* File) override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject)  override {};
	virtual void OnTriggerEnter(GameObj* gameObject)    override {};
	virtual void OnTriggerPersist(GameObj* gameObject)		override {};
	virtual void OnTriggerExit(GameObj* gameObject)		override {};
};

