#pragma once
#include "Engine.h"
#include "GameObj.h"

class Texture;
class CubeMapHDR : public GameObj
{
#define SIZE_IRRADIANCE 32;
#define SIZE_PREFILTER 128;


public:
	CubeMapHDR();
	virtual ~CubeMapHDR();

	virtual void Initialize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	void Bind();
	//TextureHDR loadFromFile(std::string name, bool linearFilter = true, bool flipTex = false);



private:
	void createEnvMap();
	void bindIrradianceMap();
	void bindPrefilterMap();

private:
	ID3D11Device* g_d3dDevice;

	D3D11_VIEWPORT mViewport;
	SkyCB mProjConstantBuffer;
	Mesh* mCubemesh;
	
	std::vector<ID3D11RenderTargetView*> mRTVs2;
	std::vector<ID3D11RenderTargetView*> mRTVs3;

	ID3D11Texture2D* mIrradianceTex;
	ID3D11RenderTargetView* mIrradianceRTV;
	ID3D11ShaderResourceView* mIrradianceSRV;
	ID3D11Texture2D* mPreFilterTex;
	ID3D11RenderTargetView* mPreFilterRTV;
	ID3D11ShaderResourceView* mPreFilterSRV;

	ID3D11SamplerState* pSampler;

	XMMATRIX captureProjection;
	XMMATRIX captureViews[6];

};