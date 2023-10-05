#include "DynamicObject.h"

DynamicObject::DynamicObject()
	: GameObj()
{
	mObjectTypeName = "DynamicObject";
}

DynamicObject::DynamicObject(const DynamicObject& Obj)
	: GameObj(Obj)
	, mStateInfo(Obj.mStateInfo)
{
}


DynamicObject::~DynamicObject()
{
}

DynamicObject* DynamicObject::Clone() const
{
	return new DynamicObject(*this);
}

void DynamicObject::Save(FILE* File)
{
	GameObj::Save(File);
}

void DynamicObject::Load(FILE* File)
{
	GameObj::Load(File);
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


