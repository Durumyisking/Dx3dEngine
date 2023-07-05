#include "PlayerScript.h"
#include "Input.h"
#include "TimeMgr.h"
#include "Transform.h"

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
		if (Input::GetKeyState(eKeyCode::A) == eKeyState::DOWN)
		{
			mTransform->AddRotationX(50.f * TimeMgr::DeltaTime());
		}
		if (Input::GetKeyState(eKeyCode::S) == eKeyState::DOWN)
		{
			mTransform->AddRotationY(50.f * TimeMgr::DeltaTime());
		}
		if (Input::GetKeyState(eKeyCode::D) == eKeyState::DOWN)
		{
			mTransform->AddRotationZ(50.f * TimeMgr::DeltaTime());
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