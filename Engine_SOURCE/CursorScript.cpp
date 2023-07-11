#include "CursorScript.h"
#include "InputMgr.h"
#include "GameObj.h"

namespace dru
{
	CursorScript::CursorScript()
		: Script(),
		mTransform(nullptr),
		mPos{},
		mRot{}
	{
	}

	CursorScript::~CursorScript()
	{
	}

	void CursorScript::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();
	}

	void CursorScript::update()
	{
	

	}

	void CursorScript::fixedUpdate()
	{
		mPos = GETSINGLE(InputMgr)->GetMousePosition();
		Vector3 newpos = mPos / 100.f;

		mTransform->SetPosition(newpos);
	}

	void CursorScript::render()
	{
	}

}