#include "DynamicObject.h"

DynamicObject::DynamicObject()
<<<<<<< HEAD
=======
	: GameObj()
>>>>>>> 255dec6e611b73f6e438073350714e0bf2481d2d
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

<<<<<<< HEAD
void DynamicObject::OnCollisionEnter(GameObj* gameObject)
{
}

void DynamicObject::OnTriggerEnter(GameObj* gameObject)
{
}

void DynamicObject::OnTriggerExit(GameObj* gameObject)
{
=======
void DynamicObject::InsertLockState(UINT curState, UINT lockState)
{
	if (curState >= mStateInfo.size())
		return;

	mStateInfo[curState].mLockState.insert(lockState);
>>>>>>> 255dec6e611b73f6e438073350714e0bf2481d2d
}


