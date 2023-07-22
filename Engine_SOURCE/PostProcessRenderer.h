#pragma once
#include "BaseRenderer.h"



class PostProcess;
class PostProcessRenderer :
    public BaseRenderer
{
public:
	PostProcessRenderer();
	virtual ~PostProcessRenderer();

	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;
	virtual void Render() final;

	void SetPostProcessOwner(PostProcess* postProcess);

private:
	PostProcess* mPostProcess;
};
