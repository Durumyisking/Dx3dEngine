#pragma once
#include "MarioParts.h"


class Player;
class MarioCap :
    public DynamicObject
{
public:
	enum class eCapState : short
	{
		Idle,
		Throw,
		Return,
		Capture,
		Die,
	};

	MarioCap();
	virtual ~MarioCap();

	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;
	virtual void Render() final;
	virtual void FontRender() final;

	void FlyStart();
	void FlyEnd();

public:
	void Physicalinit();
	virtual void OnCollisionEnter(GameObj* gameObject) final;
	virtual void OnTriggerEnter(GameObj* gameObject) final;
	virtual void OnTriggerExit(GameObj* gameObject) final;

	void boneAnimatorInit(BoneAnimator* animator);

	eCapState GetCapState() { return mCapState; }
	void SetCapState(eCapState capState);

public:
	virtual void BoneInitialize() {};

	GETSET(GameObj*, mOwner, Owner)

protected:
	virtual void stateInfoInitalize();

private: 
	eCapState mCapState;
	GameObj* mOwner;
};

