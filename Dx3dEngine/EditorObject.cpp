#include "EditorObject.h"
namespace dru
{
	EditorObject::EditorObject()
		:GameObj()
	{
	}

	EditorObject::~EditorObject()
	{
	}

	void EditorObject::Initialize()
	{
		GameObj::Initialize();
	}

	void EditorObject::update()
	{
		GameObj::update();
	}

	void EditorObject::fixedUpdate()
	{
		GameObj::fixedUpdate();
	}

	void EditorObject::render()
	{
		GameObj::render();
	}

}