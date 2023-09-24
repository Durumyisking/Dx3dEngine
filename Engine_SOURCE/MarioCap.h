#pragma once
#include "MarioParts.h"
class MarioCap :
    public DynamicObject
{
public:
	MarioCap();
	virtual ~MarioCap();

	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;
	virtual void Render() final;
	virtual void FontRender() final;

public:
	void Physicalinit();
	virtual void OnCollisionEnter(GameObj* gameObject) final;
	virtual void OnTriggerEnter(GameObj* gameObject) final;
	virtual void OnTriggerExit(GameObj* gameObject) final;

	void boneAnimatorInit(BoneAnimator* animator);

public:
	virtual void BoneInitialize() {};

protected:
	virtual void stateInfoInitalize() {};

};

