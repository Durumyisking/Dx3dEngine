#pragma once
#include "Entity.h"
#include "Texture.h"

class MultiRenderTarget : public Entity
{
public:
	MultiRenderTarget();
	~MultiRenderTarget();

	void Create(Texture* texture[8], Texture* dsTexture);
	void OMSetRenderTarget();

private:
	Texture* mRenderTargets[8];
	Texture* mDSTexture;
	UINT mRenderTargetCount;
};

