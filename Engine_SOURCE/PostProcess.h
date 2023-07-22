#pragma once
#include "GameObj.h"
#include "Renderer.h"



class PostProcessRenderer;
class PostProcess :
	public GameObj
{
public:
	PostProcess();
	virtual ~PostProcess();

	virtual void Initialize();
	virtual void update();
	virtual void fixedUpdate();
	virtual void render();

	virtual void Bind();
	void Clear();

	void SetMaterial(std::wstring key);

protected:
	Material* mPostProcessMaterial;
	renderer::PostProcessCB mConstantBuffer;
	PostProcessRenderer* mRenderer;
};

