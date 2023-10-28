#include "TitleBGMScript.h"
#include "TimeMgr.h"
#include "AudioSource.h"

TitleBGMScript::TitleBGMScript()
	:Script()
	,mPanal{}
	,mCurrentTime(0.f)
	, mState(State::None)
{

}

TitleBGMScript::~TitleBGMScript()
{

}

void TitleBGMScript::Initialize()
{
	mPanal = dynamic_cast<Panal*>(this->GetOwner());
}

void TitleBGMScript::Update()
{
	switch (mState)
	{
	case TitleBGMScript::State::None:
		break;
	case TitleBGMScript::State::MarioShout:
		MarioShout();
		break;
	case TitleBGMScript::State::StartBGM:
		StartBGM();
		break;
	default:
		break;
	}

}

void TitleBGMScript::MarioShout()
{
	mPanal->GetComponent<AudioSource>()->Play(L"SuperMarioOdyssey", false);
	mState = State::StartBGM;
}

void TitleBGMScript::StartBGM()
{
	mCurrentTime += DT;

	if (mCurrentTime > 2.f)
	{
		mPanal->GetComponent<AudioSource>()->Play(L"TitleBGM", true);
		mCurrentTime = 0;
		mState = State::None;
	}
}
