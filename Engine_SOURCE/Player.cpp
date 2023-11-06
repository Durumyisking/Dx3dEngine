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
#include "GenericAnimator.h"
#include "AudioSource.h"

#include "ParticleSystem.h"

#include "FootSmokeParticle.h"
#include "Object.h"
#include "PlayerBGMScript.h"

#include "BlockBrickScript.h"
#include "MoonScript.h"


Player::Player()
	: mPlayerState(ePlayerState::Idle)
	, mMeshRenderer(nullptr)
	, mScript(nullptr)
	, mRigidBody(nullptr)
	, mParts{}
	, mMarioCap(nullptr)
{
	SetLayerType(eLayerType::Player);
	SetName(L"Player");

//		RigidBody* rigidbody = this->AddComponent<RigidBody>(eComponentType::RigidBody);

	mObjectTypeName = "Player";
}

Player::Player(const Player& Obj)
	: DynamicObject(Obj)
	, mPlayerState(ePlayerState::Idle)
	, mMeshRenderer(nullptr)
	, mScript(nullptr)
	, mRigidBody(nullptr)
	, mParts{}
	, mMarioCap(nullptr)
{
	SetLayerType(eLayerType::Player);
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

Player* Player::Clone() const
{
	return new Player(*this);
}

void Player::Save(FILE* File)
{
	DynamicObject::Save(File);
}

void Player::Load(FILE* File)
{
	DynamicObject::Load(File);
}

void Player::Initialize()
{
	//�⺻ ����
	SetPos(Vector3(20.f, 30.f, 10.f));
	SetScale(Vector3(1.f, 1.f, 1.f));

	//������ body �ʱ�ȭ
	mMeshRenderer = AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mScript = AddComponent<PlayerStateScript>(eComponentType::Script);
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	mRigidBody = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);


	//ȿ����
	AudioSource* mAudioSource = AddComponent<AudioSource>(eComponentType::AudioSource);
	//AddComponent<PlayerBGMScript>(eComponentType::Script);
	mAudioSource->AddClipByKey(L"FootNote");
	mAudioSource->AddClipByKey(L"ha1");
	mAudioSource->AddClipByKey(L"ha2");
	mAudioSource->AddClipByKey(L"woo");
	mAudioSource->AddClipByKey(L"wahoo");
	mAudioSource->AddClipByKey(L"NewDonkCity");

	mAudioSource->AddClipByKey(L"CapThrow");
	mAudioSource->AddClipByKey(L"Capture");

	mAudioSource->Play(L"NewDonkCity", true);
	mAudioSource->SetVolume(L"ha2", 100.f);


	AddComponent<PhysXCollider>(eComponentType::Collider)->SetSwitchState(true);

	AddComponent<PhysicalMovement>(eComponentType::Movement);
	AddComponent<GenericAnimator>(eComponentType::GenericAnimator);

	BoneAnimator* animator = AddComponent<BoneAnimator>(eComponentType::BoneAnimator);

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"Mario");
	assert(model);
	mMeshRenderer->SetModel(model);
	mMeshRenderer->SetMaterialByKey(L"marioBodyMaterial", 0);


	physical->InitialDefaultProperties(eActorType::Kinematic, eGeometryType::Capsule, Vector3(0.5f, 0.25f, 0.5f));
	physical->CreateSubShape(Vector3(0.f, 0.f, 0.f), eGeometryType::Capsule, Vector3(0.5f, 0.25f, 0.5f), PxShapeFlag::eTRIGGER_SHAPE);

	mRigidBody->SetFriction(Vector3(40.f, 0.f, 40.f));

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

	int particleCount = 5;
	FootSmokeParticle* footsmokeparticle = new FootSmokeParticle(3, static_cast<ParticleFormat::eParticleType>(1));
	particle->AddParticle(footsmokeparticle, L"Default");
	ParticleFormat* pairtlceformat = particle->GetParticleFormat(L"Default");
	if (pairtlceformat)
	{
		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"CloudParticle");
		if (model)
			pairtlceformat->SetModel(model);

		Texture* tex = GETSINGLE(ResourceMgr)->Load<Texture>(L"SmokeParticle", L"SmokeParticle/Image/smoke01.png");
		pairtlceformat->SetTexture(static_cast<int>(eTextureSlot::Albedo), tex, 1, 1);
	}



	mStateInfo.resize(static_cast<int>(ePlayerState::Die) + 1);
	stateInfoInitalize();
	//mPlayerState = ePlayerState::Idle;

	//mariocap ����
	//mMarioCap = object::LateInstantiate<MarioCap>(eLayerType::Objects);
	//mMarioCap->Initialize();
	//mMarioCap->Physicalinit();
	DynamicObject::Initialize();


	mRigidBody->SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum::eLOCK_ANGULAR_Z, true);
	mRigidBody->SetRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum::eLOCK_ANGULAR_X, true);

	mMeshRenderer->SetBoneAnimator(nullptr);

	{
		MarioCap* mariocap = object::Instantiate<MarioCap>(eLayerType::Cap);
		SetMarioCap(mariocap);
	}
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

	if (KEY_TAP(R))
	{
		// ����Ʈ����
		mPlayerState = ePlayerState::Fall;
		mRigidBody->SetVelocity(Vector3::Zero);
		mRigidBody->ApplyGravity();
		mRigidBody->SetAirOn();
		mScript->SetHavingCap(true);
		Vector3 pos = GetTransform()->GetPhysicalPosition();
		pos.y += 10.f;
		GetTransform()->SetPhysicalPosition(pos);
	}
	if (KEY_TAP(T))
	{
		mScript->SetHavingCap(true);
	}
	if (KEY_TAP(F))
	{
		// �ϵ帮��
		mPlayerState = ePlayerState::Fall;
		mRigidBody->SetVelocity(Vector3::Zero);
		mRigidBody->ApplyGravity();
		mRigidBody->SetAirOn();
		mScript->SetHavingCap(true);
		GetTransform()->SetPhysicalPosition(Vector3(0.f, 10.f, 0.f));
	}
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
	// todo : ĸó ���ų� ���������϶� ��� �浹 ó���� �����ϸ� �ȵ�)
	if (L"Bind" != GetBoneAnimator()->PlayAnimationName())
	{
		if (eLayerType::Platforms == gameObject->GetLayerType())
		{
			if (mRigidBody->IsOnAir())
			{
				mRigidBody->SetAirOff();
				mRigidBody->RemoveGravity();
				SetPlayerState(Player::ePlayerState::Idle);
			}
		}

		if (eLayerType::Objects == gameObject->GetLayerType())
		{
			if (gameObject->GetObjectTypeName() == "BlockBrick")
			{
				Vector3 pentDir = GetPhysXCollider()->ComputePenetration_Direction(gameObject);
				Vector3 pentDirDepth = GetPhysXCollider()->ComputePenetration(gameObject);
				if (KEY_NONE(W) && KEY_NONE(S) && KEY_NONE(A) && KEY_NONE(D))
				{
					SetPlayerState(Player::ePlayerState::Idle);
				}

				if (!(pentDir == Vector3::Zero))
				{
					if (mRigidBody->IsOnAir())
					{
						mRigidBody->SetAirOff();
						mRigidBody->RemoveGravity();
						if (KEY_DOWN(W) || KEY_DOWN(S) || KEY_DOWN(A) || KEY_DOWN(D))
						{
							SetPlayerState(Player::ePlayerState::Move);
						}
					}

					//SetPlayerState(Player::ePlayerState::Idle);
					pentDirDepth.x = 0.f;
					pentDirDepth.z = 0.f;
					GetTransform()->SetPhysicalPosition(GetTransform()->GetPhysicalPosition() + pentDirDepth);

				}

				//if (Calculate_RelativeDirection_ByCosTheta(gameObject) < 0.95f && gameObject->GetObjectTypeName() == "BlockBrick")
				//{

				//	if (gameObject->GetScript<BlockBrickScript>() == nullptr)
				//		return;

					//gameObject->GetScript<BlockBrickScript>()->GetHit();


				//	gameObject->GetScript<BlockBrickScript>()->GetHit();
				//}
			}

			if (gameObject->GetObjectTypeName() == "PowerMoonObject")
			{
				gameObject->GetScript<MoonScript>()->GetPowerMoon();
			}
		}

		if (eLayerType::Monster == gameObject->GetLayerType())
		{
			if (Calculate_RelativeDirection_ByCosTheta(gameObject) < -0.95f)
			{
				mScript->ResetJumpCount();
				mRigidBody->SetAirOff();
				//mRigidBody->SetVelocity(AXIS::Y, 0.f);
				SetPlayerState(Player::ePlayerState::Jump);
				GetBoneAnimator()->ResetAnimator();
			}
		}
	}
	

	GameObj::OnTriggerEnter(gameObject);
}

void Player::OnTriggerPersist(GameObj* gameObject)
{
	if (eLayerType::Objects == gameObject->GetLayerType())
	{
		if (gameObject->GetObjectTypeName() == "BlockBrick")
		{
			Vector3 pentDir = GetPhysXCollider()->ComputePenetration_Direction(gameObject);
			Vector3 pentDirDepth = GetPhysXCollider()->ComputePenetration(gameObject);

			if (KEY_NONE(SPACE) && KEY_NONE(W) && KEY_NONE(S) && KEY_NONE(A) && KEY_NONE(D))
			{
				SetPlayerState(Player::ePlayerState::Idle);
			}
			if (!(pentDir == Vector3::Zero))
			{
				if (mRigidBody->IsOnAir())
				{
					mRigidBody->SetAirOff();
					mRigidBody->RemoveGravity();

					// ��ǲ �������� idle��
					if (KEY_DOWN(W) || KEY_DOWN(S) || KEY_DOWN(A) || KEY_DOWN(D))
					{
						SetPlayerState(Player::ePlayerState::Move);
					}
				}
				//SetPlayerState(Player::ePlayerState::Idle);

				pentDirDepth.x = 0.f;
				pentDirDepth.z = 0.f;
				GetTransform()->SetPhysicalPosition(GetTransform()->GetPhysicalPosition() + pentDirDepth);

			}
		}
	}
}

void Player::OnTriggerExit(GameObj* gameObject)
{
	if (eLayerType::Platforms== gameObject->GetLayerType())
	{
		if (!mRigidBody->IsOnAir())
		{
			mRigidBody->SetAirOn();
			mRigidBody->ApplyGravity();
			if (mPlayerState != Player::ePlayerState::Jump)
				SetPlayerState(Player::ePlayerState::Fall);
		}
	}
	if (eLayerType::Objects == gameObject->GetLayerType())
	{
		if (gameObject->GetObjectTypeName() == "BlockBrick")
		{
			if (Calculate_RelativeDirection_ByCosTheta(gameObject) < -0.5f)
			{
				if (!mRigidBody->IsOnAir())
				{
					mRigidBody->SetAirOn();
					mRigidBody->ApplyGravity();
					if (mPlayerState != Player::ePlayerState::Jump)
						SetPlayerState(Player::ePlayerState::Fall);
				}
			}
		}
	}
	GameObj::OnTriggerExit(gameObject);

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

	// ��ũ����
	stateEvent(eKeyState::TAP, eKeyCode::LCTRL, ePlayerState::Squat);

	// ����
	stateEvent(eKeyState::TAP, eKeyCode::SPACE, ePlayerState::Jump);

	// ���
 
	// �̵�
	if (!mRigidBody->IsOnAir())
	{
		stateEvent(eKeyState::DOWN, eKeyCode::W, ePlayerState::Move);
		stateEvent(eKeyState::DOWN, eKeyCode::S, ePlayerState::Move);
		stateEvent(eKeyState::DOWN, eKeyCode::A, ePlayerState::Move);
		stateEvent(eKeyState::DOWN, eKeyCode::D, ePlayerState::Move);
	}
	else
	{
		stateEvent(eKeyState::DOWN, eKeyCode::W, ePlayerState::Fall);
		stateEvent(eKeyState::DOWN, eKeyCode::S, ePlayerState::Fall);
		stateEvent(eKeyState::DOWN, eKeyCode::A, ePlayerState::Fall);
		stateEvent(eKeyState::DOWN, eKeyCode::D, ePlayerState::Fall);
	}

	// ���� ������
	able = false;
	stateEvent(eKeyState::TAP, eKeyCode::LBTN, ePlayerState::ThrowCap);

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
	InsertLockState(static_cast<UINT>(ePlayerState::Squat), static_cast<UINT>(ePlayerState::Groggy));
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
	//InsertLockState(static_cast<UINT>(ePlayerState::Fall), static_cast<UINT>(ePlayerState::Jump));
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

	//ThrowCap
	InsertLockState(static_cast<UINT>(ePlayerState::Capture), static_cast<UINT>(ePlayerState::Idle));
	InsertLockState(static_cast<UINT>(ePlayerState::Capture), static_cast<UINT>(ePlayerState::Move));
	//InsertLockState(static_cast<UINT>(ePlayerState::Capture), static_cast<UINT>(ePlayerState::Jump));
	InsertLockState(static_cast<UINT>(ePlayerState::Capture), static_cast<UINT>(ePlayerState::Squat));
	InsertLockState(static_cast<UINT>(ePlayerState::Capture), static_cast<UINT>(ePlayerState::SquatMove));
	InsertLockState(static_cast<UINT>(ePlayerState::Capture), static_cast<UINT>(ePlayerState::Air));
	InsertLockState(static_cast<UINT>(ePlayerState::Capture), static_cast<UINT>(ePlayerState::Fall));
	InsertLockState(static_cast<UINT>(ePlayerState::Capture), static_cast<UINT>(ePlayerState::Wall));
	InsertLockState(static_cast<UINT>(ePlayerState::Capture), static_cast<UINT>(ePlayerState::Hit));
	InsertLockState(static_cast<UINT>(ePlayerState::Capture), static_cast<UINT>(ePlayerState::Groggy));
	InsertLockState(static_cast<UINT>(ePlayerState::Capture), static_cast<UINT>(ePlayerState::ThrowCap));
	InsertLockState(static_cast<UINT>(ePlayerState::Capture), static_cast<UINT>(ePlayerState::CatchCap));
	//InsertLockState(static_cast<UINT>(ePlayerState::Capture), static_cast<UINT>(ePlayerState::Capture));
	InsertLockState(static_cast<UINT>(ePlayerState::Capture), static_cast<UINT>(ePlayerState::Die));



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

	if (mScript)
		mScript->Reset();
}

void Player::boneAnimatorInit(BoneAnimator* animator)
{
	AnimationClip* cilp = nullptr;

	//animator->LoadAnimations(L"..//Resources/MarioBody/Animation");
	animator->CreateAnimation(L"Wait", L"..//..//Resources/MarioBody/Animation/Wait.smd");
	animator->CreateAnimation(L"WaitHot", L"..//..//Resources/MarioBody/Animation/WaitHot.smd");
	animator->CreateAnimation(L"Bind", L"..//..//Resources/MarioBody/Animation/Bind.smd");

	animator->CreateAnimation(L"Walk", L"..//..//Resources/MarioBody/Animation/Walk.smd");
	animator->CreateAnimation(L"Brake", L"..//..//Resources/MarioBody/Animation/Brake.smd");
	animator->CreateAnimation(L"Fall", L"..//..//Resources/MarioBody/Animation/Fall.smd");
	animator->CreateAnimation(L"Land", L"..//..//Resources/MarioBody/Animation/Land.smd");

	animator->CreateAnimation(L"ThrowCap", L"..//..//Resources/MarioBody/Animation/ThrowCap.smd");
	animator->CreateAnimation(L"CatchCap", L"..//..//Resources/MarioBody/Animation/CatchCap.smd");

	animator->CreateAnimation(L"Jump", L"..//..//Resources/MarioBody/Animation/Jump.smd");
	animator->CreateAnimation(L"Jump2", L"..//..//Resources/MarioBody/Animation/Jump2.smd");
	animator->CreateAnimation(L"Jump3", L"..//..//Resources/MarioBody/Animation/Jump3.smd");

	animator->CreateAnimation(L"SquatStart", L"..//..//Resources/MarioBody/Animation/SquatStart.smd");
	animator->CreateAnimation(L"SquatEnd", L"..//..//Resources/MarioBody/Animation/SquatEnd.smd");
	animator->CreateAnimation(L"SquatWait", L"..//..//Resources/MarioBody/Animation/SquatWait.smd");
	animator->CreateAnimation(L"SquatWalk", L"..//..//Resources/MarioBody/Animation/SquatWalk.smd");

	animator->CreateAnimation(L"RollingStart", L"..//..//Resources/MarioBody/Animation/RollingStart.smd");
	animator->CreateAnimation(L"RollingEnd", L"..//..//Resources/MarioBody/Animation/RollingEnd.smd");
	animator->CreateAnimation(L"Rolling", L"..//..//Resources/MarioBody/Animation/Rolling.smd");
	animator->CreateAnimation(L"RollingStandUp", L"..//..//Resources/MarioBody/Animation/RollingStandUp.smd");

	animator->CreateAnimation(L"Dead", L"..//..//Resources/MarioBody/Animation/Dead.smd");
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

				mMarioCap->GetComponent<Transform>()->SetPhysicalPosition(position);
				mMarioCap->GetComponent<Transform>()->SetPhysicalRotation(rotation);

				mMarioCap->GetComponent<BoneAnimator>()->Play(L"ThrowCap",false);
				mMarioCap->SetCapState(MarioCap::eCapState::Throw);

				Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioHead");
				model->MeshRenderSwtich(L"Cap__CapMT-mesh", false);
			});

			cilp->SetCompleteEvent([this]() 
			{
				SetPlayerState(ePlayerState::Idle); 
			});
		}
	}

	// runstart �� run����
	{
		cilp = animator->GetAnimationClip(L"RunStart");
		if (cilp)
			cilp->SetCompleteEvent([animator, this]()
		{
			animator->Play(L"Run");
			ParticleSystem* particle = GetParticle();
			if (particle)
			{
				particle->Play(L"Default");
			}
		});
	}

	{
		cilp = animator->GetAnimationClip(L"Run");
		if (cilp)
			cilp->SetEndEvent([animator, this]()
				{
					ParticleSystem* particle = GetParticle();
					if (particle)
					{
						particle->Stop();
					}
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

	// SquatEnd > idle
	{
		cilp = animator->GetAnimationClip(L"SquatEnd");
		if (cilp)
			cilp->SetCompleteEvent([this]()
		{
			SetPlayerState(Player::ePlayerState::Idle);
		});
	}

	//CatchCap > idle
	{
		cilp = animator->GetAnimationClip(L"CatchCap");
		if (cilp)
			cilp->SetCompleteEvent([this]()
		{
			SetPlayerState(Player::ePlayerState::Idle);
			Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"MarioHead");
			model->MeshRenderSwtich(L"Cap__CapMT-mesh", true);
			mScript->SetHavingCap(true);
		});
	}

	// RollingStart > Rolling
	{
		cilp = animator->GetAnimationClip(L"RollingStart");
		if (cilp)
			cilp->SetCompleteEvent([animator]()
		{
			animator->Play(L"Rolling");
		});
	}

	// Bind End
	{
		cilp = animator->GetAnimationClip(L"Bind");
		if (cilp)
			cilp->SetEndEvent([this]()
		{
			int i = 0;
		});
	}
}

void Player::SetMarioCap(MarioCap* cap)
{
	mMarioCap = cap;
	cap->SetOwner(this);
}

void Player::CapturingProcess()
{
	GetPhysical()->KinematicActorSleep();
	mRigidBody->SetVelocity(Vector3::Zero);
	mRigidBody->RemoveGravity();

}

void Player::UnCapturingProcess()
{
	GetPhysical()->KinematicActorWakeup();
	//mRigidBody->SetVelocity(Vector3::Zero);
	mRigidBody->ApplyGravity();
}

