#include "CapUI.h"
#include "TimeMgr.h"
#include "SpriteRenderer.h"

CapUI::CapUI()
{

}

CapUI::CapUI(const std::wstring& key, eUIType type)
	:HUD(type)
{
	SpriteRenderer* hudRender = this->AddComponent<SpriteRenderer>(eComponentType::UI);
	hudRender->SetMeshByKey(L"Rectmesh");
	hudRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(key));
}

CapUI::~CapUI()
{

}


void CapUI::OnUpdate()
{
	Move();
}

void CapUI::OnClear()
{

}

void CapUI::Move()
{
	if (mTargetPos == Vector3::Zero)
		return;

	if (mCount >= 2)
	{
		mCurrentTime += DT;

		if (mCurrentTime >= 1)
		{
			mCount = 0;
			mCurrentTime = 0;
		}

		return;
	}

	Transform* tr = this->GetComponent<Transform>();

	Vector3 pos = tr->GetPosition();
	Vector3 scale = tr->GetScale();


	if (mbGoAndReturn == false)
	{
		if (fabs(mTargetPos.x - pos.x) <= 0.01f)
			mbGoAndReturn = true;

		pos.x += DT / 3;
		scale.x -= DT / 3;
	}
	else if (mbGoAndReturn)
	{
		if (fabs(mOriginPos.x - pos.x) <= 0.01f)
		{
			mbGoAndReturn = false;
			mCount++;
		}

		pos.x -= DT / 3;
		scale.x += DT / 3;
	}

	tr->SetPosition(pos);
	tr->SetScale(scale);
}
