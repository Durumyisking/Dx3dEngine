#include "MainMenuScript.h"
#include "InputMgr.h"
#include "SceneMgr.h"

MainMenuScript::MainMenuScript()
	:UIScript()
	,mPanal(nullptr)
	,mCount(0)
{

}

MainMenuScript::~MainMenuScript()
{

}

void MainMenuScript::Initialize()
{
	mPanal = dynamic_cast<Panal*>(this->GetOwner());
}

void MainMenuScript::Update()
{
	if ((GETSINGLE(InputMgr)->GetKeyTap(eKeyCode::UP)))
	{
		if (mCount >= 3)
			return;

		mCount++;

		for (size_t i = 0; i < mPanal->GetChilds().size(); i++)
		{
			mPanal->GetChilds()[i]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f), false);
		}

		mPanal->GetChilds()[mCount]->SetColor(Vector4(0.4f, 0.4f, 0.4f, 1.0f), true);
	}
	else if ((GETSINGLE(InputMgr)->GetKeyTap(eKeyCode::DOWN)))
	{
		if (mCount <= 0)
			return;

		mCount--;

		for (size_t i = 0; i <mPanal->GetChilds().size(); i++)
		{
			mPanal->GetChilds()[i]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f), false);
		}

		mPanal->GetChilds()[mCount]->SetColor(Vector4(0.4f, 0.4f, 0.4f, 1.0f), true);
	}

	if (KEY_TAP(N_1))
	{
		GETSINGLE(SceneMgr)->LoadScene(SceneMgr::eSceneType::Play);
		mCount = 3;
	}
}

void MainMenuScript::FixedUpdate()
{

}
