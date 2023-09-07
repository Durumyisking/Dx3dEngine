#pragma once
#include "DynamicObject.h"
class MarioParts :
    public DynamicObject
{
public:
	MarioParts();
	virtual ~MarioParts();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;
	virtual void FontRender() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override;
	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerExit(GameObj* gameObject) override;
	virtual void BoneInitialize();

public:
	//MarioParts* GetMario() { return mBody; }
	//void SetName(std::wstring name) { mName = name; }


private:
	std::vector<MarioParts*> mMario;
	//Vector3 mMarioPhysicPos;
	//std::wstring mName;
	//Player* mBody;
	//MarioParts* mHandL;
	//MarioParts* mHandR;
	//MarioParts* mHead;
	//MarioParts* mFace;

};

