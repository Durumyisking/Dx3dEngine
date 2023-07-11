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

		virtual void Initialize() override;
		virtual void update() override;
		virtual void fixedUpdate() override;
		virtual void render() override;
		virtual void fontRender();

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

