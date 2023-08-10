#pragma once
#include "Entity.h"
#include "Texture.h"

using namespace math;
class MultiRenderTarget : public Entity
{
public:
	MultiRenderTarget();
	~MultiRenderTarget();

	void Create(std::vector<Texture*> textures, Texture* dsTexture);
	void Create(Texture* textures, Texture* dsTexture);
	void OMSetRenderTarget();
	void Clear(FLOAT backgroundColor[4]);

private:
	std::vector<Texture*> mRenderTargets;
	Vector4 mClearColors[8];
	Texture* mDSTexture;
	UINT mRenderTargetCount;
};

