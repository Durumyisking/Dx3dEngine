#include "guiEditorObject.h"


namespace gui
{
	EditorObject::EditorObject()
		: GameObj()
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
		GameObj::Update();
	}
	void EditorObject::fixedUpdate()
	{
		GameObj::FixedUpdate();
	}
	void EditorObject::render()
	{
		GameObj::Render();
	}
}
