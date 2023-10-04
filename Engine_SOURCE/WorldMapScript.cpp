#include "WorldMapScript.h"
#include "ImageUI.h"
#include "TimeMgr.h"

WorldMapScript::WorldMapScript()
	:UIScript()
	,mCurrentTime(0.0f)
{

}

WorldMapScript::~WorldMapScript()
{

}

void WorldMapScript::Update()
{
	mCurrentTime += DT;

	this->GetOwner()->GetComponent<Transform>()->SetRotation(Vector3(0.0f, 0.0f, mCurrentTime * 1));
}