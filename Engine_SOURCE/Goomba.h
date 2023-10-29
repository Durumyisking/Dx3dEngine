#pragma once
#include "Monster.h"

class Goomba : public Monster
{
public:
	Goomba();
	Goomba(const Goomba& Obj);
	virtual ~Goomba();

	virtual Goomba* Clone() const;
	virtual void Save(FILE* File) final;
	virtual void Load(FILE* File) final;

	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;

	virtual void PrevRender() final;
	virtual void Render() final;

	virtual void CaptureEvent() final;
	
	virtual void OnCollisionEnter(GameObj* gameObject) final;
	virtual void OnTriggerEnter(GameObj* gameObject) final;
	virtual void OnTriggerPersist(GameObj* gameObject) final;
	virtual void OnTriggerExit(GameObj* gameObject) final;

public:
	UINT GetLayerIdx() const { return mGoombaLayerIdx; }
	std::vector<Goomba*>  GetGoombaLayer()  { return mLowerLayerGoombas; }

protected:
	virtual void boneAnimatorInit(BoneAnimator* animator) final;
	virtual void stateInfoInitalize() final;
	virtual void captureEnterModelOperation() final;

private:
	void setHalfCloseEyeModel();
	void setOpenEyeModel();
	void setPressedModel();
	void setCapturedModel();

	void modelSetting();

	GETSET(Goomba*, mTopGoomba, TopGoomba)

private:
	UINT				mGoombaLayerIdx;
	std::vector<Goomba*> mLowerLayerGoombas;
	Model* mModel;

	Goomba* mTopGoomba;

	PhysXRigidBody* mRigidbody;

	/*
	Body__BodyMT

	EyeBrow__BodyMT
	EyeBrowCap__BodyMT

	EyeClose__BodyMT

	EyeHalfClose__BodyMT
	EyeHalfClose__EyeLMT
	EyeHalfClose__EyeRMT

	EyeOpen__BodyMT
	EyeOpen__EyeLMT
	EyeOpen__EyeRMT

	Mustache__HairMT

	PressModel__BodyMT	
	*/


};

