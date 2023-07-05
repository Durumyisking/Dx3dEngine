#include "CursorScript.h"
#include "Input.h"
#include "GameObj.h"

namespace dru
{
	CursorScript::CursorScript()
		: Script(),
		mTrans(nullptr),
		mPos{},
		mRot{}
	{
	}

	CursorScript::~CursorScript()
	{
	}

	void CursorScript::Initialize()
	{
		mTrans = GetOwner()->GetComponent<Transform>();
	}

	void CursorScript::update()
	{
	

	}

	void CursorScript::fixedUpdate()
	{
		mPos = Input::GetMousePosition();
		Vector3 newpos = mPos / 100.f;

		mTrans->SetPosition(newpos);
	}

	void CursorScript::render()
	{
	}

}