#pragma once
#include "GameObj.h"
class DynamicObject :
    public GameObj
{
public:
<<<<<<< HEAD
=======
	struct StateInfo {
		std::set<UINT> mLockState;
	};

public:
>>>>>>> 255dec6e611b73f6e438073350714e0bf2481d2d
	DynamicObject();
	virtual ~DynamicObject();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;
	virtual void FontRender() override;

public:
<<<<<<< HEAD
	virtual void OnCollisionEnter(GameObj* gameObject) override;
	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerExit(GameObj* gameObject) override;

private:

public:
	virtual void BoneInitialize() {};
	struct StateInfo {
		UINT mState;
		std::set<UINT> mLockState;
	};
=======
	virtual void OnCollisionEnter(GameObj* gameObject) override{};
	virtual void OnTriggerEnter(GameObj* gameObject) override{};
	virtual void OnTriggerExit(GameObj* gameObject) override{};


public:
	virtual void BoneInitialize() {};
	void InsertLockState(UINT curState, UINT lockState);

protected:
	virtual void stateInfoInitalize() = 0;

protected:
	std::vector<StateInfo> mStateInfo;
>>>>>>> 255dec6e611b73f6e438073350714e0bf2481d2d

};

