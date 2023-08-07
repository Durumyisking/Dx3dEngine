#include "Application.h"
#include "GraphicDevice.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Shader.h"
#include "InputMgr.h"
#include "Texture.h"
#include "ResourceMgr.h"

#include <wincodec.h>
//#include <winrt/Windows.Storage.Pickers.h>
//#include <winrt/Windows.Storage.Streams.h>
//#include <winrt/Windows.Foundation.Collections.h>

extern Application application;




GraphicDevice::GraphicDevice(eValidationMode _ValidationMode)
{
	/*
			<summary>
			1. Device 와 SwapChain를 생성한다.
			2. 백버퍼에 실제로 렌더링할 렌더타겟 뷰를 생성한다.
			3. 화면을 클리어 해줘야한다. 뷰포트를 생성해줘야 한다.
			4. 매프레임마다 위에서 생성한 렌더타겟 뷰에 렌더링해준다.
			5. Swapchain을 이용하여 최종 디바이스(디스플레이)에 화면을 그려준다.
	*/

	GetDevice() = this;

	HWND hwnd = application.GetHwnd(); // 윈도우 핸들 얻어옴
				
	// Device, Device Context
	UINT DeviceFlag = D3D11_CREATE_DEVICE_DEBUG; // 디버그모드로 만들거임 릴리즈모드는 0
	D3D_FEATURE_LEVEL FeatureLevel = (D3D_FEATURE_LEVEL)0;

	// 그냥 bool 형 반환값과 같음 SUCCEEDED / FAILED로 나뉨
	// 함수 반환 성공하면 suc 아니면 fail 나옴
	HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, DeviceFlag
		, nullptr, 0, D3D11_SDK_VERSION, mDevice.GetAddressOf()
		, &FeatureLevel, mContext.GetAddressOf());

	// SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.OutputWindow = hwnd; // 렌더될 윈도우의 핸들
	swapChainDesc.Windowed = true; // 창모드 전체화면
	swapChainDesc.BufferCount = 2; // 사용할 렌더링 버퍼개수 최대 8
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 버퍼 렌더타겟으로 쓸거임
	swapChainDesc.BufferDesc.Width = application.GetWidth();
	swapChainDesc.BufferDesc.Height = application.GetHeight();
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM; // 컬러 포멧 타입
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;// 프레임 비율 분자
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1; // 프레임 비율 분모
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	/*
		sample desciption은 Direct3D의 멀티 샘플링 속성을 정의한다. 
		멀티 샘플링은 렌더된 픽셀들의 평균 샘플을 사용하여 최종 컬러를 좀더 부드럽게 하는 기술이다.
		샘플러와 관련된게 맞다.
	*/
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (!CreateSwapChain(&swapChainDesc))
		return;

	mRenderTargetTexture =  new Texture();

	Microsoft::WRL::ComPtr <ID3D11Texture2D> renderTarget;
	// Get rendertarget for swapchain
	//						0번 버퍼가 렌더타겟							렌더타겟 포인터
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)renderTarget.GetAddressOf());
	mRenderTargetTexture->Create(renderTarget);
	GETSINGLE(ResourceMgr)->Insert<Texture>(L"RenderTargetTexture", mRenderTargetTexture);
	// Create Rendertarget View

	mDepthStencilBufferTexture =  new Texture();
	mDepthStencilBufferTexture->Create(1600, 900, DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL);
	GETSINGLE(ResourceMgr)->Insert<Texture>(L"DepthStencilBufferTexture", mDepthStencilBufferTexture);
	
	// Setting Viewport		
	RECT winRect;
	GetClientRect(application.GetHwnd(), &winRect);
	mViewPort = { 0.f, 0.f, FLOAT(winRect.right - winRect.left), FLOAT(winRect.bottom - winRect.top), 0.f, 1.f };
	BindViewports(&mViewPort);

	// RenderTarget Set
	mContext->OMSetRenderTargets(1, mRenderTargetTexture->GetRTV().GetAddressOf(), mDepthStencilBufferTexture->GetDSV().Get());
}

GraphicDevice::~GraphicDevice()
{
	renderer::release();
}

bool GraphicDevice::CreateSwapChain(DXGI_SWAP_CHAIN_DESC* pDesc)
{
	Microsoft::WRL::ComPtr<IDXGIDevice> pDXGIDevice = nullptr; 
	Microsoft::WRL::ComPtr<IDXGIAdapter> pDXGIAdapter = nullptr; 
	Microsoft::WRL::ComPtr<IDXGIFactory> pDXGIFactory = nullptr; 

												// hwnd같이 gpu접근객체
	if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
		return false;
												// 내 그래픽 카드 정보가져옴
	if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pDXGIAdapter.GetAddressOf())))
		return false;
												// 전체화면 전환 관리 (swapchain 만들기 위해 필요)
	if (FAILED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pDXGIFactory.GetAddressOf())))
		return false;

	if (FAILED(pDXGIFactory->CreateSwapChain(mDevice.Get(), pDesc, mSwapChain.GetAddressOf())))
		return false;

	return true;
}

bool GraphicDevice::CreateTexture(D3D11_TEXTURE2D_DESC* pDesc, ID3D11Texture2D** ppTexture2D)
{
	if (FAILED(mDevice->CreateTexture2D(pDesc, nullptr, ppTexture2D)))
		return false;

	return true;
}

bool GraphicDevice::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* pDesc, UINT numElements, const void* bytecode, SIZE_T bytecodeLength, ID3D11InputLayout** ppInputLayout)
{
	if (FAILED(mDevice->CreateInputLayout(pDesc, numElements, bytecode, bytecodeLength, ppInputLayout)))
	{
		return false;
	}

	return true;
}


bool GraphicDevice::CreateBuffer(D3D11_BUFFER_DESC* pDESC, D3D11_SUBRESOURCE_DATA* data, ID3D11Buffer** buffer)
{
	// ram -> gpu 
	// input assembly 단계로 버퍼 넘겨주는행위

	if (FAILED(mDevice->CreateBuffer(pDESC, data, buffer)))
		return false;

	return true;
}

bool GraphicDevice::CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView)
{

	if (FAILED(mDevice->CreateShaderResourceView(pResource, pDesc, ppSRView)))
		return false;

	return true;
}

bool GraphicDevice::CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDSView)
{
	if (FAILED(mDevice->CreateDepthStencilView(pResource, pDesc, ppDSView)))
		return false;

	return true;
}

bool GraphicDevice::CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView)
{
	if (FAILED(mDevice->CreateRenderTargetView(pResource, pDesc, ppRTView)))
		return false;

	return true;
}

bool GraphicDevice::CreateUnorderedAccessView(ID3D11Resource* pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView)
{
	if (FAILED(mDevice->CreateUnorderedAccessView(pResource, pDesc, ppUAView)))
		return false;

	return true;
}

bool GraphicDevice::CreateVertexShader(const void* pShaderBytecode, SIZE_T bytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader)
{
	if (FAILED(mDevice->CreateVertexShader(pShaderBytecode, bytecodeLength, pClassLinkage, ppVertexShader)))
		return false;

	return true;
}

bool GraphicDevice::CreatePixelShader(const void* pShaderBytecode, SIZE_T bytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader)
{
	if (FAILED(mDevice->CreatePixelShader(pShaderBytecode, bytecodeLength, pClassLinkage, ppPixelShader)))
		return false;

	return true;
}

bool GraphicDevice::CreateGeometryShader(const void* pShaderBytecode, SIZE_T bytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader)
{
	if (FAILED(mDevice->CreateGeometryShader(pShaderBytecode, bytecodeLength, pClassLinkage, ppGeometryShader)))
		return false;

	return true;
}

bool GraphicDevice::CreateComputeShader(const void* pShaderBytecode, SIZE_T bytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11ComputeShader** ppComputeShader)
{
	if (FAILED(mDevice->CreateComputeShader(pShaderBytecode, bytecodeLength, pClassLinkage, ppComputeShader)))
		return false;

	return true;
}



bool GraphicDevice::CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState)
{
	if (FAILED(mDevice->CreateSamplerState(pSamplerDesc, ppSamplerState)))
		return false;

	return true;
}

bool GraphicDevice::CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState)
{
	if (FAILED(mDevice->CreateRasterizerState(pRasterizerDesc, ppRasterizerState)))
		return false;

	return true;
}

bool GraphicDevice::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D11DepthStencilState** ppDepthStencilState)
{
	if (FAILED(mDevice->CreateDepthStencilState(pDepthStencilDesc, ppDepthStencilState)))
		return false;

	return true;
}

bool GraphicDevice::CreateBlendState(const D3D11_BLEND_DESC* pBlendStateDesc, ID3D11BlendState** ppBlendState)
{
	if (FAILED(mDevice->CreateBlendState(pBlendStateDesc, ppBlendState)))
		return false;

	return true;
}


void GraphicDevice::BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	mContext->IASetPrimitiveTopology(topology);
}

void GraphicDevice::BindInputLayout(ID3D11InputLayout* inputLayout)
{
	mContext->IASetInputLayout(inputLayout	);
}

void GraphicDevice::BindVertexBuffer(UINT startSlot, UINT numBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets)
{
	mContext->IASetVertexBuffers(startSlot, numBuffers, ppVertexBuffers, pStrides, pOffsets); // vertex buffer set
}

void GraphicDevice::BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT format, UINT offset)
{
	mContext->IASetIndexBuffer(pIndexBuffer, format, offset); // index buffer set
}

void GraphicDevice::BindVS(ID3D11VertexShader* pVS, ID3D11ClassInstance* const* classInst, UINT numClassInst)
{
	mContext->VSSetShader(pVS, classInst, numClassInst);
}

void GraphicDevice::BindHS(ID3D11HullShader* pHS, ID3D11ClassInstance* const* classInst, UINT numClassInst)
{
	mContext->HSSetShader(pHS, classInst, numClassInst);
}

void GraphicDevice::BindDS(ID3D11DomainShader* pDS, ID3D11ClassInstance* const* classInst, UINT numClassInst)
{
	mContext->DSSetShader(pDS, classInst, numClassInst);
}

void GraphicDevice::BindGS(ID3D11GeometryShader* pGS, ID3D11ClassInstance* const* classInst, UINT numClassInst)
{
	mContext->GSSetShader(pGS, classInst, numClassInst);
}

void GraphicDevice::BindPS(ID3D11PixelShader* pPS, ID3D11ClassInstance* const* classInst, UINT numClassInst)
{
	mContext->PSSetShader(pPS, classInst, numClassInst);
}

void GraphicDevice::BindCS(ID3D11ComputeShader* pCS, ID3D11ClassInstance* const* classInst, UINT numClassInst)
{
	mContext->CSSetShader(pCS, classInst, numClassInst);
}

void GraphicDevice::Dispatch(UINT threadGroupCountX, UINT threadGroupCountY, UINT threadGroupCountZ)
{
	mContext->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
}



void GraphicDevice::BindViewports(D3D11_VIEWPORT* viewPort)
{
	mContext->RSSetViewports(1, viewPort);
}

void GraphicDevice::GetData(ID3D11Buffer* buffer, void* data, UINT size)
{
	D3D11_MAPPED_SUBRESOURCE sub = {};
	mContext->Map(buffer, 0, D3D11_MAP_READ, 0, &sub);
	memcpy(data, sub.pData, size);
	mContext->Unmap(buffer, 0);
}

void GraphicDevice::SetData(ID3D11Buffer* buffer, void* data, UINT size)
{
	D3D11_MAPPED_SUBRESOURCE sub = {};
	mContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	memcpy(sub.pData, data, size);
	mContext->Unmap(buffer, 0);
}

void GraphicDevice::CopyResource(ID3D11Resource* dstResource, ID3D11Resource* srcResource)
{
	mContext->CopyResource(dstResource, srcResource);
}

void GraphicDevice::BindBuffer(ID3D11Buffer* buffer, void* data, UINT size)
{
	// gpu에 값 줄거니까 데이터 바꿔서 보내야해
	D3D11_MAPPED_SUBRESOURCE sub = {};
	mContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub); // 다른 애들이 Buffer를 사용 못하게 점유하도록 함		
	memcpy(sub.pData, data, size); // GPU로 값 복사해줌
	mContext->Unmap(buffer, 0); // 점유 해제
}

void GraphicDevice::ClearConstantBuffer(ID3D11Buffer* buffer, UINT size)
{
	D3D11_MAPPED_SUBRESOURCE sub = {};
	mContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
	memset(sub.pData, 0, size);
	mContext->Unmap(buffer, 0);
}

void GraphicDevice::BindConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer)
{
	switch (stage)
	{
	case eShaderStage::VS:
		mContext->VSSetConstantBuffers(static_cast<UINT>(type), 1, &buffer);
		break;
	case eShaderStage::HS:
		mContext->HSSetConstantBuffers(static_cast<UINT>(type), 1, &buffer);
		break;
	case eShaderStage::DS:
		mContext->DSSetConstantBuffers(static_cast<UINT>(type), 1, &buffer);
		break;
	case eShaderStage::GS:
		mContext->GSSetConstantBuffers(static_cast<UINT>(type), 1, &buffer);
		break;
	case eShaderStage::PS:
		mContext->PSSetConstantBuffers(static_cast<UINT>(type), 1, &buffer);
		break;
	case eShaderStage::CS:
		mContext->CSSetConstantBuffers(static_cast<UINT>(type), 1, &buffer);
		break;
	default:
		break;
	}
}

void GraphicDevice::BindShaderResource(eShaderStage stage, UINT slot, ID3D11ShaderResourceView* const* shaderResourceViews)
{
	switch (stage)
	{
	case eShaderStage::VS:
		mContext->VSSetShaderResources(slot, 1, shaderResourceViews);
		break;
	case eShaderStage::HS:
		mContext->HSSetShaderResources(slot, 1, shaderResourceViews);
		break;
	case eShaderStage::DS:
		mContext->DSSetShaderResources(slot, 1, shaderResourceViews);
		break;
	case eShaderStage::GS:
		mContext->GSSetShaderResources(slot, 1, shaderResourceViews);
		break;
	case eShaderStage::PS:
		mContext->PSSetShaderResources(slot, 1, shaderResourceViews);
		break;
	case eShaderStage::CS:
		mContext->CSSetShaderResources(slot, 1, shaderResourceViews);
		break;
	default:
		break;
	}
}

void GraphicDevice::BindUnorderedAccessView(UINT slot, UINT numUAVs, ID3D11UnorderedAccessView* const* unorderedAccessView, const UINT* UAVInitialCount)
{
	mContext->CSSetUnorderedAccessViews(slot, numUAVs, unorderedAccessView, UAVInitialCount);
}

void GraphicDevice::BindSamplers(eShaderStage stage, UINT slot, UINT numSamplers, ID3D11SamplerState* const* samplerState)
{
	switch (stage)
	{
	case eShaderStage::VS:
		mContext->VSSetSamplers(slot, numSamplers, samplerState);
		break;
	case eShaderStage::HS:
		mContext->HSSetSamplers(slot, numSamplers, samplerState);
		break;
	case eShaderStage::DS:
		mContext->DSSetSamplers(slot, numSamplers, samplerState);
		break;
	case eShaderStage::GS:
		mContext->GSSetSamplers(slot, numSamplers, samplerState);
		break;
	case eShaderStage::PS:
		mContext->PSSetSamplers(slot, numSamplers, samplerState);
		break;
	default:
		break;
	}	
}

void GraphicDevice::BindSamplers(UINT slot, UINT numSamplers, ID3D11SamplerState* const* samplerState)
{
	mContext->VSSetSamplers(slot, numSamplers, samplerState);
	mContext->HSSetSamplers(slot, numSamplers, samplerState);
	mContext->DSSetSamplers(slot, numSamplers, samplerState);
	mContext->GSSetSamplers(slot, numSamplers, samplerState);
	mContext->PSSetSamplers(slot, numSamplers, samplerState);		
}

void GraphicDevice::BindRasterizerState(ID3D11RasterizerState* rasterizerState)
{
	mContext->RSSetState(rasterizerState);
}

void GraphicDevice::BindDepthStencilState(ID3D11DepthStencilState* depthStencilState)
{
	mContext->OMSetDepthStencilState(depthStencilState, 0);
}

void GraphicDevice::BindBlendState(ID3D11BlendState* blendState)
{
	mContext->OMSetBlendState(blendState, nullptr, 0xffffff);
}


void GraphicDevice::Clear()
{
	// clear target
	FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	mContext->ClearRenderTargetView(mRenderTargetTexture->GetRTV().Get(), backgroundColor); // 지우고 다시그림
	mContext->ClearDepthStencilView(mDepthStencilBufferTexture->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0); // 깊이버퍼도 클리어 해줘야해
}

void GraphicDevice::ClearRenderTargetView(ID3D11RenderTargetView* renderTargetView, const FLOAT colorRGBA[4])
{
	mContext->ClearRenderTargetView(renderTargetView, colorRGBA);
}

void GraphicDevice::ClearDepthStencilView(ID3D11DepthStencilView* depthStencilView, UINT clearFlags)
{
	mContext->ClearDepthStencilView(depthStencilView, clearFlags, 1.0f, 0);
}

void GraphicDevice::AdjustViewPorts()
{
	RECT winRect;
	GetClientRect(application.GetHwnd(), &winRect);
	mViewPort = { 0.f, 0.f, FLOAT(winRect.right - winRect.left), FLOAT(winRect.bottom - winRect.top), 0.f, 1.f };
	BindViewports(&mViewPort);
	mContext->OMSetRenderTargets(1, mRenderTargetTexture->GetRTV().GetAddressOf(), mDepthStencilBufferTexture->GetDSV().Get());
}

void GraphicDevice::OMSetRenderTarget()
{
	mContext->OMSetRenderTargets(1
		, mRenderTargetTexture->GetRTV().GetAddressOf()
		, mDepthStencilBufferTexture->GetDSV().Get());
}

void GraphicDevice::OMSetRenderTarget(UINT numViews, ID3D11RenderTargetView** renderTargetViews, ID3D11DepthStencilView* depthStencilView)
{
	mContext->OMSetRenderTargets(numViews, renderTargetViews, depthStencilView);
}

void GraphicDevice::Draw()
{
	mContext->Draw(0, 0);
}


void GraphicDevice::DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
{
	mContext->DrawIndexed(indexCount, startIndexLocation, startIndexLocation);
}

void GraphicDevice::DrawIndexedInstanced(UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation, INT baseVertexLocation, UINT startInstanceLocation)
{
	mContext->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

void GraphicDevice::Present()
{
	mSwapChain->Present(0, 0); // 두번째 인자는 윈도우가 아예 표시되지않을때 렌더링 할까말까 고르는거
	//mSwapChain->Present(1, 0); // 수직동기화 on
}
