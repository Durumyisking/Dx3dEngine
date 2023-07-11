#pragma once
#include "Script.h"

namespace dru
{
    class PlayerScript :
        public Script
    {
    public:
		PlayerScript();
		virtual ~PlayerScript();

		virtual void Initialize() final;
		virtual void update() final;
		virtual void fixedUpdate() final;
		virtual void render() final;
		virtual void fontRender() final;

		virtual void OnCollisionEnter(Collider2D* _oppo);
		virtual void OnCollision(Collider2D* _oppo);
		virtual void OnCollisionExit(Collider2D* _oppo);

		virtual void OnTriggerEnter(Collider2D* _oppo);
		virtual void OnTrigger(Collider2D* _oppo);
		virtual void OnTriggerExit(Collider2D* _oppo);

	private:
		Transform* mTransform;
    };
}

