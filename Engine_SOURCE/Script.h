#pragma once
#include "Component.h"
#include "Collider2D.h"

namespace dru
{
	class Script : public Component
	{
	public:
		Script();
		virtual ~Script();

		virtual void Initialize() override;
		virtual void update() override;
		virtual void fixedUpdate() override;
		virtual void render() override;
		virtual void fontRender();

		virtual void OnCollisionEnter(Collider2D* _oppo) {};
		virtual void OnCollision(Collider2D* _oppo) {};
		virtual void OnCollisionExit(Collider2D* _oppo) {};

		virtual void OnTriggerEnter(Collider2D* _oppo) {};
		virtual void OnTrigger(Collider2D* _oppo) {};
		virtual void OnTriggerExit(Collider2D* _oppo) {};



	};
}


