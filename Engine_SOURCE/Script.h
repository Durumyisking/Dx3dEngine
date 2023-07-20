#pragma once
#include "Component.h"



using namespace math;

class Transform;
class Collider2D;
class Script : public Component
{
public:
	Script();
	virtual ~Script();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;
	virtual void fontRender();

	virtual void OnCollisionEnter(Collider2D* oppo) {};
	virtual void OnCollision(Collider2D* oppo) {};
	virtual void OnCollisionExit(Collider2D* oppo) {};

	virtual void OnTriggerEnter(Collider2D* oppo) {};
	virtual void OnTrigger(Collider2D* oppo) {};
	virtual void OnTriggerExit(Collider2D* oppo) {};


};
