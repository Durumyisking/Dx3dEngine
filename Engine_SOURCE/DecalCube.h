#pragma once
#include "GameObj.h"

class Material;
class DecalCube : public GameObj
{
public:
	DecalCube();
	virtual ~DecalCube();

	virtual void Initialize() override;
	virtual void Render() override;
	
public:
	GETSET(Material*, mMateiral, Mateiral)

private:
	Material* mMateiral;
};

