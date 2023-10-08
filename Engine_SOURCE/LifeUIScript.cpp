#include "LifeUIScript.h"
#include "LifeUI.h"
#include "SpriteRenderer.h"
#include "TimeMgr.h"

LifeUIScript::LifeUIScript()
	:UIScript()
	, mPanal(nullptr)
	, mLife(3)
	, mShakeTime(0.0f)
	, mAmplitude(1.0f)
	, mFrequency(100.f)
	, mCount(0)
	, mOriginPos{}
	, mTargetPos(Vector3(0.0f,0.0f,10.0f))
	, mOriginColor(1.f, 1.f, 1.f, 1.f)
	, mState(State::None)
{

}

LifeUIScript::~LifeUIScript()
{

}

void LifeUIScript::Initialize()
{
	mPanal = dynamic_cast<Panal*>(this->GetOwner());
	mOriginPos = this->GetOwner()->GetComponent<Transform>()->GetPosition();
}

void LifeUIScript::Update()
{
	if (!isRunning)
		return;

	PlayerHit(mAmplitude, mFrequency);
}

void LifeUIScript::FixedUpdate()
{
	if (!isRunning)
		return;
}

void LifeUIScript::ChangeImage()
{
	//for (size_t i = 0; i < mPanal->GetChilds().size(); i++)
	//{
	//	LifeUI* object = (LifeUI*)mPanal->GetChilds()[i];
	//	object->Hit();
	//}

	if (mLife <= 0)
		return;

	const std::wstring& lifeKey = std::to_wstring(mLife);
	mPanal->GetChilds()[2]->ChangeTexture(lifeKey);

	const std::wstring& gaugeKey = L"Gauge_" + std::to_wstring(mLife);
	mPanal->GetChilds()[0]->ChangeTexture(gaugeKey);
}

void LifeUIScript::PlayerHit(float amplitude, float frequency)
{
	switch (mState)
	{
	case LifeUIScript::State::None:
		break;
	case LifeUIScript::State::Hit:
		ReactionHit();
		break;
	case LifeUIScript::State::Move:
		MoveUI(amplitude, frequency);
		break;
	case LifeUIScript::State::Wait:
		mShakeTime += DT;

		if (mShakeTime >= 1.0f)
		{
			mShakeTime = 0;
			mState = State::Back;
		}
		break;
	case LifeUIScript::State::Back:
		BackUI();
		break;
	default:
		break;
	}
}

void LifeUIScript::ReactionHit()
{
	mLife--;
	mState = State::Move;
}

void LifeUIScript::MoveUI(float amplitude, float frequency)
{
	this->GetOwner()->GetComponent<Transform>()->SetPosition(mTargetPos);

	mShakeTime += DT;

	float t = mShakeTime * frequency;
	float xOffset = amplitude * std::cos(t);

	Vector3 pos = this->GetOwner()->GetComponent<Transform>()->GetPosition();

	pos.x += xOffset;

	for (size_t i = 0; i < mPanal->GetChilds().size(); i++)
	{
		mPanal->GetChilds()[i]->SetColor(Vector4(1.f, 1.f, 1.f, 0.1f + std::cos(t)), true);
	}


	this->GetOwner()->GetComponent<Transform>()->SetPosition(pos);
	mCount++;

	if (mCount > 50)
	{
		for (size_t i = 0; i < mPanal->GetChilds().size(); i++)
		{
			mPanal->GetChilds()[i]->SetColor(mOriginColor, true);
		}
		mPanal->GetChilds()[2]->SetColor(Vector4(0.1f, 0.1f, 0.1f, 1.0f), true);
		ChangeImage();
		mCount = 0;
		mShakeTime = 0;
		mState = State::Wait;
	}
}

void LifeUIScript::BackUI()
{
	Transform* mTr = this->GetOwner()->GetComponent<Transform>();


	if (mOriginPos.x - mTr->GetPosition().x <= 0.01f)
	{
		mState = State::None;
	}

	Vector3 dir = mOriginPos - mTargetPos;

	Vector3 fabsDir = Vector3(dir.x, dir.y, 0.f);
	float value = sqrt(pow(fabsDir.x, 2.f) + pow(fabsDir.y, 2.f));
	Vector3 dirValue = Vector3(fabsDir.x / value, fabsDir.y / value, 0.f);

	Vector3 pos = mTr->GetPosition();

	pos += dirValue * DT * 1000;

	mTr->SetPosition(pos);
}
