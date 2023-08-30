#pragma once
#include "Engine.h"
#include "GameObj.h"

class Texture;
class CubeMapHDR : public GameObj
{
#define SIZE_SKYBOX 2048;
#define SIZE_IRRADIANCE 32;
#define SIZE_PREFILTER 256;


public:
	CubeMapHDR();
	virtual ~CubeMapHDR();

	virtual void Initialize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	void SetTexture(Texture* texture) { mTexture = texture; }

	void Bind();
	//TextureHDR loadFromFile(std::string name, bool linearFilter = true, bool flipTex = false);



private:
	void createEnvMap();
	void bindIrradianceMap();
	void bindPrefilterMap();

private:
	int m_Width;
	int m_Height;

	ID3D11Device* g_d3dDevice;

	D3D11_VIEWPORT mViewport;
	SkyCB mProjConstantBuffer;
	Mesh* mCubemesh;

	Texture* mTexture;

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

	int asdf = 0;
};