#pragma once
#include "GameObj.h"

class BlockBrick;
class BlockBrickContainer :
    public GameObj
{
public:
    BlockBrickContainer();
    virtual ~BlockBrickContainer();

	virtual void Initialize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();
	virtual void PrevRender();

public:
	template <typename T>
	void PushBlock(T* obj)
	{
		mObjects.emplace_back(obj);
	}
	GETSET(eRenderingMode, mRenderingMode, RenderingMode);
	
private:
	std::vector<GameObj*> mObjects;

	eRenderingMode mRenderingMode;
};

