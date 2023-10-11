#include "CoinUIScript.h"
#include "Texture.h"
#include "UIManager.h"
#include "Material.h"
#include "TimeMgr.h"

CoinUIScript::CoinUIScript()
	: UIScript()
	, mState(State::None)
	, mPanal(nullptr)
	, mbCoin(false)
	, mbDie(false)
	, mbLoseCion(false)
	, mCurrentTime(0.0f)
	, mCoin(0)
	, mCount(0)
	, mLoseCoinCnt(10)
{

}

CoinUIScript::~CoinUIScript()
{

}

void CoinUIScript::Initialize()
{
	mPanal = dynamic_cast<Panal*>(this->GetOwner());
}

void CoinUIScript::Update()
{
	if (!isRunning)
		return;

	switch (mState)
	{
	case CoinUIScript::State::None:
		break;
	case CoinUIScript::State::Idle:
		break;
	case CoinUIScript::State::GetCoin:
		ReturnToColor();
		break;
	case CoinUIScript::State::Die:
		ReturnToFade();
		break;
	case CoinUIScript::State::LoseCoin:
		CountLoseCoin();
		break;
	case CoinUIScript::State::End:
		End();
		break;
	default:
		break;
	}
}

void CoinUIScript::FixedUpdate()
{
	if (!isRunning)
		return;


}

void CoinUIScript::GetCoin()
{
	mCoin++;
	mCurrentTime = 0;

	int ones = mCoin % 10;
	int ten = (mCoin / 10) % 10;
	int hundreds = mCoin / 100;

	for (size_t i = 0; i < 4; i++)
	{
		mPanal->GetChilds()[i]->SetColor(Vector4(1.f, 0.92f, 0.016f, 1.f), true);

	}

	Texture* tex = (GETSINGLE(ResourceMgr)->Find<Texture>(std::to_wstring(ones)));
	mPanal->GetChilds()[3]->GetMaterial()->SetTexture(tex);

	tex = (GETSINGLE(ResourceMgr)->Find<Texture>(std::to_wstring(ten)));
	mPanal->GetChilds()[2]->GetMaterial()->SetTexture(tex);

	tex = (GETSINGLE(ResourceMgr)->Find<Texture>(std::to_wstring(hundreds)));
	mPanal->GetChilds()[1]->GetMaterial()->SetTexture(tex);
	
	mbCoin = true;
	mState = State::GetCoin;
}

void CoinUIScript::LoseCoin()
{
	mCoin--;
	mCurrentTime = 0;

	int ones = mCoin % 10;
	int ten = (mCoin / 10) % 10;
	int hundreds = mCoin / 100;

	for (size_t i = 0; i < 4; i++)
	{
		mPanal->GetChilds()[i]->SetColor(Vector4(1.f, 0.92f, 0.016f, 1.f), true);

	}

	Texture* tex = (GETSINGLE(ResourceMgr)->Find<Texture>(std::to_wstring(ones)));
	mPanal->GetChilds()[3]->GetMaterial()->SetTexture(tex);

	tex = (GETSINGLE(ResourceMgr)->Find<Texture>(std::to_wstring(ten)));
	mPanal->GetChilds()[2]->GetMaterial()->SetTexture(tex);

	tex = (GETSINGLE(ResourceMgr)->Find<Texture>(std::to_wstring(hundreds)));
	mPanal->GetChilds()[1]->GetMaterial()->SetTexture(tex);
}

void CoinUIScript::ActionToPlayerDied()
{
	ChangeColor();
	ChangePos();

	mState = State::Die;
}

void CoinUIScript::ReturnToColor()
{
	mCurrentTime += DT;


	if (mCurrentTime >= 1.0f)
	{
		for (size_t i = 0; i < 4; i++)
		{
			mPanal->GetChilds()[i]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f), true);
		}
		mCurrentTime = 0;
		mState = State::Idle;
	}
}

void CoinUIScript::ReturnToFade()
{
	mCurrentTime += DT;

	if (mCurrentTime > 1.0f)
	{
		mState = State::LoseCoin;
		return;
	}

	for (size_t i = 0; i < mPanal->GetChilds().size(); i++)
	{
		mPanal->GetChilds()[i]->SetColor(Vector4(1.0f, 1.0f, 1.0f, mCurrentTime), true);
	}
}

void CoinUIScript::CountLoseCoin()
{
	mCurrentTime += DT;

	if (mCurrentTime >= 0.1f)
	{
		LoseCoin();
		mCount++;
		mCurrentTime = 0;

		if (mCount >= mLoseCoinCnt)
		{
			mCount = 0;
			mCurrentTime = 0;
			mState = State::End;
		}
	}
}

void CoinUIScript::ChangeColor()
{
	for (size_t i = 0; i < mPanal->GetChilds().size(); i++)
	{
		mPanal->GetChilds()[i]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f), true);
	}
}

void CoinUIScript::ChangePos()
{
	mPanal->GetChilds()[0]->SetPos(Vector3(-1.0f, 0.f, 0.f));
	mPanal->GetChilds()[0]->SetScale(Vector3(0.6f, 0.6f, 1.f));

	mPanal->GetChilds()[1]->SetPos(Vector3(-0.35f, -0.1f, 0.f));
	mPanal->GetChilds()[2]->SetPos(Vector3(0.05f, -0.1f, 0.f));
	mPanal->GetChilds()[3]->SetPos(Vector3(0.45f, -0.1f, 0.f));

	mPanal->GetChilds()[4]->SetPos(Vector3(0.f, -0.5f, 0.f));
	mPanal->GetChilds()[4]->SetScale(Vector3(2.6f, 1.4f, 1.f));
	mPanal->GetChilds()[4]->SetRotation(Vector3(0.0f, 0.0f, -2.f));
}

void CoinUIScript::End()
{
	mCurrentTime += DT;

	if (mCurrentTime >= 1.0f)
	{
		for (size_t i = 0; i < 4; i++)
		{
			mPanal->GetChilds()[i]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f), true);
		}
		mCurrentTime = 0;

		Panal* panal = dynamic_cast<Panal*>((GETSINGLE(UIManager)->GetPanal(eUIType::DiePanal)));

		panal->Activate();

		panal = dynamic_cast<Panal*>((GETSINGLE(UIManager)->GetPanal(eUIType::Coin)));

		panal->InActivate();

		mState = State::None;
		Reset();
	}
}

void CoinUIScript::Reset()
{
	mPanal->GetChilds()[0]->SetPos(Vector3(-7.f, 3.5f, 0.f));
	mPanal->GetChilds()[0]->SetScale(Vector3(1.f, 1.f, 1.f));

	mPanal->GetChilds()[1]->SetPos(Vector3(-6.35f, 3.3f, 0.f));
	mPanal->GetChilds()[2]->SetPos(Vector3(-6.01f, 3.3f, 0.f));
	mPanal->GetChilds()[3]->SetPos(Vector3(-5.67f, 3.3f, 0.f));

	mPanal->GetChilds()[4]->SetPos(Vector3(-5.4f, 2.9f, 0.f));
	mPanal->GetChilds()[4]->SetScale(Vector3(4.2f, 1.4f, 1.f));
	mPanal->GetChilds()[4]->SetRotation(Vector3(0.f, 0.f, 0.f));
}