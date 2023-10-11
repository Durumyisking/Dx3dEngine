#include "Monster.h"
#include "MonsterAiScript.h"
#include "MonsterStateScript.h"
#include "BoneAnimator.h"
#include "GenericAnimator.h"
#include "MarioCap.h"
#include "TimeMgr.h"
#include "Player.h"

Monster::Monster()
	: DynamicObject()
	, mMonsterState(eMonsterState::Idle)
	, mRecognizeRadius(10.f)
	, mFoundPlayer(false)
{
	//StateInfoSetting
	mStateInfo.resize(static_cast<int>(eMonsterState::Die) + 1);

}

Monster::~Monster()
{
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

void Monster::OnTriggerStay(GameObj* gameObject)
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
	// ĸ�� �����
	// ������ �������̴� �ִϸ����� ����
	GenericAnimator* animator = cap->GetOwner()->GetComponent<GenericAnimator>();
	if (animator)
		animator->Stop();

	// ���Ϳ� �÷��̾��� �������� ������
	Vector3 monpos = GetTransform()->GetPhysicalPosition();
	Transform* tr = cap->GetOwner()->GetTransform();
	Vector3 playerpos = tr->GetPhysicalPosition();

	//���� ���
	float dh = monpos.y - playerpos.y;
	float mh = 5.f - playerpos.y;
	float ty = sqrt(2 * 9.8f * mh);

	//���� ���
	float a = 9.8f;
	float b = -2 * ty;
	float c = 2 * dh;
	float dat = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);

	float tx = -(playerpos.x - monpos.x) / dat;
	float tz = -(playerpos.z - monpos.z) / dat;
	Vector3 right = tr->Right();
	Vector3 forward = tr->Forward();

	AnimatorParam param;

	// ����Ÿ��
	param.AnimType = eAnimType::Linear;

	// ������ ���� Value ��
	param.StartValue = 0.f;
	param.EndValue = 250.f;

	// ����ð�
	param.DurationTime = 0.4f;

	Vector3 Initvelocity = Vector3(tx, ty, tz);

	//cap->GetOwner()->GetBoneAnimator()->Play(L"Bind");

	// ���� �Լ� std::function<void(float)>
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

	 //������ ȣ��Ǵ� �Լ�
	param.CompleteFunc = [this, cap](float inCurValue)
	{
		OnCapture();

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"Packun");
		if (!model)
			return;

		// ����
		model->MeshRenderSwtich(L"Head2__BodyMT-mesh", true);
		model->MeshRenderSwtich(L"Head2__HeadMT-mesh", true);
		model->MeshRenderSwtich(L"mustache__HairMT-mesh", true);

		// ������ ��ü pause
		cap->GetOwner()->Pause();
		SetPlayer(dynamic_cast<Player*>(cap->GetOwner()));

		// ĸ�� ���ʺ���
		cap->SetOwner(this);
		SetCap(cap);

		// ĸ�� Capture ����
		cap->SetCapState(MarioCap::eCapState::Capture);

		// ������ default 
		SetMonsterState(eMonsterState::Idle);

	};

	//�̺�Ʈ ����
	animator->Start(param);

}