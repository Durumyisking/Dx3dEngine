#pragma once
#include "BaseRenderer.h"
#include "Mesh.h"
#include "Material.h"


class BoneAnimator;
class MeshRenderer : public BaseRenderer
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;
	virtual void PrevRender() final;
	virtual void Render() final;

	void SetBoneAnimator(BoneAnimator* aniamtor) { mBoneAnimator = aniamtor; }

	GETSET(bool, mbRenderShadow, RenderShadowFlag)

private:
	BoneAnimator* mBoneAnimator;
	bool mbRenderShadow;
};