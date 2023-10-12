#pragma once
#include "DynamicObject.h"
class MarioParts :
    public DynamicObject
{
public:
	MarioParts();
	MarioParts(const MarioParts& Obj);
	virtual ~MarioParts();

	virtual MarioParts* Clone() const;
	virtual void Save(FILE* File) final;
	virtual void Load(FILE* File) final;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;
	virtual void PrevRender() override;
	virtual void FontRender() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override;
	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerExit(GameObj* gameObject) override;
	virtual void BoneInitialize();

public:

protected:
	virtual void stateInfoInitalize() override {};


private:

};

