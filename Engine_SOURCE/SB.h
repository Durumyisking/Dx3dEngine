#pragma once
#include "GameObj.h"
#include "Renderer.h"


class SB : public GameObj
{
public:
	SB();
	virtual ~SB();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	//ID3D11Buffer* _pVertexBuffer = nullptr;
	//ID3D11Buffer* _pIndexBuffer = nullptr;
	//Cubemap* _pCubeMap = nullptr;
	//Cubemap* _pIrradianceMap = nullptr;
	//Cubemap* _pPreFilterMap = nullptr;
	//RenderTexture* _pBrdfLUT = nullptr;
	//SkyboxShader* _pSkyboxShader = nullptr;
	//FrameCBuffer* _pFrameBuffer = nullptr;
	//Camera* _pCamera = nullptr;
};

