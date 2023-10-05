#pragma once
#include "GameObj.h"
class DynamicObject :
    public GameObj
{
public:

	struct StateInfo {
		std::set<UINT> mLockState;
	};

public:
	DynamicObject();
	DynamicObject(const DynamicObject& Obj);
	virtual ~DynamicObject();

	virtual DynamicObject* Clone() const;
	virtual void Save(FILE* File) override;
	virtual void Load(FILE* File) override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;
	virtual void FontRender() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override{};
	virtual void OnTriggerEnter(GameObj* gameObject) override{};
	virtual void OnTriggerStay(GameObj* gameObject) override {};
	virtual void OnTriggerExit(GameObj* gameObject) override{};


public:
	virtual void BoneInitialize() {};
	void InsertLockState(UINT curState, UINT lockState);

protected:
	virtual void stateInfoInitalize() {};

protected:
	std::vector<StateInfo> mStateInfo;

};

