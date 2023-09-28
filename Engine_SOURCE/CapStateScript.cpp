#include "CapStateScript.h"
#include "MarioCap.h"
#include "Physical.h"
#include "PhysXRigidBody.h"
#include "PhysicalMovement.h"

#include "InputMgr.h"
#include "TimeMgr.h"

CapStateScript::CapStateScript()
	: Script()
	, mStateEventList{}
	, mCap(nullptr)
	, mbAnimationRunning(false)
{
	// �޸� ���� Ȯ��
	mStateEventList.reserve(static_cast<UINT>(MarioCap::eCapState::Die) + 1);

	// �̺�Ʈ ���ε�
	mStateEventList.emplace_back(std::bind(&CapStateScript::Idle, this));
	mStateEventList.emplace_back(std::bind(&CapStateScript::Throw, this));
	mStateEventList.emplace_back(std::bind(&CapStateScript::Return, this));
	mStateEventList.emplace_back(std::bind(&CapStateScript::Capture, this));
	mStateEventList.emplace_back(std::bind(&CapStateScript::Die, this));

}
CapStateScript::~CapStateScript()
{
	mStateEventList.clear();
}

void CapStateScript::Update()
{
	if (mCap == nullptr)
		return;

	UINT iState = static_cast<UINT>(mCap->GetCapState());
	// enum ���¿� ��Ī�Ǵ� �迭�� �ε����� ����
	mStateEventList[iState]();

	Script::Update();
}

void CapStateScript::Initialize()
{
	// Owner �� ��ȯ
	if (GetOwner())
		mCap = dynamic_cast<MarioCap*>(GetOwner());

	mAnimator = mCap->GetComponent<BoneAnimator>();
	assert(mAnimator);
	
}

void CapStateScript::Idle()
{
	//BoneAnimator* animator = mCap->GetComponent<BoneAnimator>();
	//if (animator == nullptr)
	//	return;
	//if (!(animator->PlayAnimationName() == L"Wait"))
	//	animator->Play(L"Wait");
	//mInitialForce = 33.f;
	/*if (mCap->GetState() == GameObj::eState::Paused)
	{
		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioHead");
		model->MeshRenderSwtich(L"Cap__CapMT-mesh", true);
	}*/
	mCap->Pause();

}

void CapStateScript::Throw()
{

	/*if (!(animator->PlayAnimationName() == L"ThrowCap"))
	{
		mCap->GetComponent<BoneAnimator>()->Play(L"ThrowCap", false);
	}*/

	Transform* tr = mCap->GetComponent<Transform>();
	assert(tr);

	PhysXRigidBody* rigidbody = mCap->GetComponent<PhysXRigidBody>();
	assert(rigidbody);

	//rigidbody->SetTurnSpeed(150.f);
	//rigidbody->RightTrun();

	/*rigidbody->SetMaxVelocity(30.f);
	rigidbody->AddForce(-(tr->Forward()) * 2000.f);*/
}

void CapStateScript::Return()
{
	if (mAnimator->PlayAnimationName() != L"CatchCap")
	{
		mAnimator->Play(L"CatchCap", false);
	}
}

void CapStateScript::Capture()
{
	//if (mAnimator->PlayAnimationName() != L"Capture")
	//{
	//	mAnimator->Play(L"Capture", false);
	//}

}

void CapStateScript::Die()
{
}

