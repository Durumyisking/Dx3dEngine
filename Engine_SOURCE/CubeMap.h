#pragma once
#include "Engine.h"
#include "GameObj.h"

class TextureHDR
{
public:
	TextureHDR();
	~TextureHDR();
	void Bind(unsigned int slot);
	void loadFromFile(std::string name, bool linearFilter = true, bool flipTex = false);
	/* Supports 1 or 4 channels */
	void loadFromData(void* data, int w, int h, int channels, bool linearFilter = true);
	int get_width() { return m_Width; };
	int get_height() { return m_Height; };
private:
	int m_Width;
	int m_Height;

	ID3D11Texture2D* pTexture;
	ID3D11SamplerState* pSampler;
	ID3D11ShaderResourceView* pTextureView;

};

class Mesh;
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

	void Bind();
	//TextureHDR loadFromFile(std::string name, bool linearFilter = true, bool flipTex = false);
	int get_width() { return m_Width; };
	int get_height() { return m_Height; };

private:
	void createEnvMap();
	void createEnvMapDepthStencilTexture();

	void bindCubeMap();
	void bindIrradianceMap();
	void bindPrefilterMap();

private:
	int m_Width;
	int m_Height;

	ID3D11Device* g_d3dDevice;

	D3D11_VIEWPORT mViewport;
	TextureHDR mhdrTexture;
	CubeMapCB mProjConstantBuffer;
	Mesh* mCubemesh;

	std::vector<ID3D11RenderTargetView*> mRTVs;
	std::vector<ID3D11RenderTargetView*> mRTVs2;
	std::vector<ID3D11RenderTargetView*> mRTVs3;

	Texture* mEnvMapTex;
	Texture* mIrradianceTex;
	Texture* mPreFilterTex;

	ID3D11Texture2D* mEnvMapTex;
	ID3D11RenderTargetView* mEnvMapRTV;
	ID3D11ShaderResourceView* mEnvMapSRV;

	ID3D11Texture2D* mIrradianceTex;
	ID3D11RenderTargetView* mIrradianceRTV;
	ID3D11ShaderResourceView* mIrradianceSRV;

	ID3D11Texture2D* mPreFilterTex;
	ID3D11RenderTargetView* mPreFilterRTV;
	ID3D11ShaderResourceView* mPreFilterSRV;


	ID3D11Texture2D* mEnvMapDepth;
	ID3D11DepthStencilView* mEnvMapDSV;

	ID3D11SamplerState* pSampler;

};