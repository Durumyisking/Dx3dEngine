#include "Player.h"
#include "MarioParts.h"
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


Player::Player()
{
	SetLayerType(eLayerType::Player);

//		RigidBody* rigidbody = this->AddComponent<RigidBody>(eComponentType::RigidBody);


}

Player::~Player()
{

}

void Player::Initialize()
{
	//������ body �ʱ�ȭ
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	//AddComponent<PlayerScript>(eComponentType::Script);
	AddComponent<PlayerStateScript>(eComponentType::Script);
	AddComponent<Transform>(eComponentType::Transform);
	Physical* physical = AddComponent<Physical>(eComponentType::Physical);
	PhysXRigidBody* rigid = AddComponent<PhysXRigidBody>(eComponentType::RigidBody);
	AddComponent<PhysXCollider>(eComponentType::Collider);
	AddComponent<PhysicalMovement>(eComponentType::Movement);
	BoneAnimator* animator = AddComponent<BoneAnimator>(eComponentType::BoneAnimator);

	SetPos(Vector3(0.f, 0.f, 0.f));
	SetScale(Vector3(0.01f, 0.01f, 0.01f));
	SetName(L"Player");

	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(L"Mario");
	GetComponent<MeshRenderer>()->SetModel(model, model->GetMaterial(0));
	//animator->LoadAnimations(L"..//Resources/MarioBody/Animation");
	animator->CreateAnimation(L"Wait", L"..//..//Resources/MarioBody/Animation/Wait.smd", 0.05f);
	animator->CreateAnimation(L"WaitHot", L"..//..//Resources/MarioBody/Animation/WaitHot.smd", 0.05f);
	animator->CreateAnimation(L"Walk", L"..//..//Resources/MarioBody/Animation/Walk.smd", 0.05f);
	///animator->CreateAnimation(L"test2", L"..//..//Resources/MarioBody/Animation/Jump.smd", 0.05f);
	//animator->CreateAnimation(L"test3", L"..//..//Resources/MarioBody/Animation/Dead.smd", 0.05f);
	//animator->CreateAnimation(L"test4", L"..//..//Resources/MarioBody/Animation/Run.smd", 0.05f);
	animator->Play(L"Wait");

	GetComponent<MeshRenderer>()->SetMeshByKey(L"Cubemesh");

	physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Box, Vector3(1.0f, 1.0f, 1.0f));

	//������ ���� ����
	MarioParts* mHandL = new MarioParts();//object::Instantiate<MarioParts>(eLayerType::Player);
	MarioParts* mHandR = new MarioParts();//object::Instantiate<MarioParts>(eLayerType::Player);
	MarioParts* mHead = new MarioParts();//object::Instantiate<MarioParts>(eLayerType::Player);
	MarioParts* mFace = new MarioParts();//object::Instantiate<MarioParts>(eLayerType::Player);
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


	mStateInfo.resize(static_cast<int>(ePlayerState::Die) + 1);
	stateInfoInitalize();
	//mPlayerState = ePlayerState::Idle;

	DynamicObject::Initialize();
}

void Player::Update()
{

	DynamicObject::Update();

	for (auto i : mParts)
	{
		i->Update();
	}

}

void Player::FixedUpdate()
{
	DynamicObject::FixedUpdate();

	KeyCheck();

	for (auto i : mParts)
	{
		i->FixedUpdate();
	}


	for (auto i : mParts)
	{
		i->GetComponent<Transform>()->SetWorldMatrix(GetComponent<Transform>()->GetWorldMatrix());
	}


}

void Player::Render()
{
	DynamicObject::Render();

	for (auto i : mParts)
	{
		i->Render();
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
	PhysXRigidBody* rigid = GetComponent<PhysXRigidBody>();
	rigid->RemoveGravity();
	rigid->SetVelocity(AXIS::Y, 0.f);
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

	// ����
	able = false;
	stateEvent(eKeyState::TAP, eKeyCode::SPACE, ePlayerState::Jump);

	// Ư��

}

void Player::BoneInitialize()
{
}

void Player::PlayerAnimation(std::wstring name)
{
	//�ϰ������� �ִϸ��̼��� ��������ִ� �Լ�
	BoneAnimator* animator = GetComponent<BoneAnimator>();
	animator->Play(name);
	for (auto i : mParts)
	{
		i->GetComponent<BoneAnimator>()->Play(name);
	}
}

void Player::stateInfoInitalize()
{
	//Idle
	// ����� �����¿��� �����»��°� ����

	//Move
	
	// Jump
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Move));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::CrouchMove));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Groggy));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Die));

	//Crouch - ������ ��ũ���� ���¿����� �Ϲ� ������ �ȵȴ�. ������ ����ȭ ���� �����
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Move));
	//InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Jump));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Groggy));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Crouch));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Die));

	//CrouchMove - ��ũ���� �� �����̴� ����, �� ���¿����� �� �� �ִ� �׼ǵ��� �����Ѵ�.
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Move));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Wall));

	//Air - ���߿� ���ִ� ���¿����� �������� ���� ȸ���� �����ϸ�, �̵������� ���Ѵ�.
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Move));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Jump));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Crouch));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::CrouchMove));

	//Wall -���� ������ ¤���鼭 �������� ����, �� ���¿��� ������ �ϸ� �����Ⱑ �ȴ�. 
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Move));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::CrouchMove));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Air));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::ThrowCap));

	//Hit
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::Move));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::Jump));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::Crouch));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::CrouchMove));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Air));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Wall));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::ThrowCap));

	//Groggy
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Hit));

	//CapAction
	
	//Die
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::Idle));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::Move));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::Jump));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::Crouch));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::CrouchMove));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Air));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Wall));
	InsertLockState(static_cast<UINT>(ePlayerState::Jump), static_cast<UINT>(ePlayerState::Hit));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::Groggy));
	InsertLockState(static_cast<UINT>(ePlayerState::Die), static_cast<UINT>(ePlayerState::ThrowCap));
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
