#include "CubeMap.h"
#include "Graphics.h"
#include "GraphicDevice.h"
#include "Texture.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "SkyboxRenderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../External/stb/stb_image.h"


TextureHDR::TextureHDR()
    :m_Width(0)
    , m_Height(0)
    , pTexture(nullptr)
    , pSampler(nullptr)
    , pTextureView(nullptr)
{
}

TextureHDR::~TextureHDR()
{
    SafeRelease(&pTexture);
    SafeRelease(&pTextureView);
    SafeRelease(&pSampler);
}

/* HDR Texture */
void TextureHDR::loadFromFile(std::string name, bool linearFilter, bool flipTex)
{
    int nrChannels;
    stbi_set_flip_vertically_on_load(flipTex);
    stbi_info(name.c_str(), &m_Width, &m_Height, &nrChannels);
    float* data = stbi_loadf(name.c_str(), &m_Width, &m_Height, &nrChannels, (nrChannels == 3) ? 4 : 0);
    loadFromData(data, m_Width, m_Height, (nrChannels == 3) ? 4 : nrChannels, linearFilter);
}

void TextureHDR::loadFromData(void* data, int w, int h, int channels, bool linearFilter)
{
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = w;
    desc.Height = h;
    desc.MipLevels = 1;         //0 = all mip levels
    desc.ArraySize = 1;
    desc.Format = (channels == 1) ? DXGI_FORMAT_R32_FLOAT : DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = data;
    sd.SysMemPitch = w * ((channels == 1) ? 1 : 4) * sizeof(float); // *4 because there are 4 values-> RGBA all 8bit
    sd.SysMemSlicePitch = w * h * ((channels == 1) ? 1 : 4) * sizeof(float);

    pTexture = {};
    HRESULT hr = GetDevice()->GetID3D11Device()->CreateTexture2D(&desc, &sd, &pTexture);
    assert(SUCCEEDED(hr));

    stbi_image_free(data);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;           //-1 = all the mipmap levels

    hr = GetDevice()->GetID3D11Device()->CreateShaderResourceView(pTexture, &srvDesc, &pTextureView);
    assert(SUCCEEDED(hr));

}


void TextureHDR::Bind(unsigned int slot)
{
     GetDevice()->GetDeviceContext()->PSSetShaderResources(slot, 1, &pTextureView);
}


//////////////////////////////////////////////////////////////////////////////////////////////////


CubeMapHDR::CubeMapHDR()
    : g_d3dDevice(nullptr)
    , mEnvMapTex(nullptr)
    , mEnvMapRTV(nullptr)
    , mEnvMapSRV(nullptr)

{
    g_d3dDevice = GetDevice()->GetID3D11Device();

    AddComponent<SkyboxRenderer>(eComponentType::MeshRenderer);
}

CubeMapHDR::~CubeMapHDR()
{
}

void CubeMapHDR::Initialize()
{
    mhdrTexture.loadFromFile("C:/Users/csh/Desktop/Dx3dEngine-sehyun/Dx3dEngine/Resources/environment.hdr");
    //    mhdrTexture.loadFromFile("E:/Dev/3d-my/Dx3dEngine/Resources/environment.hdr");
    createEnvMap();
    //createEnvMapDepthStencilTexture();

    XMMATRIX captureProjection = XMMatrixPerspectiveFovLH((XM_PI / 180) * 90, 1, 0.1, 10);
    XMMATRIX captureViews[] =
    {
        XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(1, 0, 0, 0), XMVectorSet(0, -1, 0, 0)),
        XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(-1, 0, 0, 0), XMVectorSet(0, -1, 0, 0)),
        XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(0,-1, 0, 0), XMVectorSet(0,  0, 1, 0)),
        XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(0, 1, 0, 0), XMVectorSet(0,  0,-1, 0)),
        XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(0, 0,-1, 0), XMVectorSet(0, -1, 0, 0)),
        XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(0, 0, 1, 0), XMVectorSet(0, -1, 0, 0))
    };
    for (uint32_t i = 0; i < 6; ++i)
    {
        XMMATRIX d = XMMatrixTranspose(captureViews[i] * captureProjection);
        mProjConstantBuffer.matrix = d;
    }

    mCubemesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Cubemesh");

    bindCubeMap();
    bindIrradianceMap();
    bindPrefilterMap();


 //   GetDevice()->AdjustViewPorts();

    GameObj::Initialize();
}

void CubeMapHDR::Update()
{
    GameObj::Update();
}

void CubeMapHDR::FixedUpdate()
{
    GameObj::FixedUpdate();
}

void CubeMapHDR::Render()
{
    Bind();
    GameObj::Render();
}

//TextureHDR CubeMapHDR::loadFromFile(std::string name, bool linearFilter, bool flipTex)
//{
//    return loadFromFile(name, true, flipTex);
//}

void CubeMapHDR::createEnvMap()
{
    // Create the TextureCube for enviroment mapping rendertarget
    D3D11_TEXTURE2D_DESC textureDesc = {};


    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 6;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    //textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    textureDesc.Width = SIZE_SKYBOX;
    textureDesc.Height = SIZE_SKYBOX;
    g_d3dDevice->CreateTexture2D(&textureDesc, nullptr, &mEnvMapTex);

    textureDesc.Width = SIZE_IRRADIANCE;
    textureDesc.Height = SIZE_IRRADIANCE;
    g_d3dDevice->CreateTexture2D(&textureDesc, nullptr, &mIrradianceTex);

    textureDesc.Width = SIZE_PREFILTER;
    textureDesc.Height = SIZE_PREFILTER;
    g_d3dDevice->CreateTexture2D(&textureDesc, nullptr, &mPreFilterTex);

    // Create SRV to access to envMap in Shader
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.TextureCube.MostDetailedMip = 0;
    srvDesc.TextureCube.MipLevels = 1;
    GetDevice()->CreateShaderResourceView(mEnvMapTex, &srvDesc, &mEnvMapSRV);
    GetDevice()->CreateShaderResourceView(mIrradianceTex, &srvDesc, &mIrradianceSRV);
    GetDevice()->CreateShaderResourceView(mPreFilterTex, &srvDesc, &mPreFilterSRV);

    for (uint32_t i = 0; i < 6; i++)
    {
        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = textureDesc.Format;
        rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
        rtvDesc.Texture2DArray.MipSlice = 0;
        rtvDesc.Texture2DArray.FirstArraySlice = i;
        rtvDesc.Texture2DArray.ArraySize = 1;

        ID3D11RenderTargetView* rtv = nullptr;
        GetDevice()->CreateRenderTargetView(mEnvMapTex, &rtvDesc, &rtv); // envmaptex텍스처를 rtv로 사용할거야
        ID3D11RenderTargetView* rtv2 = nullptr;
        GetDevice()->CreateRenderTargetView(mIrradianceTex, &rtvDesc, &rtv2);
        ID3D11RenderTargetView* rtv3 = nullptr;
        GetDevice()->CreateRenderTargetView(mPreFilterTex, &rtvDesc, &rtv3);

        mRTVs.emplace_back(rtv);
        mRTVs2.emplace_back(rtv2);
        mRTVs3.emplace_back(rtv3);
    }
    mEnvMapTex->Release();
    mIrradianceTex->Release();
    mPreFilterTex->Release();
}

void CubeMapHDR::createEnvMapDepthStencilTexture()
{
    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = SIZE_SKYBOX;
    textureDesc.Height = SIZE_SKYBOX;
    textureDesc.Format = DXGI_FORMAT_R32_FLOAT;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 6;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    g_d3dDevice->CreateTexture2D(&textureDesc, nullptr, &mEnvMapDepth);

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = textureDesc.Format;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
    dsvDesc.Texture2DArray.FirstArraySlice = 0;
    dsvDesc.Texture2DArray.ArraySize = 6;
    dsvDesc.Texture2DArray.MipSlice = 0;
    GetDevice()->CreateDepthStencilView(mEnvMapDepth, &dsvDesc, &mEnvMapDSV);
}

void CubeMapHDR::bindCubeMap()
{
    Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"RectToCubemapShader");
  /*  mViewport.Width = SIZE_SKYBOX;
    mViewport.Height = SIZE_SKYBOX;
    mViewport.MinDepth = 0.0f;
    mViewport.MaxDepth = 1.0f;
    mViewport.TopLeftX = 0;
    mViewport.TopLeftY = 0;
    GetDevice()->BindViewports(&mViewport);*/

    mhdrTexture.Bind(static_cast<UINT>(eTextureSlot::Skybox));
    shader->Bind();

    for (uint32_t i = 0; i < 6; ++i)
    {
        float clearColor[4] = { 0.f, 0.f, 0.f, 1.f };
        switch (i)
        {
        case 0:
            clearColor[0] = 1.f;
            break;
        case 1:
            clearColor[1] = 1.f;
            break;
        case 2:
            clearColor[2] = 1.f;
            break;
        case 3:
            clearColor[0] = 1.f;
            clearColor[1] = 1.f;
            break;
        case 4:
            clearColor[0] = 1.f;
            clearColor[2] = 1.f;
            break;
        case 5:
            clearColor[1] = 1.f;
            clearColor[2] = 1.f;
            break;
        default:
            break;
        }

        GetDevice()->GetDeviceContext()->ClearRenderTargetView(mRTVs[i], clearColor);
        constantBuffers[static_cast<UINT>(eCBType::CubeMapProj)]->SetData(&mProjConstantBuffer);
        constantBuffers[static_cast<UINT>(eCBType::CubeMapProj)]->Bind(eShaderStage::VS);
        GetDevice()->GetDeviceContext()->OMSetRenderTargets(1, &mRTVs[i], nullptr);
        mCubemesh->BindBuffer();
        mCubemesh->Render();
    }
}

void CubeMapHDR::bindIrradianceMap()
{
    Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"IrradianceShader");
  /*  mViewport.Width = SIZE_IRRADIANCE;
    mViewport.Height = SIZE_IRRADIANCE;
    GetDevice()->BindViewports(&mViewport);*/

    mhdrTexture.Bind(static_cast<UINT>(eTextureSlot::Skybox));
    shader->Bind();

    for (uint32_t i = 0; i < 6; ++i)
    {
        float clearColor[4] = { 0.0f, 1.f, 0.f, 1.0f };
        GetDevice()->GetDeviceContext()->ClearRenderTargetView(mRTVs2[i], clearColor);
        constantBuffers[static_cast<UINT>(eCBType::CubeMapProj)]->SetData(&mProjConstantBuffer);
        constantBuffers[static_cast<UINT>(eCBType::CubeMapProj)]->Bind(eShaderStage::VS);
        GetDevice()->GetDeviceContext()->OMSetRenderTargets(1, &mRTVs2[i], nullptr);
        mCubemesh->BindBuffer();
        mCubemesh->Render();
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

void CubeMapHDR::bindPrefilterMap()
{
    Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PreFilterShader");
   /* mViewport.Width = SIZE_PREFILTER;
    mViewport.Height = SIZE_PREFILTER;
    GetDevice()->BindViewports(&mViewport);*/

    mhdrTexture.Bind(static_cast<UINT>(eTextureSlot::Skybox));
    shader->Bind();

    for (uint32_t i = 0; i < 6; ++i)
    {
        float clearColor[4] = { 0.0f, 0.f, 1.f, 1.0f };
        GetDevice()->GetDeviceContext()->ClearRenderTargetView(mRTVs3[i], clearColor);
        constantBuffers[static_cast<UINT>(eCBType::CubeMapProj)]->SetData(&mProjConstantBuffer);
        constantBuffers[static_cast<UINT>(eCBType::CubeMapProj)]->Bind(eShaderStage::VS);
        GetDevice()->GetDeviceContext()->OMSetRenderTargets(1, &mRTVs3[i], nullptr);
        mCubemesh->BindBuffer();
        mCubemesh->Render();
    }
}



void CubeMapHDR::Bind()
{
    GetDevice()->BindShaderResource(eShaderStage::PS, static_cast<UINT>(eTextureSlot::CubeMap), &mEnvMapSRV);
    GetDevice()->BindShaderResource(eShaderStage::PS, static_cast<UINT>(eTextureSlot::IrradianceMap), &mIrradianceSRV);
    GetDevice()->BindShaderResource(eShaderStage::PS, static_cast<UINT>(eTextureSlot::PrefilteredMap),  &mPreFilterSRV);
}