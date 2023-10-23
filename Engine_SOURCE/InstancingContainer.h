#pragma once
#include "GameObj.h"

class InstantiativeObject;
class BlockBrick;
class InstancingContainer :
    public GameObj
{
public:
    InstancingContainer();
    virtual ~InstancingContainer();

	virtual void Initialize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();
	virtual void PrevRender();

public:
	void PushObject(InstantiativeObject* obj);

	GETSET(eRenderingMode, mRenderingMode, RenderingMode);
	GETSET(std::vector<InstantiativeObject*>, mObjects, GameObjects);
	GETSET(std::vector<InstancingData>, mObjectsInstancingData, ObjectsInstancingData);

	void SetObjectInstancingData(UINT instanceID, const InstancingData& data)
	{
		mObjectsInstancingData[instanceID] = data;
	}
	void ResizeObjectInstancingData() { mObjectsInstancingData.resize(mObjects.size()); }

	UINT GetContainerSize() const { return static_cast<UINT>(mObjects.size()); }

private:
	std::vector<InstantiativeObject*> mObjects;
	std::vector<InstancingData> mObjectsInstancingData;

	eRenderingMode mRenderingMode;
};

