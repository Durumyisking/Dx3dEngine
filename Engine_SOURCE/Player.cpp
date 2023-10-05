#include "Player.h"
#include "MarioParts.h"
#include "MarioCap.h"
#include "InputMgr.h"
#include "TimeMgr.h"

#include "MeshRenderer.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"
#include "PhysicalMovement.h"
#include "BoneAnimator.h"
#include "Object.h"

#include "PlayerStateScript.h"
#include "PlayerScript.h"

#include "ParticleSystem.h"

Player::Player()
{
	SetLayerType(eLayerType::Player);
	SetName(L"Player");

//		RigidBody* rigidbody = this->AddComponent<RigidBody>(eComponentType::RigidBody);

}

Player::~Player()
{
	for (MarioParts* parts : mParts)
	{
		if (!parts)
			continue;

		delete parts;
		parts = nullptr;
	}

	//if (mMarioCap)
	//{
	//	delete mMarioCap;
	//	mMarioCap = nullptr;
	//}
}

void Player::Initialize()
{
	//�⺻ ����
	SetPos(Vector3(20.f, 5.f, 10.f));
	SetScale(Vector3(1.f, 1.f, 1.f));

	//������ body �ʱ�ȭ
	MeshRenderer* mr = AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	//AddComponent<PlayerScript>(eComponentType::Script);
	AddComponent<PlayerStateScript>(eComponentType::Script);
	//AddComponent<Transform>(eComponentType::Transform);
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

	AddComponent<PhysXCollider>(eComponentType::Collider);
	AddComponent<PhysicalMovement>(eComponentType::Movement);

	BoneAnimator* animator = AddComponent<BoneAnimator>(eComponentType::BoneAnimator);

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"Mario");
	assert(model);
	mr->SetModel(model);
	mr->SetMaterialByKey(L"marioBodyMaterial", 0);


	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Capsule, Vector3(0.5f, 1.f, 0.5f));
	physical->CreateSubShape(Vector3(0.f, 0.f, 0.f), eGeometryType::Capsule, Vector3(0.5f, 1.f, 0.5f), PxShapeFlag::eTRIGGER_SHAPE);

	rigid->SetFriction(Vector3(40.f, 0.f, 40.f));

	// OffsetScale Setting
	Transform* tr = GetComponent<Transform>();
	if (tr)
		tr->SetOffsetScale(0.01f);



	//animation setting
	boneAnimatorInit(animator);

	//������ ���� ����
	MarioParts* mHandL = new MarioParts();
	MarioParts* mHandR = new MarioParts();
	MarioParts* mHead = new MarioParts();
	MarioParts* mFace = new MarioParts();
	MarioParts* mEye = new MarioParts();

	mHandL->SetName(L"HandL");
	mHandR->SetName(L"HandR");
	mHead->SetName(L"Head");
	mFace->SetName(L"Face");
	mEye->SetName(L"Eye");

	mParts.push_back(mHandL);
	mParts.push_back(mHandR);
	mParts.push_back(mHead);
	mParts.push_back(mFace);
	mParts.push_back(mEye);

	for (auto i : mParts)
	{
		i->Initialize();
	}

	ParticleSystem* particle = AddComponent<ParticleSystem>(eComponentType::Particle);
	particle->InsertParticle(L"Default", L"CloudParticle");
	particle->SetComputeShader(L"ParticleCS");

	particle->Play(L"Default");



	mStateInfo.resize(static_cast<int>(ePlayerState::Die) + 1);
	stateInfoInitalize();
	//mPlayerState = ePlayerState::Idle;

	//mariocap ����
	//mMarioCap = object::LateInstantiate<MarioCap>(eLayerType::Objects);
	//mMarioCap->Initialize();
	//mMarioCap->Physicalinit();
	DynamicObject::Initialize();


	rigid->SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum::eLOCK_ANGULAR_Z, true);
	rigid->SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum::eLOCK_ANGULAR_X, true);

	mr->SetBoneAnimator(nullptr);
}

void Player::Update()
{
	if (GetState() != GameObj::eState::Active)
		return;

	DynamicObject::Update();

	for (auto i : mParts)
	{
		i->Update();
	}
	//mMarioCap->Update();
}

void Player::FixedUpdate()
{
	if (GetState() != GameObj::eState::Active)
		return;

	DynamicObject::FixedUpdate();

	KeyCheck();

	for (auto i : mParts)
	{
		i->FixedUpdate();
	}

	Transform* transform = GetComponent<Transform>();
	math::Matrix playerWorldMatirx = transform == nullptr ? math::Matrix::Identity : transform->GetWorldMatrix();
	for (auto i : mParts)
	{
		i->GetComponent<Transform>()->SetWorldMatrix(playerWorldMatirx);
	}

	//mMarioCap->FixedUpdate();
}

void Player::Render()
{
	if (GetState() != GameObj::eState::Active)
		return;

	DynamicObject::Render();

	for (auto i : mParts)
	{
		i->Render();
	}

	//mMarioCap->Render();
}

void Player::PrevRender()
{
	if (GetState() != GameObj::eState::Active)
		return;

	DynamicObject::PrevRender();

	for (auto i : mParts)
	{
		i->PrevRender();
	}
}

void Player::FontRender()
{
}

void Player::OnCollisionEnter(GameObj* gameObject)
{


}

void Player::OnTriggerEnter(GameObj* gameObject)
{
	if (eLayerType::Platforms == gameObject->GetLayerType())
	{
		GetPhysXRigidBody()->SetAirOff();
		SetPlayerState(Player::ePlayerState::Idle);

	}

	if (eLayerType::Monster == gameObject->GetLayerType())
	{
		Vector3 monToPlayer = GetWorldPos() - gameObject->GetWorldPos();
		monToPlayer.Normalize();
		Vector3 monUpVector = gameObject->GetTransform()->WorldUp();

		float cosTheta = monToPlayer.Dot(monUpVector);
		if (cosTheta > 0.95f)
		{
			GetPhysXRigidBody()->SetAirOff();
			SetPlayerState(Player::ePlayerState::Jump);
		}
	}

}

void Player::OnTriggerExit(GameObj* gameObject)
{
}

void Player::KeyCheck()
{
	// ĸó �̺�Ʈ ������
	bool able = false;

	std::vector<std::function<bool(eKeyCode)>> keyEvent;
	keyEvent.resize((static_cast<UINT>(eKeyState::NONE) + 1));

	keyEvent[static_cast<UINT>(eKeyState::TAP)] = std::bind(&InputMgr::GetKeyTap, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::DOWN)] = std::bind(&InputMgr::GetKeyDown, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::UP)] = std::bind(&InputMgr::GetKeyUp, GETSINGLE(InputMgr), std::placeholders::_1);
	keyEvent[static_cast<UINT>(eKeyState::NONE)] = std::bind(&InputMgr::GetKeyNone, GETSINGLE(InputMgr), std::placeholders::_1);

	// Ű �Է� �̺�Ʈ ó���ϴ� ���ٽ�
	std::function<void(eKeyState, eKeyCode, ePlayerState)> stateEvent =
		[&]
	(eKeyState keyState, eKeyCode curPress, ePlayerState nextState) ->void
	{
		if (able)
			return;
		if (keyEvent[static_cast<UINT>(keyState)](curPress))
		{
			SetPlayerState(nextState);
			able = true;
		}
	};

	// ���
 
	// �̵�
	stateEvent(eKeyState::DOWN, eKeyCode::UP, ePlayerState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::DOWN, ePlayerState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::LEFT, ePlayerState::Move);
	stateEvent(eKeyState::DOWN, eKeyCode::RIGHT, ePlayerState::Move);

	// ���� ������
	able = false;
	stateEvent(eKeyState::TAP, eKeyCode::LCTRL, ePlayerState::ThrowCap);

	// ����
	stateEvent(eKeyState::TAP, eKeyCode::SPACE, ePlayerState::Jump);

	// ��ũ����
	stateEvent(eKeyState::TAP, eKeyCode::Z, ePlayerState::Squat);

	// Ư��
	able = false;
}

void Player::BoneInitialize()
{
}

//void Player::PlayerAnimation(std::wstring name)
//{
//	//�ϰ������� �ִϸ��̼��� ��������ִ� �Լ�
//	BoneAnimator* animator = GetComponent<BoneAnimator>();
//	animator->Play(name);
//	for (auto i : mParts)
//	{
//		i->GetComponent<BoneAnimator>()->Play(name);
//	}
//}

void Player::stateInfoInitalize()
{
	//Idle
	// ����� �����¿��� �����»��°� ����

	//Move
	
	// Jump
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Move));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::SquatMove));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Groggy));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Die));

	//Squat - ������ ��ũ���� ���¿����� �Ϲ� ������ �ȵȴ�. ������ ����ȭ ���� �����
	InsertLockState(static_cast<UINT>(ePlayerState::Squat), static_cast<UINT>(ePlayerState::Move));
	//InsertLockState(static_cast<UINT>(ePlayerState::Crouch), static_cast<UINT>(ePlayerState::Jump));
	InsertLockState(static_cast<UINT>(ePlayerState::Squat), static_cast<UINT>(ePlayerState::Groggy));
	InsertLockState(static_cast<UINT>(ePlayerState::Squat), static_cast<UINT>(ePlayerState::Squat));
	InsertLockState(static_cast<UINT>(ePlayerState::Squat), static_cast<UINT>(ePlayerState::Die));

	//SquatMove - ��ũ���� �� �����̴� ����, �� ���¿����� �� �� �ִ� �׼ǵ��� �����Ѵ�.
	InsertLockState(static_cast<UINT>(ePlayerState::SquatMove), static_cast<UINT>(ePlayerState::Move));
	InsertLockState(static_cast<UINT>(ePlayerState::SquatMove), static_cast<UINT>(ePlayerState::Wall));

	//Air - ���߿� ���ִ� ���¿����� �������� ���� ȸ���� �����ϸ�, �̵������� ���Ѵ�.
	InsertLockState(static_cast<UINT>(ePlayerState::Air), static_cast<UINT>(ePlayerState::Move));
	InsertLockState(static_cast<UINT>(ePlayerState::Air), static_cast<UINT>(ePlayerState::Jump));
	InsertLockState(static_cast<UINT>(ePlayerState::Air), static_cast<UINT>(ePlayerState::Squat));
	InsertLockState(static_cast<UINT>(ePlayerState::Air), static_cast<UINT>(ePlayerState::SquatMove));

	//Fall
	InsertLockState(static_cast<UINT>(ePlayerState::Fall), static_cast<UINT>(ePlayerState::Move));
	InsertLockState(static_cast<UINT>(ePlayerState::Fall), static_cast<UINT>(ePlayerState::Jump));
	InsertLockState(static_cast<UINT>(ePlayerState::Fall), static_cast<UINT>(ePlayerState::Squat));
	InsertLockState(static_cast<UINT>(ePlayerState::Fall), static_cast<UINT>(ePlayerState::SquatMove));

	//Wall -���� ������ ¤���鼭 �������� ����, �� ���¿��� ������ �ϸ� �����Ⱑ �ȴ�. 
	InsertLockState(static_cast<UINT>(ePlayerState::Wall), static_cast<UINT>(ePlayerState::Move));
	InsertLockState(static_cast<UINT>(ePlayerState::Wall), static_cast<UINT>(ePlayerState::SquatMove));
	InsertLockState(static_cast<UINT>(ePlayerState::Wall), static_cast<UINT>(ePlayerState::Air));
	InsertLockState(static_cast<UINT>(ePlayerState::Wall), static_cast<UINT>(ePlayerState::ThrowCap));

	//Hit
	InsertLockState(static_cast<UINT>(ePlayerState::Hit), static_cast<UINT>(ePlayerState::Move));
	InsertLockState(static_cast<UINT>(ePlayerState::Hit), static_cast<UINT>(ePlayerState::Hit));
	InsertLockState(static_cast<UINT>(ePlayerState::Hit), static_cast<UINT>(ePlayerState::Squat));
	InsertLockState(static_cast<UINT>(ePlayerState::Hit), static_cast<UINT>(ePlayerState::SquatMove));
	InsertLockState(static_cast<UINT>(ePlayerState::Hit), static_cast<UINT>(ePlayerState::Air));
	InsertLockState(static_cast<UINT>(ePlayerState::Hit), static_cast<UINT>(ePlayerState::Wall));
	InsertLockState(static_cast<UINT>(ePlayerState::Hit), static_cast<UINT>(ePlayerState::ThrowCap));

	//Groggy
	InsertLockState(static_cast<UINT>(ePlayerState::Groggy), static_cast<UINT>(ePlayerState::Hit));

	//ThrowCap
	InsertLockState(static_cast<UINT>(ePlayerState::ThrowCap), static_cast<UINT>(ePlayerState::Move));
	InsertLockState(static_cast<UINT>(ePlayerState::ThrowCap), static_cast<UINT>(ePlayerState::Jump));
	InsertLockState(static_cast<UINT>(ePlayerState::ThrowCap), static_cast<UINT>(ePlayerState::Squat));
	InsertLockState(static_cast<UINT>(ePlayerState::ThrowCap), static_cast<UINT>(ePlayerState::SquatMove));
	InsertLockState(static_cast<UINT>(ePlayerState::ThrowCap), static_cast<UINT>(ePlayerState::Air));
	InsertLockState(static_cast<UINT>(ePlayerState::ThrowCap), static_cast<UINT>(ePlayerState::Wall));

	//CatchCap
	InsertLockState(static_cast<UINT>(ePlayerState::CatchCap), static_cast<UINT>(ePlayerState::Move));
	InsertLockState(static_cast<UINT>(ePlayerState::CatchCap), static_cast<UINT>(ePlayerState::Jump));
	InsertLockState(static_cast<UINT>(ePlayerState::CatchCap), static_cast<UINT>(ePlayerState::Squat));
	InsertLockState(static_cast<UINT>(ePlayerState::CatchCap), static_cast<UINT>(ePlayerState::SquatMove));
	InsertLockState(static_cast<UINT>(ePlayerState::CatchCap), static_cast<UINT>(ePlayerState::Air));
	InsertLockState(static_cast<UINT>(ePlayerState::CatchCap), static_cast<UINT>(ePlayerState::Wall));

	//Die
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::Idle));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::Move));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::Jump));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::Squat));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::SquatMove));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::Air));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::Wall));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::Hit));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::Groggy));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::ThrowCap));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::CatchCap));
}
void Player::SetPlayerState(ePlayerState playerState)
{
	std::set<UINT>::iterator iter;
	iter = mStateInfo[static_cast<UINT>(mPlayerState)].mLockState.find(static_cast<UINT>(playerState));
	if (iter != mStateInfo[static_cast<UINT>(mPlayerState)].mLockState.end())
		return;

	mPlayerState = playerState;

	PlayerStateScript* script = GetScript<PlayerStateScript>();
	if (script)
		script->Reset();
}

void Player::boneAnimatorInit(BoneAnimator* animator)
{
	AnimationClip* cilp = nullptr;

	//animator->LoadAnimations(L"..//Resources/MarioBody/Animation");
	animator->CreateAnimation(L"Wait", L"..//..//Resources/MarioBody/Animation/Wait.smd");
	animator->CreateAnimation(L"WaitHot", L"..//..//Resources/MarioBody/Animation/WaitHot.smd");

	animator->CreateAnimation(L"Walk", L"..//..//Resources/MarioBody/Animation/Walk.smd");
	animator->CreateAnimation(L"Brake", L"..//..//Resources/MarioBody/Animation/Brake.smd");
	animator->CreateAnimation(L"Fall", L"..//..//Resources/MarioBody/Animation/Fall.smd");

	animator->CreateAnimation(L"ThrowCap", L"..//..//Resources/MarioBody/Animation/ThrowCap.smd");
	animator->CreateAnimation(L"CatchCap", L"..//..//Resources/MarioBody/Animation/CatchCap.smd");
	animator->CreateAnimation(L"Jump", L"..//..//Resources/MarioBody/Animation/Jump.smd");

	animator->CreateAnimation(L"SquatStart", L"..//..//Resources/MarioBody/Animation/SquatStart.smd");
	animator->CreateAnimation(L"SquatEnd", L"..//..//Resources/MarioBody/Animation/SquatEnd.smd");
	animator->CreateAnimation(L"SquatWait", L"..//..//Resources/MarioBody/Animation/SquatWait.smd");
	animator->CreateAnimation(L"SquatWalk", L"..//..//Resources/MarioBody/Animation/SquatWalk.smd");

	//animator->CreateAnimation(L"Dead", L"..//..//Resources/MarioBody/Animation/Dead.smd", 0.1f);
	animator->CreateAnimation(L"Run", L"..//..//Resources/MarioBody/Animation/Run.smd");
	animator->CreateAnimation(L"RunStart", L"..//..//Resources/MarioBody/Animation/RunStart.smd");
	animator->Play(L"Wait");

	// ���� ������
	{
		cilp = animator->GetAnimationClip(L"ThrowCap");
		if (cilp)
		{
			cilp->SetStartEvent([this]()
			{
				//mMarioCap->SetCapState(MarioCap::eCapState::Throw);
					
				//mMarioCap->Physicalinit();
				Transform* tr = GetComponent<Transform>();

				Vector3 position = tr->GetPhysicalPosition();
				Vector3 rotation = tr->GetRotation();

				mMarioCap->Active();

				mMarioCap->GetComponent<Transform>()->SetPhysicalPosition(position+Vector3(0.f,0.6f,0.f));
				mMarioCap->GetComponent<Transform>()->SetPhysicalRotation(rotation);

				mMarioCap->GetComponent<BoneAnimator>()->Play(L"ThrowCap",false);
				mMarioCap->SetCapState(MarioCap::eCapState::Throw);

				Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioHead");
				model->MeshRenderSwtich(L"Cap__CapMT-mesh", false);
			});

			cilp->SetCompleteEvent([this]() {SetPlayerState(ePlayerState::Idle); });
		}
	}

	// runstart �� run����
	{
		cilp = animator->GetAnimationClip(L"RunStart");
		if (cilp)
			cilp->SetCompleteEvent([animator]()
		{
			animator->Play(L"Run");
		});
	}

	//���ߴ� �ִϸ��̼� �� idle��
	{
		cilp = animator->GetAnimationClip(L"Brake");
		if (cilp)
			cilp->SetCompleteEvent([this]()
		{
			SetPlayerState(Player::ePlayerState::Idle);
		});
	}

	// SquatStart > Squat
	{
		cilp = animator->GetAnimationClip(L"SquatStart");
		if (cilp)
			cilp->SetCompleteEvent([animator]()
		{
			animator->Play(L"SquatWait");
		});
	}

	//CatchCap > idle
	{
		cilp = animator->GetAnimationClip(L"CatchCap");
		if (cilp)
			cilp->SetCompleteEvent([this]()
		{
			SetPlayerState(Player::ePlayerState::Idle);
		});
	}
}

void Player::SetMarioCap(MarioCap* cap)
{
	mMarioCap = cap;
	cap->SetOwner(this);
}
