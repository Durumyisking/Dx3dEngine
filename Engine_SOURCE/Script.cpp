#include "Script.h"
#include "Transform.h"
#include "GameObj.h"
#include "Animator.h"
#include "Object.h"
#include "Input.h"
namespace dru
{
	CScript::CScript()
		: CComponent(eComponentType::Script)
	{
	}

	CScript::~CScript()
	{
	}

	void CScript::Initialize()
	{
	}

	void CScript::update()
	{
	}

	void CScript::fixedUpdate()
	{
	}

	void CScript::render()
	{

	}
	void CScript::fontRender()
	{
	}
	void CScript::OnCollisionEnter(CCollider2D* _oppo)
	{


	}
	void CScript::OnCollision(CCollider2D* _oppo)
	{
	}

}