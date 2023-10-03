#include "UIScript.h"

UIScript::UIScript()
	:isRunning(true)
{

}

UIScript::~UIScript()
{

}

void UIScript::Initialize()
{
	Script::Initialize();
}

void UIScript::Update()
{
	Script::Update();
}

void UIScript::FixedUpdate()
{
	Script::FixedUpdate();
}