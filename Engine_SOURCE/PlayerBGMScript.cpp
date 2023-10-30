#include "PlayerBGMScript.h"
#include "TimeMgr.h"
#include "AudioSource.h"
#include "Player.h"

PlayerBGMScript::PlayerBGMScript()
	:Script()
	, mPlayer{}
	, mCurrentTime(0.f)
	, mState(eSoundState::None)
{

}

PlayerBGMScript::~PlayerBGMScript()
{

}

void PlayerBGMScript::Initialize()
{
	mPlayer = dynamic_cast<Player*>(this->GetOwner());
}

void PlayerBGMScript::Update()
{
	switch (mState)
	{
	case PlayerBGMScript::eSoundState::None:
		break;
	case PlayerBGMScript::eSoundState::FootNote:
		FootNote();
		break;
	default:
		break;
	}

}

void PlayerBGMScript::FootNote()
{
	mCurrentTime += DT;

	if (mCurrentTime > 0.2f)
	{
		mPlayer->GetComponent<AudioSource>()->Play(L"FootNote", false);
		mCurrentTime = 0;
	}

}

void PlayerBGMScript::StartBGM()
{
	mCurrentTime += DT;

	if (mCurrentTime > 2.f)
	{
		//mPlayer->GetComponent<AudioSource>()->Play(L"PlayerBGM", true);
		mCurrentTime = 0;
		mState = eSoundState::None;
	}
}
