#include "Packun.h"
#include "Object.h"
#include "InputMgr.h"
#include "BoneAnimator.h"
#include "Model.h"
#include "ResourceMgr.h"
#include "MeshRenderer.h"
#include "PackunStateScript.h"
#include "PhysXRigidBody.h"
#include "Physical.h"
#include "PhysXCollider.h"
#include "PhysicalMovement.h"

#include "PackunPostionBall.h"
#include "MarioCap.h"

#include "GenericAnimator.h"
#include "Player.h"
#include "TimeMgr.h"

Packun::Packun()
	: Monster()
{

}

Packun::~Packun()
{
}

void Packun::Initialize()
{
	// Add MeshRenderer
	MeshRenderer* mesh = AddComponent<MeshRenderer>(eComponentType::MeshRenderer);

	// SetModel
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"Packun");
	if (model)
	{
		MeshRenderer* test = GetComponent<MeshRenderer>();
		test->SetModel(model, model->GetMaterial(0));


		//// 오프
		model->MeshRenderSwtich(L"Head2__BodyMT-mesh", false);
		model->MeshRenderSwtich(L"Head2__HeadMT-mesh", false);
		model->MeshRenderSwtich(L"mustache__HairMT-mesh", false);

		//// 온ㄴ
		//model->MeshRenderSwtich(L"Head3__BodyMT-mesh");
		//model->MeshRenderSwtich(L"Head3__HeadMT-mesh");
	}


	assert(AddComponent<PackunStateScript>(eComponentType::Script));

	//Phsical
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Capsule, Vector3(0.5f, 0.5f, 0.5f));
	physical->CreateSubShape(Vector3::Zero, eGeometryType::Capsule, Vector3(0.5f, 0.5f, 0.5f), PxShapeFlag::eTRIGGER_SHAPE);


	// Rigidbody
	assert(AddComponent<PhysXRigidBody>(eComponentType::RigidBody));

	// MoveMent
	assert(AddComponent<PhysXCollider>(eComponentType::Collider));
	assert(AddComponent<PhysicalMovement>(eComponentType::Movement));

	// 초기화
	Monster::Initialize();

	// state초기화
	stateInfoInitalize();
}

void Packun::Update()
{
	Monster::Update();
}

void Packun::FixedUpdate()
{
	Monster::FixedUpdate();
}

void Packun::CaptureEvent()
{
	// 캡처 이벤트 구현부
	bool able = false;

	std::vector<std::function<bool(eKeyCode)>> keyEvent;
	keyEvent.resize((static_cast<UINT>(eKeyState::NONE) + 1));

	keyEvent[static_cast<UINT>(eKeyState::TAP)] = std::bind(&InputMgr::GetKeyTap, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::DOWN)] = std::bind(&InputMgr::GetKeyDown, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::UP)] = std::bind(&InputMgr::GetKeyUp, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::NONE)] = std::bind(&InputMgr::GetKeyNone, GETSINGLE(InputMgr), std::placeholders::_1);

	// 키 입력 이벤트 처리하는 람다식
	std::function<void(eKeyState, eKeyCode, eMonsterState)> stateEvent =
		[&]
		(eKeyState keyState, eKeyCode curPress, eMonsterState nextState) ->void
		{
			if (able)
				return;
			if (keyEvent[static_cast<UINT>(keyState)](curPress))
			{
				SetMonsterState(nextState);
				able = true;
			}
		};

	// 이동
	stateEvent(eKeyState::DOWN, eKeyCode::UP, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::DOWN, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::LEFT, eMonsterState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::RIGHT, eMonsterState::Move);

	// 공격
	able = false;
	stateEvent(eKeyState::TAP, eKeyCode::SPACE, eMonsterState::Attack);
}

void Packun::CaptureEnter(MarioCap* cap)
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

		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"Packun");
		if (!model)
			return;

		// 오프
		model->MeshRenderSwtich(L"Head2__BodyMT-mesh", true);
		model->MeshRenderSwtich(L"Head2__HeadMT-mesh", true);
		model->MeshRenderSwtich(L"mustache__HairMT-mesh", true);

		// 마리오 본체 pause
		cap->GetOwner()->Pause();
		SetPlayer(dynamic_cast<Player*>(cap->GetOwner()));

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

void Packun::DivideEvent()
{
	// 마지막 상태가 어떤지 모르기때문에 idle 로 초기화후
	// groggy로 상태변경
	SetMonsterState(eMonsterState::Idle);
	SetMonsterState(eMonsterState::Groggy);
}

void Packun::OnTriggerEnter(GameObj* gameObject)
{
	if (!gameObject)
		return;

	// 마리오 모자와 충돌시 캡처 상태로 변경
	if (gameObject->GetLayerType() == eLayerType::Player)
	{
		if (IsCapture())
			return;

		MarioCap* cap = dynamic_cast<MarioCap*>(gameObject);
		if (cap == nullptr)
			return;

		// 진행중인 애니메이터 초기화
		GenericAnimator* animator = cap->GetComponent<GenericAnimator>();
		if (animator)
			animator->Stop();

		dynamic_cast<Player*>(cap->GetOwner())->SetPlayerState(Player::ePlayerState::Capture);
		
		//CaptureEnter(cap);
	
	}
}

void Packun::stateInfoInitalize()
{
	mStateInfo.resize(static_cast<UINT>(eMonsterState::Die) + 1);
	//Idle
	// 현재는 대기상태에서 못가는상태가 없다

	// Jump
	InsertLockState(static_cast<UINT>(eMonsterState::Jump), static_cast<UINT>(eMonsterState::Move));
	InsertLockState(static_cast<UINT>(eMonsterState::Jump), static_cast<UINT>(eMonsterState::SpecialSituation));
	InsertLockState(static_cast<UINT>(eMonsterState::Jump), static_cast<UINT>(eMonsterState::Groggy));
	InsertLockState(static_cast<UINT>(eMonsterState::Jump), static_cast<UINT>(eMonsterState::Die));

	//Attack
	InsertLockState(static_cast<UINT>(eMonsterState::Attack), static_cast<UINT>(eMonsterState::Attack));
	InsertLockState(static_cast<UINT>(eMonsterState::Attack), static_cast<UINT>(eMonsterState::Jump));
	InsertLockState(static_cast<UINT>(eMonsterState::Attack), static_cast<UINT>(eMonsterState::Move));
	InsertLockState(static_cast<UINT>(eMonsterState::Attack), static_cast<UINT>(eMonsterState::SpecialSituation));

	//SpecialCast
	InsertLockState(static_cast<UINT>(eMonsterState::SpecialSituation), static_cast<UINT>(eMonsterState::Attack));
	InsertLockState(static_cast<UINT>(eMonsterState::SpecialSituation), static_cast<UINT>(eMonsterState::Jump));
	InsertLockState(static_cast<UINT>(eMonsterState::SpecialSituation), static_cast<UINT>(eMonsterState::Move));

	// Hit
	InsertLockState(static_cast<UINT>(eMonsterState::Hit), static_cast<UINT>(eMonsterState::Move));
	InsertLockState(static_cast<UINT>(eMonsterState::Hit), static_cast<UINT>(eMonsterState::Jump));
	InsertLockState(static_cast<UINT>(eMonsterState::Hit), static_cast<UINT>(eMonsterState::Attack));
	InsertLockState(static_cast<UINT>(eMonsterState::Hit), static_cast<UINT>(eMonsterState::SpecialSituation));

	// Groggy
	InsertLockState(static_cast<UINT>(eMonsterState::Groggy), static_cast<UINT>(eMonsterState::Move));
	InsertLockState(static_cast<UINT>(eMonsterState::Groggy), static_cast<UINT>(eMonsterState::Jump));
	InsertLockState(static_cast<UINT>(eMonsterState::Groggy), static_cast<UINT>(eMonsterState::Attack));
	InsertLockState(static_cast<UINT>(eMonsterState::Groggy), static_cast<UINT>(eMonsterState::SpecialSituation));

	// Die
	InsertLockState(static_cast<UINT>(eMonsterState::Die), static_cast<UINT>(eMonsterState::Idle));
	InsertLockState(static_cast<UINT>(eMonsterState::Die), static_cast<UINT>(eMonsterState::Move));
	InsertLockState(static_cast<UINT>(eMonsterState::Die), static_cast<UINT>(eMonsterState::Jump));
	InsertLockState(static_cast<UINT>(eMonsterState::Die), static_cast<UINT>(eMonsterState::Attack));
	InsertLockState(static_cast<UINT>(eMonsterState::Die), static_cast<UINT>(eMonsterState::SpecialSituation));
	InsertLockState(static_cast<UINT>(eMonsterState::Die), static_cast<UINT>(eMonsterState::Groggy));
}

void Packun::boneAnimatorInit(BoneAnimator* animator)
{
	animator->LoadAnimations(L"..//Resources/Packun/Animation");
	AnimationClip* cilp = nullptr;

	// 어택 동작 연계
	{
		cilp = animator->GetAnimationClip(L"Attack");
		if (cilp)
		{
			cilp->SetKeyFrameEvent(3, [this]()
				{
					PackunPostionBall* packunball = object::LateInstantiate<PackunPostionBall>(eLayerType::Objects);
					packunball->Initialize();

					Transform* tr = GetComponent<Transform>();
					Vector3 position = tr->GetPhysicalPosition();
					Vector3 rotation = tr->GetRotation();

					packunball->GetComponent<Transform>()->SetPhysicalPosition(position);
					packunball->GetComponent<Transform>()->SetPhysicalRotation(rotation);

					PhysXRigidBody* rigidbody = packunball->GetComponent<PhysXRigidBody>();
					if (rigidbody)
					{
						rigidbody->SetMaxVelocity_Y(5.f);
						rigidbody->ApplyGravity();
						rigidbody->SetAirOn();


						Vector3 force = -tr->WorldForward() * 350.f;
						force.y = 999999.f;

						rigidbody->AddForce(force);
					}

				});

			cilp->SetCompleteEvent([this]()
				{
					if (IsCapture())
						SetMonsterState(Monster::eMonsterState::Idle);
					else
						SetMonsterState(Monster::eMonsterState::Groggy);

				});
		}

	}

	// 피격 동작 연계
	{
		cilp = animator->GetAnimationClip(L"AttackHit");
		if (cilp)
			cilp->SetCompleteEvent([this]()
				{
					SetMonsterState(Monster::eMonsterState::Idle);
				});
	}


	{
		cilp = animator->GetAnimationClip(L"HackWait");
		if (cilp)
			cilp->SetCompleteEvent([this]()
				{
					SetMonsterState(Monster::eMonsterState::Idle);
				});
	}

	// 죽었을때
	{
		cilp = animator->GetAnimationClip(L"PressDown");
		if (cilp)
			cilp->SetCompleteEvent([this, cilp]()
				{
					cilp->SetDuration(static_cast<double>(1.f / 30.f));
					Die();
					//GetComponent<Physical>()->RemoveActorToPxScene();
				});
	}
}
