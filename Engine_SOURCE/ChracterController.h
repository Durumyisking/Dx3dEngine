#pragma once
#include "Component.h"
class ChracterController :
    public Component
{
public:
    ChracterController();
    virtual ~ChracterController();

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void FixedUpdate() override;
    virtual void Render() override;

	void Create();

	void Move(math::Vector3 displacement);

	void SetController(physx::PxController* _controller) { mController = _controller; }
	auto GetHeight() { return mHeight; }
	auto GetRadius() { return mRadius; }

	bool GetGravityState() { return mbUseGravity; }
	void SetGravityState(bool state) { mbUseGravity = state; }

private:
	physx::PxController* mController = nullptr;

	float mHeight;
	float mRadius;

	bool mbUseGravity;
};

