#include "PlayerScript.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "Transform.h"
#include "PxFilter.h"
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

	}
	void PlayerScript::fixedUpdate()
	{
		Vector3 Pos = {};

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

		if (KEY_DOWN(UP))
		{
			Pos = mTransform->Forward() * 5.f * DT;
		}

		if (KEY_DOWN(DOWN))
		{
			Pos = mTransform->Forward() * -5.f * DT;
		}

		if (KEY_DOWN(LEFT))
		{
			Pos = mTransform->Right() * -5.f * DT;
		}

		if (KEY_DOWN(RIGHT))
		{
			Pos = mTransform->Right() * 5.f * DT;
		}

		PxFilter filter = PxFilter();

		PxFilterData filterData = {};
		filterData.word0 = 1 << 0;
		filterData.word1 = 1 << 1;

	}
	void PlayerScript::render()
	{
	}
	void PlayerScript::fontRender()
	{
	}
	void PlayerScript::OnCollisionEnter(Collider2D* oppo)
	{
	}
	void PlayerScript::OnCollision(Collider2D* oppo)
	{
	}
	void PlayerScript::OnCollisionExit(Collider2D* oppo)
	{
	}
	void PlayerScript::OnTriggerEnter(Collider2D* oppo)
	{
	}
	void PlayerScript::OnTrigger(Collider2D* oppo)
	{
	}
	void PlayerScript::OnTriggerExit(Collider2D* oppo)
	{
	}
}