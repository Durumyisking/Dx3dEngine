#include "CompassUIScript.h"
#include "TimeMgr.h"
#include "Camera.h"
#include "ImageUI.h"


CompassUIScript::CompassUIScript()
	:UIScript()
	, mCameraTr(nullptr)
	, mTr(nullptr)
{

}

CompassUIScript::~CompassUIScript()
{

}

void CompassUIScript::Initialize()
{
	mCameraTr = renderer::mainCamera->GetOwner()->GetComponent<Transform>();
	mTr = this->GetOwner()->GetComponent<Transform>();
}

void CompassUIScript::Update()
{
	Vector3 rot = mCameraTr->GetRotation();

	mTr->SetRotation(Vector3(0.0f, 0.0f, -rot.y));
}
