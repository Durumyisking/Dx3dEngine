#pragma once
#include "Entity.h"
#include "Texture.h"

using namespace math;
class MultiRenderTarget : public Entity
{
public:
	MultiRenderTarget();
	~MultiRenderTarget();

	void Create(Texture* texture[8], Texture* dsTexture);
	void OMSetRenderTarget();
	void Clear();

private:
	Texture* mRenderTargets[8];
	Vector4 mClearColors[8];
	Texture* mDSTexture;
	UINT mRenderTargetCount;
};

