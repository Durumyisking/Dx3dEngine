#include "MonsterStateScript.h"
#include "Monster.h"

#include "SceneMgr.h"
#include "Scene.h"
#include "PhysXRigidBody.h"
#include "TimeMgr.h"
#include "Player.h"
MonsterStateScript::MonsterStateScript()
	: Script()
	, mStateEventList{}
	, mMonster(nullptr)
	, mPlayer(nullptr)
	, mbAnimationRunning(false)
	, mbTurnLeft(false)
{
	// 메모리 공간 확보
	mStateEventList.reserve(static_cast<UINT>(Monster::eMonsterState::Die) + 1);

	// 이벤트 바인딩 (this를 넣는 이유는 각 함수들의 인자에 this가 들어가기 때문임)
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Idle, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Move, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Jump, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Fall, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Land, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Turn, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Chase, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Attack, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::SpecialSituation, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Hit, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Groggy, this));
	mStateEventList.emplace_back(std::bind(&MonsterStateScript::Die, this));
}

MonsterStateScript::~MonsterStateScript()
{
	mStateEventList.clear();
}

void MonsterStateScript::Update()
{
	if (mMonster == nullptr)
		return;

	if (!mPlayer)
		mPlayer = GETSINGLE(SceneMgr)->GetActiveScene()->GetPlayer();

	UINT iState = static_cast<UINT>(mMonster->GetMonsterState());
	// enum 상태와 매칭되는 배열을 인덱스로 접근
	mStateEventList[iState]();
	
	Script::Update();
}

void MonsterStateScript::Initialize()
{
	// Owner 형 변환
	if (GetOwner())
		mMonster = dynamic_cast<Monster*>(GetOwner());

}

void MonsterStateScript::Idle()
{	
	if (mPlayer && !mMonster->IsCapture())
	{
		float dist = Vector3::Distance(GETSINGLE(SceneMgr)->GetActiveScene()->GetPlayer()->GetWorldPos(), GetOwnerWorldPos());
		if (mMonster->GetGetRecognizeRadius() > dist)
		{
			if (GetOwner()->IsObjectInFOV(mPlayer, 30.f))
			{
				Player::ePlayerState state = dynamic_cast<Player*>(mPlayer)->GetPlayerState();
				if (state != Player::ePlayerState::Capture)
				{
					mMonster->SetIsFoundPlayer(true);
					mMonster->SetMonsterState(Monster::eMonsterState::Turn);

					Vector3 dirToPlayer = mPlayer->GetWorldPos() - GetTransform()->WorldForward();
					dirToPlayer.Normalize();
					Vector3 CrossRst = dirToPlayer.Cross(GetTransform()->WorldForward());
					Vector3 worldUp = { 0.f, 1.f, 0.f };

					if (worldUp.Dot(CrossRst) < 0.f)
					{
						mbTurnLeft = true;
					}
				}
			}
		}
		else
		{
			mMonster->SetIsFoundPlayer(false);
			mMonster->SetMonsterState(Monster::eMonsterState::Idle);
		}
	}
}

void MonsterStateScript::MoveForward(float speed)
{
	Vector3 moveDir = -GetTransform()->WorldForward();
	moveDir.y = 0.f;
	GetPhysXRigidBody()->AddForce((moveDir * speed * DT));
}
