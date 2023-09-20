#include "CubeMap.h"
#include "Graphics.h"
#include "GraphicDevice.h"
#include "Texture.h"
#include "MeshRenderer.h"
#include "Material.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "../External/stb/stb_image.h"

//////////////////////////////////////////////////////////////////////////////////////////////////


CubeMapHDR::CubeMapHDR()
    : g_d3dDevice(GetDevice()->GetID3D11Device())
    , mViewport{}
    , mProjConstantBuffer{}
    , mCubemesh(nullptr)
    , mRTVs2{}
    , mRTVs3{}
    , mIrradianceTex(nullptr)
    , mIrradianceRTV(nullptr)
    , mIrradianceSRV(nullptr)
    , mPreFilterTex(nullptr)
    , mPreFilterRTV(nullptr)
    , mPreFilterSRV(nullptr)
    , pSampler(nullptr)
    , captureProjection{}
    , captureViews{}

{


}

CubeMapHDR::~CubeMapHDR()
{
    mIrradianceSRV->Release();
    mPreFilterSRV->Release();

    for (size_t i = 0; i < mRTVs2.size(); i++)
    {
        mRTVs2[i]->Release();
        mRTVs3[i]->Release();
    }
}

void CubeMapHDR::Initialize()
{
    createEnvMap();


    captureProjection = XMMatrixPerspectiveFovLH((XM_PI / 180.f) * 90.f, 1.f, 0.1f, 10.f);
    captureViews[0] = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(1, 0, 0, 0), XMVectorSet(0, -1, 0, 0)),
    captureViews[1] = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(-1, 0, 0, 0), XMVectorSet(0, -1, 0, 0)),
    captureViews[2] = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(0,-1, 0, 0), XMVectorSet(0,  0, 1, 0)),
    captureViews[3] = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(0, 1, 0, 0), XMVectorSet(0,  0,-1, 0)),
    captureViews[4] = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(0, 0,-1, 0), XMVectorSet(0, -1, 0, 0)),
    captureViews[5] = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(0, 0, 1, 0), XMVectorSet(0, -1, 0, 0));


    mCubemesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Cubemesh");

    mViewport.MinDepth = 0.0f;
    mViewport.MaxDepth = 1.0f;
    mViewport.TopLeftX = 0;
    mViewport.TopLeftY = 0;
    bindIrradianceMap();
    bindPrefilterMap();
    GetDevice()->AdjustToDefaultResolutionViewPorts();
    renderTargets[static_cast<UINT>(eRenderTargetType::Swapchain)]->OMSetRenderTarget();

    GetDevice()->AdjustToDefaultResolutionViewPorts();

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
    GameObj::Render();
    Bind();
}


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
    GetDevice()->CreateShaderResourceView(mIrradianceTex, &srvDesc, &mIrradianceSRV);
    GetDevice()->CreateShaderResourceView(mPreFilterTex, &srvDesc, &mPreFilterSRV);

    //for (uint32_t m = 0; m < 6; m++)
    //{
        for (uint32_t i = 0; i < 6; i++)
        {
            D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
            rtvDesc.Format = textureDesc.Format;
            rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
            rtvDesc.Texture2DArray.MipSlice = 0;
            rtvDesc.Texture2DArray.FirstArraySlice = i;
            rtvDesc.Texture2DArray.ArraySize = 1;

            ID3D11RenderTargetView* rtv2 = nullptr;
            GetDevice()->CreateRenderTargetView(mIrradianceTex, &rtvDesc, &rtv2);
            ID3D11RenderTargetView* rtv3 = nullptr;
            GetDevice()->CreateRenderTargetView(mPreFilterTex, &rtvDesc, &rtv3);

            mRTVs2.emplace_back(rtv2);
            mRTVs3.emplace_back(rtv3);
        }
    //}
    mIrradianceTex->Release();
    mPreFilterTex->Release();
}




void CubeMapHDR::bindIrradianceMap()
{
    Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"IrradianceShader");
    mViewport.Width = SIZE_IRRADIANCE;
    mViewport.Height = SIZE_IRRADIANCE;
    GetDevice()->BindViewports(&mViewport);

    shader->Bind();

    for (uint32_t i = 0; i < 6; ++i)
    {   
        float clearColor[4] = { 0.0f, 1.f, 0.f, 1.0f };
        GetDevice()->GetDeviceContext()->ClearRenderTargetView(mRTVs2[i], clearColor);
        XMMATRIX d = XMMatrixTranspose(captureViews[i] * captureProjection);
        mProjConstantBuffer.matrix = d;
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
    mViewport.Width = SIZE_PREFILTER;
    mViewport.Height = SIZE_PREFILTER;
    GetDevice()->BindViewports(&mViewport);

    shader->Bind();

    for (uint32_t i = 0; i < 6; ++i)
    {
        float clearColor[4] = { 1.0f, 0.f, 0.f, 1.0f };
        GetDevice()->GetDeviceContext()->ClearRenderTargetView(mRTVs3[i], clearColor);
        XMMATRIX d = XMMatrixTranspose(captureViews[i] * captureProjection);
        mProjConstantBuffer.matrix = d;
        constantBuffers[static_cast<UINT>(eCBType::CubeMapProj)]->SetData(&mProjConstantBuffer);
        constantBuffers[static_cast<UINT>(eCBType::CubeMapProj)]->Bind(eShaderStage::VS);
        GetDevice()->GetDeviceContext()->OMSetRenderTargets(1, &mRTVs3[i], nullptr);
        mCubemesh->BindBuffer();
        mCubemesh->Render();
    }
}



void CubeMapHDR::Bind()
{
    GetDevice()->BindShaderResource(eShaderStage::PS, static_cast<UINT>(eTextureSlot::IrradianceMap), &mIrradianceSRV);
    GetDevice()->BindShaderResource(eShaderStage::PS, static_cast<UINT>(eTextureSlot::PrefilteredMap),  &mPreFilterSRV);

} 