#include "TitleUIBarScript.h"
#include "ImageUI.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "UIManager.h"
#include "SceneMgr.h"

TitleUIBarScript::TitleUIBarScript()
	:UIScript()
	, mTitleTextPanal{}
	, mCurrentTime(0.0f)
	, mState(State::Idle)
	, mOriginScale{}
	, mChangeSize(Vector3(1.f, 0.9f, 1.0f))
	, mCount(0)
{

}

TitleUIBarScript::~TitleUIBarScript()
{

}

void TitleUIBarScript::Initialize()
{
	mOriginScale = this->GetOwner()->GetComponent<Transform>()->GetScale();
}

void TitleUIBarScript::Update()
{
	//Vector3 scale = this->GetOwner()->GetComponent<Transform>()->GetScale();
	//scale.x -= DT;
	//this->GetOwner()->GetComponent<Transform>()->SetScaleX(scale.x);


	switch (mState)
	{
	case TitleUIBarScript::State::Idle:
		Idle();
		break;
	case TitleUIBarScript::State::ScaleChange:
		ScaleChange();
		break;
	default:
		break;
	}
}

//	for (size_t i = 0; i < (GETSINGLE(UIManager)->GetPanal(eUIType::TitleText)->GetChilds().size()); i++)
//	{
//		(GETSINGLE(UIManager)->GetPanal(eUIType::TitleText)->GetChilds()[i]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f), false));
//	}

//	(GETSINGLE(UIManager)->GetPanal(eUIType::TitleText)->GetChilds()[GETSINGLE(UIManager)->mCount]->SetColor(Vector4(0.4f, 0.4f, 0.4, 1.0f), true));


void TitleUIBarScript::Idle()
{
	if ((GETSINGLE(InputMgr)->GetKeyTap(eKeyCode::UP) && mCount != 0))
	{
		Vector3 pos = this->GetOwner()->GetComponent<Transform>()->GetPosition() + Vector3(0.0f, 1.f, 0.0f);
		this->GetOwner()->GetComponent<Transform>()->SetPosition(pos);

		pos = mOriginScale * mChangeSize;  //Vector3(1.f, 0.9f, 1.0f)
		this->GetOwner()->GetComponent<Transform>()->SetScale(pos);

		mCount--;
		mState = State::ScaleChange;
	}
	else if ((GETSINGLE(InputMgr)->GetKeyTap(eKeyCode::DOWN)) && mCount != 3)
	{
		Vector3 pos = this->GetOwner()->GetComponent<Transform>()->GetPosition() + Vector3(0.0f, -1.f, 0.0f);
		this->GetOwner()->GetComponent<Transform>()->SetPosition(pos);

		pos = mOriginScale * mChangeSize;
		this->GetOwner()->GetComponent<Transform>()->SetScale(pos);

		mCount++;
		mState = State::ScaleChange;
	}
}

void TitleUIBarScript::ScaleChange()
{
	if(mTitleTextPanal == nullptr)
		mTitleTextPanal = dynamic_cast<Panal*>(GETSINGLE(UIManager)->GetTitlePanal(eUIType::TitleText));

	Vector3 scale = this->GetOwner()->GetComponent<Transform>()->GetScale() + Vector3(DT, DT, 0.0f);

	this->GetOwner()->GetComponent<Transform>()->SetScale(scale);

	if (scale.x > mOriginScale.x && scale.y > mOriginScale.y)
	{
		this->GetOwner()->GetComponent<Transform>()->SetScale(mOriginScale);
		mCurrentTime = 0;
		mState = State::Idle;
	}

	for (size_t i = 0; i < mTitleTextPanal->GetChilds().size(); i++)
	{
		mTitleTextPanal->GetChilds()[i]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f), false);
	}

	mTitleTextPanal->GetChilds()[mCount]->SetColor(Vector4(0.4f, 0.4f, 0.4f, 1.0f), true);
}

void TitleUIBarScript::Select()
{
	if (mTitleTextPanal == nullptr)
	{
		mTitleTextPanal = dynamic_cast<Panal*>(GETSINGLE(UIManager)->GetTitlePanal(eUIType::TitleText));
	}

	switch (mCount)
	{
	case 0:
		GETSINGLE(SceneMgr)->LoadScene(SceneMgr::eSceneType::Play);
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}
}
