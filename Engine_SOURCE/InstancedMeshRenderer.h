#pragma once
#include "BaseRenderer.h"
#include "Mesh.h"
#include "Material.h"


class BoneAnimator;
class InstancedMeshRenderer : public BaseRenderer
{
public:
	InstancedMeshRenderer();
	virtual ~InstancedMeshRenderer();

	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;
	virtual void PrevRender() final;
	virtual void Render() final;

	void SetBoneAnimator(BoneAnimator* aniamtor) { mBoneAnimator = aniamtor; }

	GETSET(UINT, mInstanceCount, InstanceCount)

private:
	BoneAnimator* mBoneAnimator;
	UINT mInstanceCount;
};