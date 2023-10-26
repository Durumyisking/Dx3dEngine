#include "Monster.h"
#include "MonsterAiScript.h"
#include "MonsterStateScript.h"
#include "BoneAnimator.h"
#include "PhysXRigidBody.h"
#include "GenericAnimator.h"
#include "MarioCap.h"
#include "TimeMgr.h"
#include "Player.h"
#include "Physical.h"

Monster::Monster()
	: DynamicObject()
	, mMonsterState(eMonsterState::Idle)
	, mRecognizeRadius(10.f)
	, mFoundPlayer(false)
{
	//StateInfoSetting
	mStateInfo.resize(static_cast<int>(eMonsterState::Die) + 1);

	mObjectTypeName = "Monster";
}

Monster::Monster(const Monster& Obj)
	: DynamicObject(Obj)
	, mMonsterState(Obj.mMonsterState)
	, mRecognizeRadius(Obj.mRecognizeRadius)
	, mFoundPlayer(Obj.mFoundPlayer)
{
	//StateInfoSetting
	mStateInfo.resize(static_cast<int>(eMonsterState::Die) + 1);
}

Monster::~Monster()
{
}

Monster* Monster::Clone() const
{
	return new Monster(*this);
}

void Monster::Save(FILE* File)
{
	DynamicObject::Save(File);
}

void Monster::Load(FILE* File)
{
	DynamicObject::Load(File);
}

void Monster::Initialize()
{
	// Animator
	BoneAnimator* animator = AddComponent<BoneAnimator>(eComponentType::BoneAnimator);
	if (animator != nullptr)
	{
		boneAnimatorInit(animator);
	}

	// OffsetScale Setting
	Transform* tr = GetComponent<Transform>();
	if (tr)
		tr->SetOffsetScale(0.01f);

	GameObj::Initialize();
}

void Monster::Update()
{
	CaptureObj::Update();
	GameObj::Update();
}

void Monster::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void Monster::OnTriggerEnter(GameObj* gameObject)
{
	if (eLayerType::Cap == gameObject->GetLayerType())
	{
		MarioCap* cap = dynamic_cast<MarioCap*>(gameObject);
		OnCapture();
		SetCap(cap);
		SetPlayer(dynamic_cast<Player*>(cap->GetOwner()));
		CaptureEnter(cap);
	}
}

void Monster::OnTriggerPersist(GameObj* gameObject)
{
}

void Monster::OnTriggerExit(GameObj* gameObject)
{
}


void Monster::SetMonsterState(eMonsterState monsterState)
{
	std::set<UINT>::iterator iter;
	iter = mStateInfo[static_cast<UINT>(mMonsterState)].mLockState.find(static_cast<UINT>(monsterState));
	if (iter != mStateInfo[static_cast<UINT>(mMonsterState)].mLockState.end())
		return;

	mMonsterState = monsterState;

	MonsterStateScript* script = GetScript<MonsterStateScript>();
	if (script)
		script->Reset();
}

void Monster::CaptureEnter(MarioCap* cap)
{
	// 캡쳐 연출부
	// 이전에 진행중이던 애니메이터 종료
	GenericAnimator* animator = cap->GetOwner()->GetComponent<GenericAnimator>();
	if (animator)
		animator->Stop();

	// 몬스터와 플레이어의 포지션을 가져옴
	Vector3 monpos = GetTransform()->GetPhysicalPosition();
	Transform* tr = cap->GetOwner()->GetTransform();
	Vector3 playerpos = tr->GetPhysicalPosition();

	//수직 계산
	float dh = monpos.y - playerpos.y;
	float mh = 5.f - playerpos.y;
	float ty = sqrt(2 * 9.8f * mh);

	//수평 계산
	float a = 9.8f;
	float b = -2 * ty;
	float c = 2 * dh;
	float dat = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);

	float tx = -(playerpos.x - monpos.x) / dat;
	float tz = -(playerpos.z - monpos.z) / dat;
	Vector3 right = tr->Right();
	Vector3 forward = tr->Forward();

	AnimatorParam param;

	// 진행타입
	param.AnimType = eAnimType::Linear;

	// 진행중 사용될 Value 값
	param.StartValue = 0.f;
	param.EndValue = 250.f;

	// 진행시간
	param.DurationTime = 0.4f;

	Vector3 Initvelocity = Vector3(tx, ty, tz);

	//cap->GetOwner()->GetBoneAnimator()->Play(L"Bind");

	// 진행 함수 std::function<void(float)>
	param.DurationFunc = [this, cap, tr, playerpos, right, forward, Initvelocity](float inCurValue)
	{
		tr->SetPhysicalPosition(
			Vector3(
				(playerpos.x + (Initvelocity.x * inCurValue * DT)),
				(playerpos.y + (Initvelocity.y * inCurValue * DT) - (0.5f * 9.8f * inCurValue * DT * inCurValue * DT)),
				(playerpos.z + (Initvelocity.z * inCurValue * DT))
			));	

		
		tr->SetPhysicalRotation(right * inCurValue);

	};

	 //끝날때 호출되는 함수
	param.CompleteFunc = [this, cap](float inCurValue)
	{
		OnCapture();
		captureEnterModelOperation();

		// 마리오 본체 pause
		cap->GetOwner()->Pause();
		cap->GetPhysical()->KinematicActorSleep();
		//cap->Pause();
		Player* player = dynamic_cast<Player*>(cap->GetOwner());
		SetPlayer(player);
		player->CapturingProcess();


		// 캡의 오너변경
		cap->SetOwner(this);
		SetCap(cap);

		// 캡의 Capture 상태
		cap->SetCapState(MarioCap::eCapState::Capture);

		// 몬스터의 default 
		SetMonsterState(eMonsterState::Idle);

	};

	//이벤트 시작
	animator->Start(param);

}