#include "CubeMap.h"
#include "Graphics.h"
#include "GraphicDevice.h"
#include "Texture.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "SB.h"

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
{
}

CubeMapHDR::~CubeMapHDR()
{
    SafeRelease(&pCubemap);
    SafeRelease(&pCubemapView);
    SafeRelease(&pFilteredMapView);
    SafeRelease(&pSampler);
}

void CubeMapHDR::loadFromFile(std::string name, bool linearFilter, bool flipTex)
{

    TextureHDR t;
    t.loadFromFile(name, true, flipTex);

    //XMMATRIX captureProjection = XMMatrixPerspectiveFovLH((XM_PI / 180) * 90, 1, 0.1, 10);
    //XMMATRIX captureViews
    //{
    //    XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(1, 0, 0, 0), XMVectorSet(0, -1, 0, 0)),
    //    XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(-1, 0, 0, 0), XMVectorSet(0, -1, 0, 0)),
    //    XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(0, -1, 0, 0), XMVectorSet(0, 0, 1, 0)),
    //    XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(0, 1, 0, 0), XMVectorSet(0, 0, -1, 0)),
    //    XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(0, 0, -1, 0), XMVectorSet(0, -1, 0, 0)),
    //    XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMVectorSet(0, 0, 1, 0), XMVectorSet(0, -1, 0, 0))
    //};

    //Create the TextureCube
    D3D11_TEXTURE2D_DESC textureDesc = {};

    const int SkyboxSize = 2048;
    const int IrradianceSize = 32;
    const int PreFilterSize = 256;

    textureDesc.Width = SkyboxSize;
    textureDesc.Height = SkyboxSize;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 6;
    textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    ID3D11Texture2D* tex = nullptr;
     GetDevice()->GetID3D11Device()->CreateTexture2D(&textureDesc, nullptr, &tex);
    textureDesc.Width = IrradianceSize;
    textureDesc.Height = IrradianceSize;
    ID3D11Texture2D* texIrradiance = nullptr;
     GetDevice()->GetID3D11Device()->CreateTexture2D(&textureDesc, nullptr, &texIrradiance);
    textureDesc.Width = PreFilterSize;
    textureDesc.Height = PreFilterSize;
    ID3D11Texture2D* texPreFilter = nullptr;
     GetDevice()->GetID3D11Device()->CreateTexture2D(&textureDesc, nullptr, &texPreFilter);
    //textureDesc.Width = BrdfLookupSize;
    //textureDesc.Height = BrdfLookupSize;
    //ID3D11Texture2D* BrdfLookupSize = nullptr;
    // GetDevice()->GetID3D11Device()->CreateTexture2D(&textureDesc, nullptr, &BrdfLookupSize);

    // Create the Shader Resource view for the texture cube
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;
    GetDevice()->GetID3D11Device()->CreateShaderResourceView(tex, &srvDesc, &pCubemapView);
    GetDevice()->GetID3D11Device()->CreateShaderResourceView(texIrradiance, &srvDesc, &pIrradiaceMapView);
    GetDevice()->GetID3D11Device()->CreateShaderResourceView(texPreFilter, &srvDesc, &pFilteredMapView);

    // Convolution texture
    // GetDevice()->GetID3D11Device()->CreateTexture2D(&textureDesc, nullptr, &texConv);
    // GetDevice()->GetID3D11Device()->CreateShaderResourceView(texConv, &srvDesc, &pTextureViewConvoluted);


    //Create the Render target views
    std::vector<ID3D11RenderTargetView*> rtvs;
    std::vector<ID3D11RenderTargetView*> preFilteredRtv;
    std::vector<ID3D11RenderTargetView*> IrradianceRtv;

    //std::vector<ID3D11RenderTargetView*> rtvsCon;

    for (uint32_t i = 0; i < 6; i++)
    {
        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
        renderTargetViewDesc.Format = textureDesc.Format;
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
        renderTargetViewDesc.Texture2DArray.MipSlice = 0;
        renderTargetViewDesc.Texture2DArray.FirstArraySlice = i;
        renderTargetViewDesc.Texture2DArray.ArraySize = 1;

        ID3D11RenderTargetView* view1 = nullptr;
        ID3D11RenderTargetView* view2 = nullptr;
        ID3D11RenderTargetView* view3 = nullptr;

        //ID3D11RenderTargetView* viewConv = nullptr;
         GetDevice()->GetID3D11Device()->CreateRenderTargetView(tex, &renderTargetViewDesc, &view1);
         GetDevice()->GetID3D11Device()->CreateRenderTargetView(texIrradiance, &renderTargetViewDesc, &view2);
         GetDevice()->GetID3D11Device()->CreateRenderTargetView(texPreFilter, &renderTargetViewDesc, &view3);

        // GetDevice()->GetID3D11Device()->CreateRenderTargetView(texConv, &renderTargetViewDesc, &viewConv);
        rtvs.push_back(view1);
        IrradianceRtv.push_back(view2);
        preFilteredRtv.push_back(view3);

        //rtvsCon.push_back(viewConv);
    }
    tex->Release();
    texIrradiance->Release();
    texPreFilter->Release();


    ////ENVIRONMENT MAP
    //MeshRenderer* mr = AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
    //mr->SetMeshByKey(L"Cubemesh");

    //Material* material = new Material;
    //GETSINGLE(ResourceMgr)->Insert<Material>(L"SkyboxMat", material);
    //material->SetShaderByKey(L"RectToCubemapShader");

    //mr->SetMaterial(material);

    Skybox cmap;
    //Shader s;
    //s.Create("res/Shaders/EquirectangulatToCubemap.hlsl");

    //ConstantBuffer cb;
    //cb.Create(nullptr, 16 * sizeof(float), 0, CONST_BUFFER_USAGE_VERTEX);

    D3D11_VIEWPORT vp;
    vp.Width = SkyboxSize;
    vp.Height = SkyboxSize;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    GetDevice()->GetDeviceContext()->RSSetViewports(1, &vp);

    //s.Bind();
    //t.Bind(0);
    //for (uint32_t i = 0; i < 6; ++i)
    //{
    //    float clearColor[4] = { 1.0f, 0.1f, 0.1f, 1.0f };
    //     GetDevice()->GetDeviceContext()->ClearRenderTargetView(rtvs[i], clearColor);
    //    XMMATRIX d = XMMatrixTranspose(captureViews[i] * captureProjection);
    //    cb.SetData(&d, 16 * sizeof(float));
    //    cb.Bind(0);
    //     GetDevice()->GetDeviceContext()->OMSetRenderTargets(1, &rtvs[i], nullptr);
    //    cmap.Draw();
    //}

    // IRADIANCE

    //Shader sirr;
    //sirr.Create("res/Shaders/irradiance.hlsl");
    //sirr.Bind();
    //vp.Width = IrradianceSize;
    //vp.Height = IrradianceSize;
    // GetDevice()->GetDeviceContext()->RSSetViewports(1, &vp);

    //t.Bind(0);

    //for (uint32_t i = 0; i < 6; ++i)
    //{
    //    float clearColor[4] = { 1.0f, 0.1f, 0.1f, 1.0f };
    //     GetDevice()->GetDeviceContext()->ClearRenderTargetView(IrradianceRtv[i], clearColor);
    //    XMMATRIX d = XMMatrixTranspose(captureViews[i] * captureProjection);
    //    cb.SetData(&d, 16 * sizeof(float));
    //    cb.Bind(0);
    //     GetDevice()->GetDeviceContext()->OMSetRenderTargets(1, &IrradianceRtv[i], nullptr);
    //    cmap.Draw();
    //    std::this_thread::sleep_for(std::chrono::microseconds(100)); // ¼º´É¶«½Ã
    //}

    //PREFILTERED MAP

    Shader* spre;
    spre = GETSINGLE(ResourceMgr)->Find<Shader>(L"PreFilterShader");
    spre->Bind();
    vp.Width = PreFilterSize;
    vp.Height = PreFilterSize;
     GetDevice()->GetDeviceContext()->RSSetViewports(1, &vp);

    t.Bind(0);

    for (uint32_t i = 0; i < 6; ++i)
    {
        float clearColor[4] = { 1.0f, 0.1f, 0.1f, 1.0f };
         GetDevice()->GetDeviceContext()->ClearRenderTargetView(preFilteredRtv[i], clearColor);
        //XMMATRIX d = XMMatrixTranspose(captureViews[i] * captureProjection);
         GetDevice()->GetDeviceContext()->OMSetRenderTargets(1, &preFilteredRtv[i], nullptr);
        cmap.Draw();
    }

    vp.Width = 1600.f;
    vp.Height = 900.f;
    GetDevice()->GetDeviceContext()->RSSetViewports(1, &vp);
}


void CubeMapHDR::Bind()
{
    Texture::Clears();
     //GetDevice()->GetDeviceContext()->PSSetShaderResources(0, 1, &pCubemapView);
     //GetDevice()->GetDeviceContext()->PSSetShaderResources(6, 1, &pIrradiaceMapView);
     GetDevice()->BindShaderResource(eShaderStage::PS, static_cast<UINT>(eTextureSlot::PrefilteredMap),  &pFilteredMapView);
}