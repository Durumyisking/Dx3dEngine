#include "Script.h"
#include "Transform.h"
#include "GameObj.h"
#include "Animator.h"
#include "Object.h"
#include "Input.h"
namespace dru
{
	Script::Script()
		: Component(eComponentType::Script)
	{
	}

	Script::~Script()
	{
	}

	void Script::Initialize()
	{
	}

	void Script::update()
	{
	}

	void Script::fixedUpdate()
	{
	}

	void Script::render()
	{

	}
	void Script::fontRender()
	{
	}
	void Script::OnCollisionEnter(Collider2D* _oppo)
	{


	}
	void Script::OnCollision(Collider2D* _oppo)
	{
	}

}