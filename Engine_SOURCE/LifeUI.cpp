#include "LifeUI.h"
#include "SpriteRenderer.h"
#include "TimeMgr.h"



LifeUI::LifeUI()
	:UIBase(eUIType::None)
	, mCurrentTime(0.0f)
	, mShakeTime(0.0f)
	, mAmplitude(0.01f)
	, mFrequency(100.f)
	, mPositionX(0.0f)
	, mActivate(false)
	, mbShake(false)
	, mbBack(false)
	, mCount(0)
	, mOriginPos{}
	, mTargetPos{}
	, mOriginColor(1.f,1.f,1.f,1.f)
	, mState{}
{
}

LifeUI::LifeUI(const std::wstring& key, eUIType type)
	:UIBase(type)
	, mCurrentTime(0.0f)
	, mShakeTime(0.0f)
	, mAmplitude(0.01f)
	, mFrequency(100.f)
	, mPositionX(0.0f)
	, mActivate(false)
	, mbShake(false)
	, mbBack(false)
	, mCount(0)
	, mOriginPos{}
	, mTargetPos{}
	, mOriginColor(1.f, 1.f, 1.f, 1.f)
	, mState{}
{
	SpriteRenderer* render = this->AddComponent<SpriteRenderer>(eComponentType::UI);
	render->SetMeshByKey(L"Rectmesh");
	render->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(key));
}


LifeUI::~LifeUI()
{

}

void LifeUI::OnInit()
{
	mOriginPos = this->GetComponent<Transform>()->GetPosition();
}

void LifeUI::OnActive()
{
	mActivate = true;
	this->Active();
}

void LifeUI::OnInActive()
{
	mActivate = false;
	this->Die();
}

void LifeUI::OnUpdate()
{
	if (mbUIEnable == false)
		return;

	if (mActivate == false)
		return;

	PlayerHIt(mAmplitude, mFrequency);
}

void LifeUI::OnFixedUpdate()
{
}

void LifeUI::OnRender()
{
}

void LifeUI::OnFontRender()
{
}

void LifeUI::OnClear()
{
}

void LifeUI::SetColor(Vector4 color, bool isColor)
{
	SpriteRenderer* renderer = this->GetComponent<SpriteRenderer>();
	Material* material = renderer->GetMaterial();

	renderer::MaterialCB data = {};
	data.bool3 = isColor;
	data.xyzw1 = color;
	material->SetData(eGPUParam::Bool_3, &data.bool3);
	material->SetData(eGPUParam::Vector4_1, &data.xyzw1);

	mbColor = true;
}

void LifeUI::PlayerHIt(float amplitude, float frequency)
{	

	switch (mState)
	{
	case LifeUI::State::Hit:
		ReactionHit();
		break;
	case LifeUI::State::Move:
		MoveUI(amplitude, frequency);
		break;
	case LifeUI::State::Wait:
		mShakeTime += DT;
		
		if (mShakeTime >= 1.0f)
		{
			mShakeTime = 0;
			mState = State::Back;
		}
		break;
	case LifeUI::State::Back:
		BackUI();
		break;
	default:
		break;
	}
}

void LifeUI::ReactionHit()
{
	mState = State::Move;
}

void LifeUI::MoveUI(float amplitude, float frequency)
{
	if (mbShake)
	{
		this->GetComponent<Transform>()->SetPosition(mTargetPos);

		mShakeTime += DT;

		float t = mShakeTime * frequency;
		float xOffset = amplitude * std::cos(t);

		Vector3 pos = this->GetComponent<Transform>()->GetPosition();

		pos.x += xOffset;

		this->SetColor(Vector4(1.f, 1.f, 1.f,  0.1f + std::cos(t)), true);

		this->GetComponent<Transform>()->SetPosition(pos);
		mCount++;

		if (mCount > 50)
		{
			this->SetColor(mOriginColor, true);
			mCount = 0;
			mShakeTime = 0;
			mbShake = false;
			mbBack = true;
			mState = State::Wait;
		}
	}
}

void LifeUI::BackUI()
{
	if (mbBack)
	{
		Transform* mTr = this->GetComponent<Transform>();


		if (mOriginPos.x - mTr->GetPosition().x <= 0.01f)
		{
			mbBack = false;
		}

		Vector3 dir = mOriginPos - mTargetPos;

		Vector3 fabsDir = Vector3(dir.x, dir.y, 0);
		double value = sqrt(pow(fabsDir.x, 2) + pow(fabsDir.y, 2));
		Vector3 dirValue = Vector3(fabsDir.x / value, fabsDir.y / value, 0);

		Vector3 pos = mTr->GetPosition();

		pos += dirValue * DT * 12;

		mTr->SetPosition(pos);
	}
}
