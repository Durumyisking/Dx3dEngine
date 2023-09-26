#include "DynamicObject.h"

DynamicObject::DynamicObject()
	: GameObj()
{
}

DynamicObject::~DynamicObject()
{
}

void DynamicObject::Initialize()
{
	GameObj::Initialize();
}

void DynamicObject::Update()
{
	GameObj::Update();
}

void DynamicObject::FixedUpdate()
{
	GameObj::FixedUpdate();
}

void DynamicObject::Render()
{
	GameObj::Render();
}

void DynamicObject::FontRender()
{
	GameObj::FontRender();
}


void DynamicObject::InsertLockState(UINT curState, UINT lockState)
{
	if (curState >= mStateInfo.size())
		return;

	mStateInfo[curState].mLockState.insert(lockState);
}


