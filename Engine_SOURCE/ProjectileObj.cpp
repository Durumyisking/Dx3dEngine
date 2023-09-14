#include "ProjectileObj.h"

ProjectileObj::ProjectileObj()
	: GameObj()
{
	Transform* tr = GetComponent<Transform>();
	tr->SetOffsetScale(0.01f);
}

ProjectileObj::~ProjectileObj()
{
}
