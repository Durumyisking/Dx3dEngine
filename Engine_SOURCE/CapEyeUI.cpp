#include "CapEyeUI.h"
#include "SpriteRenderer.h"
#include "TimeMgr.h"
#include "Animator.h"

CapEyeUI::CapEyeUI()
	:mbEyeblink(false)
{

}

CapEyeUI::CapEyeUI(const std::wstring& key, eUIType type)
{
	SpriteRenderer* hudRender = this->AddComponent<SpriteRenderer>(eComponentType::UI);
	hudRender->SetMeshByKey(L"Rectmesh");
	hudRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(key));
}

CapEyeUI::~CapEyeUI()
{

}

void CapEyeUI::OnUpdate()
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

			if (mbEyeblink)
			{
				this->GetComponent<Animator>()->Play(L"CapEyeAni", false);
				mbEyeblink = false;
			}

			mbEyeblink = true;
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
	}
	else if (mbGoAndReturn)
	{
		if (fabs(mOriginPos.x - pos.x) <= 0.01f)
		{
			mbGoAndReturn = false;
			mCount++;
		}

		pos.x -= DT / 3;
	}

	tr->SetPosition(pos);
	tr->SetScale(scale);
}

void CapEyeUI::OnClear()
{

}
