#include "PlayerScript.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "Transform.h"
#include "GameObj.h"

namespace dru
{
	PlayerScript::PlayerScript()
		: mTransform(nullptr)
	{
	}
	PlayerScript::~PlayerScript()
	{
	}
	void PlayerScript::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();

	}
	void PlayerScript::update()
	{
		if (KEY_DOWN(A))
		{
			mTransform->AddRotationX(50.f * DT);
		}
		if (KEY_DOWN(S))
		{
			mTransform->AddRotationY(50.f * DT);
		}
		if (KEY_DOWN(D))
		{
			mTransform->AddRotationZ(50.f * DT);
		}

	}
	void PlayerScript::fixedUpdate()
	{
	}
	void PlayerScript::render()
	{
	}
	void PlayerScript::fontRender()
	{
	}
	void PlayerScript::OnCollisionEnter(Collider2D* _oppo)
	{
	}
	void PlayerScript::OnCollision(Collider2D* _oppo)
	{
	}
	void PlayerScript::OnCollisionExit(Collider2D* _oppo)
	{
	}
	void PlayerScript::OnTriggerEnter(Collider2D* _oppo)
	{
	}
	void PlayerScript::OnTrigger(Collider2D* _oppo)
	{
	}
	void PlayerScript::OnTriggerExit(Collider2D* _oppo)
	{
	}
}