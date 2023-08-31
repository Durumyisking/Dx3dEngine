#include "Panal.h"
#include "MeshRenderer.h"
#include "SpriteRenderer.h"

Panal::Panal()
	:UIBase(eUIType::HP)
{

}

Panal::Panal(eUIType type)
	:UIBase(type)
{

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
	this->Die();
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