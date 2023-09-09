#include "DynamicObject.h"

DynamicObject::DynamicObject()
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

void DynamicObject::OnCollisionEnter(GameObj* gameObject)
{
}

void DynamicObject::OnTriggerEnter(GameObj* gameObject)
{
}

void DynamicObject::OnTriggerExit(GameObj* gameObject)
{
}


