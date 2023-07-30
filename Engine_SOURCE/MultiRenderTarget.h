#pragma once
#include "Entity.h"
#include "Texture.h"

using namespace math;
class MultiRenderTarget : public Entity
{
public:
	MultiRenderTarget();
	~MultiRenderTarget();

	void Create(Texture* texture[12], Texture* dsTexture);
	void OMSetRenderTarget();
	void Clear(FLOAT backgroundColor[4]);

private:
	Texture* mRenderTargets[12];
	Vector4 mClearColors[8];
	Texture* mDSTexture;
	UINT mRenderTargetCount;
};

