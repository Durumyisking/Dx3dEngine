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

		virtual void OnCollisionEnter(Collider2D* oppo);
		virtual void OnCollision(Collider2D* oppo);
		virtual void OnCollisionExit(Collider2D* oppo);

		virtual void OnTriggerEnter(Collider2D* oppo);
		virtual void OnTrigger(Collider2D* oppo);
		virtual void OnTriggerExit(Collider2D* oppo);

	private:
		Transform* mTransform;
    };
}

