#include "Panal.h"
#include "MeshRenderer.h"
#include "SpriteRenderer.h"

Panal::Panal()
	:UIBase(eUIType::HP)
{
	SpriteRenderer* spritreRender = AddComponent<SpriteRenderer>(eComponentType::UI);

}

Panal::Panal(eUIType type)
	:UIBase(type)
{
	SpriteRenderer* spritreRender = AddComponent<SpriteRenderer>(eComponentType::UI);

}

Panal::~Panal()
{

}

void Panal::OnInit()
{

}

void Panal::OnActive()
{

}

void Panal::OnInActive()
{

}

void Panal::OnTick()
{

}

void Panal::OnRender()
{

}

void Panal::OnClear()
{

}