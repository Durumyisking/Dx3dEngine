#pragma once
#include "DynamicObject.h"

class InstancingContainer;
class InstantiativeObject :
    public DynamicObject
{
public:
	InstantiativeObject();
	InstantiativeObject(const InstantiativeObject& Obj);
	virtual ~InstantiativeObject();

	virtual InstantiativeObject* Clone() const;
	virtual void Save(FILE* File) override;
	virtual void Load(FILE* File) override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;
	virtual void PrevRender() override;
	virtual void FontRender() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override {};
	virtual void OnTriggerEnter(GameObj* gameObject) override {};
	virtual void OnTriggerPersist(GameObj* gameObject) override {};
	virtual void OnTriggerExit(GameObj* gameObject) override {};

	void SetContainer(InstancingContainer* container, UINT instanceID) { mContainer = container; mInstanceID = instanceID; }

private:
	InstancingContainer* mContainer;
	UINT				mInstanceID;

};

