#pragma once
#include "Graphics.h"


class GraphicDevice
{
public:
	GraphicDevice(eValidationMode _ValidationMode = eValidationMode::Disabled);
	~GraphicDevice();

	bool CreateSwapChain(DXGI_SWAP_CHAIN_DESC* pDesc);
	bool CreateTexture(D3D11_TEXTURE2D_DESC* pDesc, ID3D11Texture2D** ppTexture2D);

	bool CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* pDesc, UINT numElements, const void* bytecode, SIZE_T bytecodeLength, ID3D11InputLayout** ppInputLayout);
	bool CreateBuffer(D3D11_BUFFER_DESC* pDESC, D3D11_SUBRESOURCE_DATA* data, ID3D11Buffer** buffer);
	bool CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView = 0);
	bool CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDSView = 0);
	bool CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView = 0);
	bool CreateUnorderedAccessView(ID3D11Resource* pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView = 0);

	bool CreateVertexShader(const void* pShaderBytecode, SIZE_T bytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader);
	bool CreatePixelShader(const void* pShaderBytecode, SIZE_T bytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader);
	bool CreateGeometryShader(const void* pShaderBytecode, SIZE_T bytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader);
	bool CreateComputeShader(const void* pShaderBytecode, SIZE_T bytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11ComputeShader** ppComputeShader);

	bool CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState);
	bool CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState);
	bool CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D11DepthStencilState** ppDepthStencilState);
	bool CreateBlendState(const D3D11_BLEND_DESC* pBlendStateDesc, ID3D11BlendState** ppBlendState);
	

	void BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
	void BindInputLayout(ID3D11InputLayout* inputLayout);
	void BindVertexBuffer(UINT startSlot, UINT numBuffers, ID3D11Buffer *const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets);
	void BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT format, UINT offset);

	void BindVS(ID3D11VertexShader* pVS, ID3D11ClassInstance* const* classInst, UINT numClassInst);
	void BindHS(ID3D11HullShader* pHS, ID3D11ClassInstance* const* classInst, UINT numClassInst);
	void BindDS(ID3D11DomainShader* pDS, ID3D11ClassInstance* const* classInst, UINT numClassInst);
	void BindGS(ID3D11GeometryShader* pGS, ID3D11ClassInstance* const* classInst, UINT numClassInst);
	void BindPS(ID3D11PixelShader* pPS, ID3D11ClassInstance* const* classInst, UINT numClassInst);
	void BindCS(ID3D11ComputeShader* pCS, ID3D11ClassInstance* const* classInst, UINT numClassInst);
	void Dispatch(UINT threadGroupCountX, UINT threadGroupCountY, UINT threadGroupCountZ);

	void BindViewports(D3D11_VIEWPORT* viewPort);
	void GetData(ID3D11Buffer* buffer, void* data, UINT size);
	void SetData(ID3D11Buffer* buffer, void* data, UINT size);
	void CopyResource(ID3D11Resource* dstResource, ID3D11Resource* srcResource);
	void BindBuffer(ID3D11Buffer* buffer, void* data, UINT size);
	void ClearConstantBuffer(ID3D11Buffer* buffer, UINT size);
	void BindConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer); // ���̴��� ������� ������ ������

	void BindShaderResource(eShaderStage stage, UINT slot, ID3D11ShaderResourceView* const* shaderResourceViews);
							// ����̴�			//�Ÿ��

	void BindUnorderedAccessView(UINT slot, UINT numUAVs, ID3D11UnorderedAccessView* const* unorderedAccessView, const UINT* pUAVInitialCount);

	void BindSamplers(eShaderStage stage, UINT slot, UINT numSamplers, ID3D11SamplerState* const* samplerState);
	void BindSamplers(UINT slot, UINT numSamplers, ID3D11SamplerState* const* samplerState);

	void BindRasterizerState(ID3D11RasterizerState* rasterizerState);
	void BindDepthStencilState(ID3D11DepthStencilState* depthStencilState);
	void BindBlendState(ID3D11BlendState* blendState);


	void Clear();
	void AdjustViewPorts();
	void OMSetRenderTarget();
	void OMSetRenderTarget(UINT numViews, ID3D11RenderTargetView** renderTargetViews, ID3D11DepthStencilView* depthStencilView);

	void Draw();
	void DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation);
	void DrawIndexedInstanced(UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation, INT baseVertexLocation, UINT startInstanceLocation);

	void Present();


	Microsoft::WRL::ComPtr <ID3D11DeviceContext> GetDeviceContext() const { return mContext; }
	ID3D11Device* GetID3D11Device() { return mDevice.Get(); }

	D3D11_VIEWPORT GetViewPort() const { return mViewPort; }
	float ViewportWidth() { return (float)mViewPort.Width; }
	float ViewportHeight() { return (float)mViewPort.Height; }


private:
	// GPU ��ü ���� �׷���ī��� ����Ǵ� �⺻���� ��ü 
	Microsoft::WRL::ComPtr <ID3D11Device> mDevice; // ComPtr : �� ����� ������ �ִ� Ŭ������ �Ҹ��ڰ� ȣ��ɶ� �ش� ����� Ÿ���� �Ҹ��ڰ� �ڵ����� ȣ��ȴ�.
	// GPU read write ����̽��� ���� �������� �ʰ� �̰�ü�� ���ؼ� gpu�� ����� ������.
	// ����̽��� ������ �κп� ������ �Ȱ��� �Լ���� �����Ǿ��ִ�.
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> mContext;

	// ���������� �׷����� ��ȭ��

	class Texture* mRenderTargetTexture;
	class Texture* mDepthStencilBufferTexture;



	// ȭ�鿡 ���������� �׷����� 
	// �����(Frame buffer)�� �����ϰ�, ������ ȭ�鿡 ������ �ϴ� 
	// ������ ����ϴ� ��ü
	Microsoft::WRL::ComPtr <IDXGISwapChain> mSwapChain;

	//�ؽ�ó �ε��Ҷ� ���ȴ�.
	// ID3D11SamplerState* mSampler[];

	D3D11_VIEWPORT mViewPort;
};

inline GraphicDevice*& GetDevice()
{
	static GraphicDevice* device = nullptr; // static�̶� ó�� �ѹ��� �ʱ�ȭ �ϰ� �㿡�� ��������
	return device;
}

