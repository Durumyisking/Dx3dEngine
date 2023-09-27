#include "Renderer.h"
#include "ResourceMgr.h"
#include "Material.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "ParticleShader.h"
#include "TimeMgr.h"
#include "Application.h"
#include "AudioClip.h"
#include "FileMgr.h"
#include "CubeMap.h"

extern Application application;

namespace renderer
{

	ConstantBuffer* constantBuffers[static_cast<UINT>(eCBType::End)] = {};
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState[static_cast<UINT>(eSamplerType::End)];
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState[static_cast<UINT>(eRasterizerType::End)];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState[static_cast<UINT>(eDepthStencilType::End)];
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState[static_cast<UINT>(eBlendStateType::End)];

	Camera* mainCamera = nullptr;
	Camera* UICamera = nullptr;
	std::vector<Camera*> Cameras[static_cast<UINT>(SceneMgr::eSceneType::End)];
	std::vector<DebugMesh> debugMeshes;
	std::vector<Light*> lights;
	std::vector<LightAttribute> lightAttributes;
	StructedBuffer* lightBuffer = nullptr;

	Texture* postProcessTexture = nullptr;
	Texture* dsTexture = nullptr;
	GameObj* outlineGameObject = nullptr;

	MultiRenderTarget* renderTargets[static_cast<UINT>(eRenderTargetType::End)] = {};

	void LoadMesh()
	{

		CreatePointMesh();
		CreateLineMesh();
		CreateRectMesh();
		CreateGridMesh();
		CreateCircleMesh();
		CreateCubeMesh();
		CreateSphereMesh();
		CreateCapsuleMesh();
	}


	void SetUpState()
	{

#pragma region InputLayout
		D3D11_INPUT_ELEMENT_DESC arrLayout[8] = {};

		UINT offset = 0;
		arrLayout[0].AlignedByteOffset = offset;
		arrLayout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayout[0].InputSlot = 0;
		arrLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[0].SemanticName = "POSITION";
		arrLayout[0].SemanticIndex = 0;
		offset += sizeof(float) * 4;

		arrLayout[1].AlignedByteOffset = offset;
		arrLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		arrLayout[1].InputSlot = 0;
		arrLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[1].SemanticName = "TEXCOORD";
		arrLayout[1].SemanticIndex = 0;
		offset += sizeof(float) * 2;

		arrLayout[2].AlignedByteOffset = offset;
		arrLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		arrLayout[2].InputSlot = 0;
		arrLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[2].SemanticName = "TANGENT";
		arrLayout[2].SemanticIndex = 0;
		offset += sizeof(float) * 3;

		arrLayout[3].AlignedByteOffset = offset;
		arrLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		arrLayout[3].InputSlot = 0;
		arrLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[3].SemanticName = "NORMAL";
		arrLayout[3].SemanticIndex = 0;
		offset += sizeof(float) * 3;

		arrLayout[4].AlignedByteOffset = offset;
		arrLayout[4].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayout[4].InputSlot = 0;
		arrLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[4].SemanticName = "BLENDINDICES";
		arrLayout[4].SemanticIndex = 0;
		offset += sizeof(float) * 4;

		arrLayout[5].AlignedByteOffset = offset;
		arrLayout[5].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayout[5].InputSlot = 0;
		arrLayout[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayout[5].SemanticName = "BLENDWEIGHT";
		arrLayout[5].SemanticIndex = 0;

		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"SpriteShader");
			GetDevice()->CreateInputLayout(arrLayout, 3
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"UIShader");
			GetDevice()->CreateInputLayout(arrLayout, 3
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"ColorShader");
			GetDevice()->CreateInputLayout(arrLayout, 3
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"GridShader");
			GetDevice()->CreateInputLayout(arrLayout, 3
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DebugShader");
			GetDevice()->CreateInputLayout(arrLayout, 3
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"ParticleShader");
			GetDevice()->CreateInputLayout(arrLayout, 3
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PostProcessShader");
			GetDevice()->CreateInputLayout(arrLayout, 3
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DebugGeometryShader");
			GetDevice()->CreateInputLayout(arrLayout, 3
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PhongShader");
			GetDevice()->CreateInputLayout(arrLayout, 6
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"FlatShader");
			GetDevice()->CreateInputLayout(arrLayout, 6
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PBRShader");
			GetDevice()->CreateInputLayout(arrLayout, 6
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DeferredShader");
			GetDevice()->CreateInputLayout(arrLayout, 6
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"MergeShader");
			GetDevice()->CreateInputLayout(arrLayout, 1
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DepthShader");
			GetDevice()->CreateInputLayout(arrLayout, 1
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		} 
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"LightDirShader");
			GetDevice()->CreateInputLayout(arrLayout, 1
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"LightPointShader");
			GetDevice()->CreateInputLayout(arrLayout, 1
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"SkySphereShader");
			GetDevice()->CreateInputLayout(arrLayout, 2
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"IrradianceShader");
			GetDevice()->CreateInputLayout(arrLayout, 1
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PreFilterShader");
			GetDevice()->CreateInputLayout(arrLayout, 1
				, shader->GetVSBlobBufferPointer()
				, shader->GetVSBlobBufferSize()
				, shader->GetInputLayoutAddr());
		}

		Shader* uiSpriteShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"UISpriteShader");
		GetDevice()->CreateInputLayout(arrLayout, 2
			, uiSpriteShader->GetVSBlobBufferPointer()
			, uiSpriteShader->GetVSBlobBufferSize()
			, uiSpriteShader->GetInputLayoutAddr());

#pragma endregion

#pragma region SamplerState
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		//samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		//samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		//samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;

		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;

		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MinLOD = 0.0f;

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerState[static_cast<UINT>(eSamplerType::Point)].GetAddressOf());
		samplerDesc.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerState[static_cast<UINT>(eSamplerType::Linear)].GetAddressOf());
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerState[static_cast<UINT>(eSamplerType::Anisotropic)].GetAddressOf());

		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerState[static_cast<UINT>(eSamplerType::Skybox)].GetAddressOf());

		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerState[static_cast<UINT>(eSamplerType::Clamp)].GetAddressOf());

		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.BorderColor[0] = 1.f;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerState[static_cast<UINT>(eSamplerType::ShadowPoint)].GetAddressOf());

		GetDevice()->BindSamplers(static_cast<UINT>(eSamplerType::Point), 1, samplerState[static_cast<UINT>(eSamplerType::Point)].GetAddressOf());
		GetDevice()->BindSamplers(static_cast<UINT>(eSamplerType::Linear), 1, samplerState[static_cast<UINT>(eSamplerType::Linear)].GetAddressOf());
		GetDevice()->BindSamplers(static_cast<UINT>(eSamplerType::Anisotropic), 1, samplerState[static_cast<UINT>(eSamplerType::Anisotropic)].GetAddressOf());
		GetDevice()->BindSamplers(static_cast<UINT>(eSamplerType::Skybox), 1, samplerState[static_cast<UINT>(eSamplerType::Skybox)].GetAddressOf());
		GetDevice()->BindSamplers(static_cast<UINT>(eSamplerType::Clamp), 1, samplerState[static_cast<UINT>(eSamplerType::Clamp)].GetAddressOf());
		GetDevice()->BindSamplers(static_cast<UINT>(eSamplerType::ShadowPoint), 1, samplerState[static_cast<UINT>(eSamplerType::ShadowPoint)].GetAddressOf());

#pragma endregion

#pragma region RasterizerState
		D3D11_RASTERIZER_DESC  reDesc = {};

		//reDesc.DepthClipEnable = true;
		//reDesc.FrontCounterClockwise = false;

		reDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		reDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		GetDevice()->CreateRasterizerState(&reDesc, rasterizerState[static_cast<UINT>(eRasterizerType::SolidBack)].GetAddressOf());

		reDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		reDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		GetDevice()->CreateRasterizerState(&reDesc, rasterizerState[static_cast<UINT>(eRasterizerType::SolidFront)].GetAddressOf());

		reDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		reDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizerState(&reDesc, rasterizerState[static_cast<UINT>(eRasterizerType::SolidNone)].GetAddressOf());

		reDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		reDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizerState(&reDesc, rasterizerState[static_cast<UINT>(eRasterizerType::WireframeNone)].GetAddressOf());

#pragma endregion

#pragma region DepthStencilState

		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(&dsDesc, depthStencilState[static_cast<UINT>(eDepthStencilType::UI)].GetAddressOf());

		dsDesc.DepthEnable = true; // 깊이값 사용할지 말지
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL; // depth 값이 작거나 같을때 그림
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL; // depth buffer 껏다켰다할때 사용
		dsDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(&dsDesc, depthStencilState[static_cast<UINT>(eDepthStencilType::Less)].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER; // depth 값이 크거나 같을때 그림
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(&dsDesc, depthStencilState[static_cast<UINT>(eDepthStencilType::Greater)].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(&dsDesc, depthStencilState[static_cast<UINT>(eDepthStencilType::NoWrite)].GetAddressOf());

		dsDesc.DepthEnable = false;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(&dsDesc, depthStencilState[static_cast<UINT>(eDepthStencilType::None)].GetAddressOf());

#pragma endregion

#pragma region BlendState

		blendState[static_cast<UINT>(eBlendStateType::Default)] = nullptr;

		D3D11_BLEND_DESC bsDesc = {};
		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;
		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		GetDevice()->CreateBlendState(&bsDesc, blendState[static_cast<UINT>(eBlendStateType::AlphaBlend)].GetAddressOf());

		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;
		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		GetDevice()->CreateBlendState(&bsDesc, blendState[static_cast<UINT>(eBlendStateType::OneOne)].GetAddressOf());


#pragma endregion

	}


	void LoadBuffer()
	{

		constantBuffers[static_cast<UINT>(eCBType::Transform)] = new ConstantBuffer(eCBType::Transform);
		constantBuffers[static_cast<UINT>(eCBType::Transform)]->Create(sizeof(TransformCB));

		constantBuffers[static_cast<UINT>(eCBType::Material)] = new ConstantBuffer(eCBType::Material);
		constantBuffers[static_cast<UINT>(eCBType::Material)]->Create(sizeof(MaterialCB));

		constantBuffers[static_cast<UINT>(eCBType::Grid)] = new ConstantBuffer(eCBType::Grid);
		constantBuffers[static_cast<UINT>(eCBType::Grid)]->Create(sizeof(GridCB));

		constantBuffers[static_cast<UINT>(eCBType::Color)] = new ConstantBuffer(eCBType::Color);
		constantBuffers[static_cast<UINT>(eCBType::Color)]->Create(sizeof(ColorCB));

		constantBuffers[static_cast<UINT>(eCBType::Animation)] = new ConstantBuffer(eCBType::Animation);
		constantBuffers[static_cast<UINT>(eCBType::Animation)]->Create(sizeof(AnimationCB));

		constantBuffers[static_cast<UINT>(eCBType::Light)] = new ConstantBuffer(eCBType::Light);
		constantBuffers[static_cast<UINT>(eCBType::Light)]->Create(sizeof(LightCB));

		constantBuffers[static_cast<UINT>(eCBType::ParticleSystem)] = new ConstantBuffer(eCBType::ParticleSystem);
		constantBuffers[static_cast<UINT>(eCBType::ParticleSystem)]->Create(sizeof(ParticleSystemCB));

		constantBuffers[static_cast<UINT>(eCBType::Noise)] = new ConstantBuffer(eCBType::Noise);
		constantBuffers[static_cast<UINT>(eCBType::Noise)]->Create(sizeof(NoiseCB));

		constantBuffers[static_cast<UINT>(eCBType::PostProcess)] = new ConstantBuffer(eCBType::PostProcess);
		constantBuffers[static_cast<UINT>(eCBType::PostProcess)]->Create(sizeof(PostProcessCB));

		constantBuffers[static_cast<UINT>(eCBType::CubeMapProj)] = new ConstantBuffer(eCBType::CubeMapProj);
		constantBuffers[static_cast<UINT>(eCBType::CubeMapProj)]->Create(sizeof(SkyCB));

		constantBuffers[static_cast<UINT>(eCBType::LightMatrix)] = new ConstantBuffer(eCBType::LightMatrix);
		constantBuffers[static_cast<UINT>(eCBType::LightMatrix)]->Create(sizeof(LightMatrixCB));

		lightBuffer = new StructedBuffer();
		lightBuffer->Create(sizeof(LightAttribute), 128, eSRVType::SRV, nullptr, true);
	}

	void LoadShader()
	{
#pragma region DebugGeometryShader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"DebugGeometryVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"DebugGeometryPS.hlsl", "main");
			GETSINGLE(ResourceMgr)->Insert<Shader>(L"DebugGeometryShader", shader);
		}
#pragma endregion

#pragma region PhongShader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"PhongVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"PhongPS.hlsl", "main");
			GETSINGLE(ResourceMgr)->Insert<Shader>(L"PhongShader", shader);
		}
#pragma endregion

#pragma region FlatShader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"FlatVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"FlatPS.hlsl", "main");
			GETSINGLE(ResourceMgr)->Insert<Shader>(L"FlatShader", shader);
		}
#pragma endregion

#pragma region PBRShader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"PBRVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"PBRPS.hlsl", "main");
			GETSINGLE(ResourceMgr)->Insert<Shader>(L"PBRShader", shader);
			shader->SetBSState(eBlendStateType::Default);

		}
#pragma endregion

#pragma region SpriteShader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"SpritePS.hlsl", "main");
			GETSINGLE(ResourceMgr)->Insert<Shader>(L"SpriteShader", shader);
		}
#pragma endregion

#pragma region GridShader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"GridVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"GridPS.hlsl", "main");
			GETSINGLE(ResourceMgr)->Insert<Shader>(L"GridShader", shader);
		}
#pragma endregion

#pragma region UIShader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"UIPS.hlsl", "main");
			GETSINGLE(ResourceMgr)->Insert<Shader>(L"UIShader", shader);
		}
#pragma endregion

#pragma region ColorShader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"ColorPS.hlsl", "main");
			GETSINGLE(ResourceMgr)->Insert<Shader>(L"ColorShader", shader);
		}
#pragma endregion

#pragma region DebugShader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"DebugVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"DebugPS.hlsl", "main");
			shader->SetRSState(eRasterizerType::SolidNone);
			shader->SetDSState(eDepthStencilType::NoWrite);
			shader->SetBSState(eBlendStateType::AlphaBlend);
			shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
			GETSINGLE(ResourceMgr)->Insert<Shader>(L"DebugShader", shader);
		}
#pragma endregion

#pragma region ParticleShader
		{
			Shader* particleShader = new Shader();
			particleShader->Create(eShaderStage::VS, L"ParticleVS.hlsl", "main");
			particleShader->Create(eShaderStage::GS, L"ParticleGS.hlsl", "main");
			particleShader->Create(eShaderStage::PS, L"ParticlePS.hlsl", "main");
			particleShader->SetRSState(eRasterizerType::SolidNone);
			particleShader->SetDSState(eDepthStencilType::NoWrite);
			particleShader->SetBSState(eBlendStateType::AlphaBlend);
			particleShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			GETSINGLE(ResourceMgr)->Insert<Shader>(L"ParticleShader", particleShader);

			ParticleShader* particleCS = new ParticleShader();
			GETSINGLE(ResourceMgr)->Insert<ParticleShader>(L"ParticleCS", particleCS);
			particleCS->Create(L"ParticleCS.hlsl", "main");
		}
#pragma endregion

#pragma region PostProcessShader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"PostProcessVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"PostProcessPS.hlsl", "main");
			shader->SetDSState(eDepthStencilType::NoWrite);
			GETSINGLE(ResourceMgr)->Insert<Shader>(L"PostProcessShader", shader);
		}
#pragma endregion

#pragma region DeferredShader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"DeferredVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"DeferredPS.hlsl", "main");
			GETSINGLE(ResourceMgr)->Insert<Shader>(L"DeferredShader", shader);
		}
#pragma endregion

#pragma region MergeShader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"MergeVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"MergePS.hlsl", "main");
			shader->SetRSState(eRasterizerType::SolidBack);
			shader->SetDSState(eDepthStencilType::None);
			shader->SetBSState(eBlendStateType::Default);

			GETSINGLE(ResourceMgr)->Insert<Shader>(L"MergeShader", shader);
		}
#pragma endregion

#pragma region LightDirShader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"LightDirVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"LightDirPS.hlsl", "main");
			shader->SetRSState(eRasterizerType::SolidBack);
			shader->SetDSState(eDepthStencilType::None);
			shader->SetBSState(eBlendStateType::OneOne);

			GETSINGLE(ResourceMgr)->Insert<Shader>(L"LightDirShader", shader);
		}
#pragma endregion

#pragma region LightPointShader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"LightPointVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"LightPointPS.hlsl", "main");
			shader->SetRSState(eRasterizerType::SolidFront);
			shader->SetDSState(eDepthStencilType::None);
			shader->SetBSState(eBlendStateType::OneOne);

			GETSINGLE(ResourceMgr)->Insert<Shader>(L"LightPointShader", shader);
		}
#pragma endregion

#pragma region DepthShader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"DepthVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"DepthPS.hlsl", "main");
			shader->SetRSState(eRasterizerType::SolidBack);
			shader->SetDSState(eDepthStencilType::Less);
			shader->SetBSState(eBlendStateType::Default);

			GETSINGLE(ResourceMgr)->Insert<Shader>(L"DepthShader", shader);
		}
#pragma endregion

#pragma region SkySphere Shader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"SkySphereVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"SkySpherePS.hlsl", "main");
			shader->SetRSState(eRasterizerType::SolidFront);
			shader->SetDSState(eDepthStencilType::Less);
			shader->SetBSState(eBlendStateType::Default);

			GETSINGLE(ResourceMgr)->Insert<Shader>(L"SkySphereShader", shader);
		}
#pragma endregion

#pragma region SkySphere Shader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"IrradianceVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"IrradiancePS.hlsl", "main");
			shader->SetRSState(eRasterizerType::SolidFront);
			shader->SetDSState(eDepthStencilType::Less);
			shader->SetBSState(eBlendStateType::AlphaBlend);

			GETSINGLE(ResourceMgr)->Insert<Shader>(L"IrradianceShader", shader);
		}
#pragma endregion
#pragma region SkySphere Shader
		{
			Shader* shader = new Shader();
			shader->Create(eShaderStage::VS, L"PreFilterVS.hlsl", "main");
			shader->Create(eShaderStage::PS, L"PreFilterPS.hlsl", "main");
			shader->SetRSState(eRasterizerType::SolidFront);
			shader->SetDSState(eDepthStencilType::Less);
			shader->SetBSState(eBlendStateType::AlphaBlend);

			GETSINGLE(ResourceMgr)->Insert<Shader>(L"PreFilterShader", shader);
		}
#pragma endregion


#pragma region UISprite Shader
		Shader* uiSS = new Shader();
		uiSS->Create(eShaderStage::VS, L"UISpriteVS.hlsl", "main");
		uiSS->Create(eShaderStage::PS, L"UISpritePS.hlsl", "main");
		uiSS->SetRSState(eRasterizerType::SolidNone);
		uiSS->SetDSState(eDepthStencilType::UI);
		uiSS->SetBSState(eBlendStateType::AlphaBlend);
		GETSINGLE(ResourceMgr)->Insert<Shader>(L"UISpriteShader", uiSS);
#pragma endregion

	}

	void LoadLoadingSceneTexture()
	{
		GETSINGLE(ResourceMgr)->Load<Texture>(L"noise1", L"noise/noise_01.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"noise2", L"noise/noise_02.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"noise3", L"noise/noise_03.png");

		GETSINGLE(ResourceMgr)->Load<Texture>(L"default", L"default.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"FolderImage", L"folderImage.png");

		GETSINGLE(ResourceMgr)->Load<Texture>(L"texCursor", L"MainScene/Cursor.png");

		GETSINGLE(ResourceMgr)->Load<Texture>(L"t_a", L"temp/albedo.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"t_n", L"temp/normal.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"t_m", L"temp/metallic.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"t_r", L"temp/roughness.png");

		GETSINGLE(ResourceMgr)->Load<Texture>(L"BRDF", L"Textures/BRDF.png");
	
		GETSINGLE(ResourceMgr)->Load<Texture>(L"loading", L"Loading.png");

		Texture* uavTexture = new Texture();
		uavTexture->Create(1024, 1024,
		DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS);
		GETSINGLE(ResourceMgr)->Insert<Texture>(L"PaintTexture", uavTexture);

		postProcessTexture = new Texture();
		postProcessTexture->Create(1600, 900, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
		postProcessTexture->BindShaderResource(eShaderStage::PS, 60);
		GETSINGLE(ResourceMgr)->Insert<Texture>(L"PostProcessTexture", postProcessTexture);
	}


	void LoadDefaultMaterial()
	{

#pragma region SpriteMaterial
		{
			Texture* texture = GETSINGLE(ResourceMgr)->Find<Texture>(L"default");
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"SpriteShader");
			Material* material = new Material();
			material->SetRenderingMode(eRenderingMode::Transparent);
			material->SetShader(shader);
			material->SetTexture(texture);
			GETSINGLE(ResourceMgr)->Insert<Material>(L"SpriteMaterial", material);
		}
#pragma endregion
#pragma region GridMaterial
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"GridShader");
			Material* material = new Material();
			material->SetRenderingMode(eRenderingMode::Opaque);
			material->SetShader(shader);
			GETSINGLE(ResourceMgr)->Insert<Material>(L"GridMaterial", material);
		}
#pragma endregion

#pragma region ParticleMaterial
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"ParticleShader");
			Material* material = new Material();
			material->SetRenderingMode(eRenderingMode::Transparent);
			material->SetShader(shader);
			GETSINGLE(ResourceMgr)->Insert<Material>(L"ParticleMaterial", material);
		}
#pragma endregion

#pragma region DebugMaterial
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DebugShader");
			Material* material = new Material();
			material->SetRenderingMode(eRenderingMode::Transparent);
			material->SetShader(shader);
			GETSINGLE(ResourceMgr)->Insert<Material>(L"DebugMaterial", material);
		}
#pragma endregion

#pragma region PostProcessMaterial
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PostProcessShader");
			Material* material = new Material();
			material->SetRenderingMode(eRenderingMode::PostProcess);
			material->SetShader(shader);
			material->SetTexture(postProcessTexture);
			GETSINGLE(ResourceMgr)->Insert<Material>(L"PostProcessMaterial", material);
		}
#pragma endregion

#pragma region DebugGeometryMaterial
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DebugGeometryShader");
			Material* material = new Material();
			material->SetRenderingMode(eRenderingMode::Transparent);
			material->SetShader(shader);
			GETSINGLE(ResourceMgr)->Insert<Material>(L"DebugGeometryMaterial", material);
		}
#pragma endregion

#pragma region PhongMaterial
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PhongShader");
			Material* material = new Material();
			material->SetRenderingMode(eRenderingMode::Transparent);
			material->SetShader(shader);
			GETSINGLE(ResourceMgr)->Insert<Material>(L"PhongMaterial", material);
		}
#pragma endregion

#pragma region FlatMaterial
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"FlatShader");
			Material* material = new Material();
			material->SetRenderingMode(eRenderingMode::Transparent);
			material->SetShader(shader);
			GETSINGLE(ResourceMgr)->Insert<Material>(L"FlatMaterial", material);
		}
#pragma endregion

#pragma region PBRMaterial
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PBRShader");
			Material* material = new Material();
			material->SetRenderingMode(eRenderingMode::Transparent);
			material->SetShader(shader);
			material->SetTextureByKey(L"t_a", eTextureSlot::Albedo);
			material->SetTextureByKey(L"t_n", eTextureSlot::Normal);
			material->SetTextureByKey(L"t_m", eTextureSlot::Metallic);
			material->SetTextureByKey(L"t_r", eTextureSlot::Roughness);
			GETSINGLE(ResourceMgr)->Insert<Material>(L"PBRMaterial", material);
		}
#pragma endregion

#pragma region CursorMat
		{
			Material* material = new Material(L"texCursor", L"UIShader");
			GETSINGLE(ResourceMgr)->Insert<Material>(L"CursorMat", material);
		};
#pragma endregion

#pragma region DeferredMaterial
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DeferredShader");
			Material* material = new Material();
			material->SetRenderingMode(eRenderingMode::DeferredOpaque);
			material->SetShader(shader);
			material->SetTextureByKey(L"t_a", eTextureSlot::Albedo);
			material->SetTextureByKey(L"t_n", eTextureSlot::Normal);
			material->SetTextureByKey(L"t_m", eTextureSlot::Metallic);
			material->SetTextureByKey(L"t_r", eTextureSlot::Roughness);

			GETSINGLE(ResourceMgr)->Insert<Material>(L"DeferredMaterial", material);
		}
#pragma endregion

#pragma region LightDirMaterial
		{
			Shader* lightDirShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"LightDirShader");
			Material* lightDirMaterial = new Material();
			lightDirMaterial->SetRenderingMode(eRenderingMode::None);
			lightDirMaterial->SetShader(lightDirShader);

			Texture* lightDirTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"PositionTargetTexture");
			lightDirMaterial->SetTexture(eTextureSlot::PositionTarget, lightDirTex);
			lightDirTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"AlbedoTargetTexture");
			lightDirMaterial->SetTexture(eTextureSlot::AlbedoTarget, lightDirTex);
			lightDirTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"NormalTargetTexture");
			lightDirMaterial->SetTexture(eTextureSlot::NormalTarget, lightDirTex);
			lightDirTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"MRDTargetTexture");
			lightDirMaterial->SetTexture(eTextureSlot::MRDTarget, lightDirTex);
			lightDirTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"ShadowMapTexture");
			lightDirMaterial->SetTexture(eTextureSlot::ShadowMap, lightDirTex);

			GETSINGLE(ResourceMgr)->Insert<Material>(L"LightDirMaterial", lightDirMaterial);
		}
#pragma endregion

#pragma region LightPointMaterial
		{
			Shader* lightPointShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"LightPointShader");
			Material* lightPointMaterial = new Material();
			lightPointMaterial->SetRenderingMode(eRenderingMode::None);
			lightPointMaterial->SetShader(lightPointShader);

			Texture* lightPointTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"PositionTarget");
			lightPointMaterial->SetTexture(eTextureSlot::PositionTarget, lightPointTex);
			lightPointTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"AlbedoTargetTexture");
			lightPointMaterial->SetTexture(eTextureSlot::AlbedoTarget, lightPointTex);
			lightPointTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"NormalTarget");
			lightPointMaterial->SetTexture(eTextureSlot::NormalTarget, lightPointTex);
			lightPointTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"MRDTargetTexture");
			lightPointMaterial->SetTexture(eTextureSlot::MRDTarget, lightPointTex);
			GETSINGLE(ResourceMgr)->Insert<Material>(L"LightPointMaterial", lightPointMaterial);
		}
#pragma endregion

#pragma region MergeMRT_Material
		{
			// RenderTarget Merge 시에 사용할 머테리얼
			Shader* mergeShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"MergeShader");
			Material* mergeMaterial = new Material();
			mergeMaterial->SetRenderingMode(eRenderingMode::None);
			mergeMaterial->SetShader(mergeShader);

			Texture* mergeTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"PositionTargetTexture");
			mergeMaterial->SetTexture(eTextureSlot::PositionTarget, mergeTex);
			mergeTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"AlbedoTargetTexture");
			mergeMaterial->SetTexture(eTextureSlot::AlbedoTarget, mergeTex);
			mergeTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"DiffuseLightTargetTexture");
			mergeMaterial->SetTexture(eTextureSlot::DiffuseLightTarget, mergeTex);
			mergeTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"SpecularLightTargetTexture");
			mergeMaterial->SetTexture(eTextureSlot::SpecularLightTarget, mergeTex);

			GETSINGLE(ResourceMgr)->Insert<Material>(L"MergeMRT_Material", mergeMaterial);
		}
#pragma endregion

#pragma region Depth Material
		{
			Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DepthShader");
			Material* material = new Material();
			material->SetRenderingMode(eRenderingMode::None);
			material->SetShader(shader);
			GETSINGLE(ResourceMgr)->Insert<Material>(L"ShadowMaterial", material);
		}
#pragma endregion
#pragma region LoadingImgMaterial
		Texture* loadingImgTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"loading");
		Shader* uiSpriteShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"UISpriteShader");
		Material* loadingImgMaterial = new Material();
		loadingImgMaterial->SetRenderingMode(eRenderingMode::Transparent);
		loadingImgMaterial->SetShader(uiSpriteShader);
		loadingImgMaterial->SetTexture(eTextureSlot::Albedo, loadingImgTex);
		GETSINGLE(ResourceMgr)->Insert<Material>(L"loadingImgMaterial", loadingImgMaterial);
#pragma endregion

	
	}

	void CreateRenderTargets()
	{
		UINT width = application.GetWidth();
		UINT height = application.GetHeight();
		std::vector<Texture*> vecRTTex = { };

		//SwapChain MultiRenderTargets
		{
			Texture* dsTex = nullptr;

			vecRTTex.emplace_back(GETSINGLE(ResourceMgr)->Find<Texture>(L"RenderTargetTexture"));
			dsTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"DepthStencilBufferTexture");

			renderTargets[static_cast<UINT>(eRenderTargetType::Swapchain)] = new MultiRenderTarget();
			renderTargets[static_cast<UINT>(eRenderTargetType::Swapchain)]->Create(vecRTTex, dsTex);
		}
		vecRTTex.clear();
		// Deferred MultiRenderTargets
		{
			Texture* pos = new Texture();
			Texture* albedo = new Texture();
			Texture* normal = new Texture();
			Texture* mrd = new Texture();

			GETSINGLE(ResourceMgr)->Insert<Texture>(L"PositionTargetTexture", pos);
			GETSINGLE(ResourceMgr)->Insert<Texture>(L"AlbedoTargetTexture", albedo);
			GETSINGLE(ResourceMgr)->Insert<Texture>(L"NormalTargetTexture", normal);
			GETSINGLE(ResourceMgr)->Insert<Texture>(L"MRDTargetTexture", mrd);
			vecRTTex.emplace_back(pos);
			vecRTTex.emplace_back(albedo);
			vecRTTex.emplace_back(normal);
			vecRTTex.emplace_back(mrd);
		
			vecRTTex[0]->Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
			vecRTTex[1]->Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
			vecRTTex[2]->Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
			vecRTTex[3]->Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

			Texture* dsTex = nullptr;
			dsTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"DepthStencilBufferTexture");

			renderTargets[static_cast<UINT>(eRenderTargetType::Deferred)] = new MultiRenderTarget();
			renderTargets[static_cast<UINT>(eRenderTargetType::Deferred)]->Create(vecRTTex, dsTex);
		}
		vecRTTex.clear();
		// Light MultiRenderTargets
		{
			Texture* diffuse = new Texture();
			Texture* specular = new Texture();

			GETSINGLE(ResourceMgr)->Insert<Texture>(L"DiffuseLightTargetTexture", diffuse);
			GETSINGLE(ResourceMgr)->Insert<Texture>(L"SpecularLightTargetTexture", specular);

			vecRTTex.emplace_back(diffuse);
			vecRTTex.emplace_back(specular);

			vecRTTex[0]->Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
			vecRTTex[1]->Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

			renderTargets[static_cast<UINT>(eRenderTargetType::Light)] = new MultiRenderTarget();
			renderTargets[static_cast<UINT>(eRenderTargetType::Light)]->Create(vecRTTex, nullptr);
		}
		vecRTTex.clear();
		// Shadow MRT
		{	
			Texture* shadowMap = new Texture();
			GETSINGLE(ResourceMgr)->Insert<Texture>(L"ShadowMapTexture", shadowMap);
			vecRTTex.emplace_back(shadowMap);
			vecRTTex[0]->Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);

			Texture* depthStencilTex = new Texture();
			depthStencilTex->Create(width, height, DXGI_FORMAT_D32_FLOAT
				, D3D11_BIND_DEPTH_STENCIL);
			GETSINGLE(ResourceMgr)->Insert<Texture>(L"ShadowMapDSTexture", depthStencilTex);

			renderTargets[static_cast<UINT>(eRenderTargetType::Shadow)] = new MultiRenderTarget();
			renderTargets[static_cast<UINT>(eRenderTargetType::Shadow)]->Create(vecRTTex, depthStencilTex);
		}
	}

	void ClearRenderTargets()
	{
		for (size_t i = 0; i < static_cast<UINT>(eRenderTargetType::End); i++)
		{
			if (renderTargets[i] == nullptr)
			{
				continue;
			}

			if (i == 0)
			{
				FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
				renderTargets[i]->Clear(backgroundColor);
			}
			else
			{
				FLOAT backgroundColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
				renderTargets[i]->Clear(backgroundColor);
			}
		}
	}

	void PushLightAttribute(LightAttribute attribute)
	{
		lightAttributes.push_back(attribute);
	}

	void BindLight()
	{
		lightBuffer->SetData(lightAttributes.data(), static_cast<UINT>(lightAttributes.size()));
		lightBuffer->BindSRV(eShaderStage::VS, 22);
		lightBuffer->BindSRV(eShaderStage::PS, 22);

		renderer::LightCB Lightcb = {};
		Lightcb.lightCount = static_cast<UINT>(lightAttributes.size());

		ConstantBuffer* cb = constantBuffers[static_cast<UINT>(eCBType::Light)];
		cb->SetData(&Lightcb);

		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}

	float noiseTime = 10.f;
	float ElapsedTime = 0.f;

	void BindNoiseTexture()
	{
		Texture* noise = GETSINGLE(ResourceMgr)->Find<Texture>(L"noise1");
		noise->BindShaderResource(eShaderStage::VS, static_cast<UINT>(eTextureSlot::NoiseTexture));
		noise->BindShaderResource(eShaderStage::HS, static_cast<UINT>(eTextureSlot::NoiseTexture));
		noise->BindShaderResource(eShaderStage::DS, static_cast<UINT>(eTextureSlot::NoiseTexture));
		noise->BindShaderResource(eShaderStage::GS, static_cast<UINT>(eTextureSlot::NoiseTexture));
		noise->BindShaderResource(eShaderStage::PS, static_cast<UINT>(eTextureSlot::NoiseTexture));
		noise->BindShaderResource(eShaderStage::CS, static_cast<UINT>(eTextureSlot::NoiseTexture));

		NoiseCB info = {};
		info.noiseSize.x = static_cast<float>(noise->GetWidth());
		info.noiseSize.y = static_cast<float>(noise->GetHeight());
		noiseTime -= DT;
		info.noiseTime = noiseTime;

		ConstantBuffer* cb = renderer::constantBuffers[static_cast<UINT>(eCBType::Noise)];
		cb->SetData(&info);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::HS);
		cb->Bind(eShaderStage::DS);
		cb->Bind(eShaderStage::GS);
		cb->Bind(eShaderStage::PS);
		cb->Bind(eShaderStage::CS);
	}

	void CopyRenderTarget()
	{
		Texture* renderTarget = GETSINGLE(ResourceMgr)->Find<Texture>(L"RenderTargetTexture");

		ID3D11ShaderResourceView* srv = nullptr;
		GetDevice()->BindShaderResource(eShaderStage::PS, 60, &srv);

		ID3D11Texture2D* dest = postProcessTexture->GetTexture().Get();
		ID3D11Texture2D* source = renderTarget->GetTexture().Get();

		GetDevice()->CopyResource(dest, source);

		postProcessTexture->BindShaderResource(eShaderStage::PS, 60);
	}

	void BindPBRProprerties()
	{
		Texture* BRDF = GETSINGLE(ResourceMgr)->Find<Texture>(L"BRDF");
		BRDF->BindShaderResource_VP(15);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CreatePointMesh()
	{
		Vertex PointVertex = {};
		PointVertex.pos = Vector4(0.5f, 0.5f, 0.5f, 1.f);
		PointVertex.uv = Vector2(0.f, 0.f);
		Mesh* pointMesh = new Mesh();
		GETSINGLE(ResourceMgr)->Insert<Mesh>(L"Pointmesh", pointMesh);
		pointMesh->CreateVertexBuffer(&PointVertex, 1);
		UINT pointIndex = 0;
		pointMesh->CreateIndexBuffer(&pointIndex, 1);
	}

	void CreateLineMesh()
	{
		Vertex LineVertex[2] = {};

		LineVertex[0].pos = Vector4(-0.5f, 0.25f, 0.f, 1.f);
		LineVertex[0].uv = Vector2(0.f, 0.f);

		LineVertex[1].pos = Vector4(0.5f, 0.25f, 0.f, 1.f);
		LineVertex[1].uv = Vector2(1.f, 0.f);


		Mesh* lineMesh = new Mesh();
		GETSINGLE(ResourceMgr)->Insert<Mesh>(L"Linemesh", lineMesh);
		lineMesh->CreateVertexBuffer(&LineVertex, 2);
		std::vector<UINT> indices;
		indices.emplace_back(0);
		indices.emplace_back(1);
		lineMesh->CreateIndexBuffer(indices.data(), static_cast<UINT>(indices.size()));
	}

	void CreateRectMesh()
	{
		Vertex	RectVertexes[4] = {};

		RectVertexes[0].pos = Vector4(-0.5f, 0.5f, 0.f, 1.f);
		RectVertexes[0].uv = Vector2(0.f, 0.f);

		RectVertexes[1].pos = Vector4(0.5f, 0.5f, 0.f, 1.f);
		RectVertexes[1].uv = Vector2(1.f, 0.f);

		RectVertexes[2].pos = Vector4(0.5f, -0.5f, 0.f, 1.f);
		RectVertexes[2].uv = Vector2(1.f, 1.f);

		RectVertexes[3].pos = Vector4(-0.5f, -0.5f, 0.f, 1.f);
		RectVertexes[3].uv = Vector2(0.f, 1.f);

		Mesh* Rectmesh = new Mesh();
		GETSINGLE(ResourceMgr)->Insert<Mesh>(L"Rectmesh", Rectmesh);
		Rectmesh->CreateVertexBuffer(RectVertexes, 4);

		std::vector<UINT> indices = {};
		indices.emplace_back(0);
		indices.emplace_back(1);
		indices.emplace_back(2);
		indices.emplace_back(0);
		indices.emplace_back(2);
		indices.emplace_back(3);
		indices.emplace_back(0);
		Rectmesh->CreateIndexBuffer(indices.data(), static_cast<UINT>(indices.size()));
	}

	void CreateGridMesh()
	{
		Vertex	GridVertexes[4] = {};

		GridVertexes[0].pos = Vector4(-200.f, 0.f, 200.f, 1.f);
		GridVertexes[0].uv = Vector2(0.f, 0.f);

		GridVertexes[1].pos = Vector4(200.f, 0.f, 200.f, 1.f);
		GridVertexes[1].uv = Vector2(1.f, 0.f);

		GridVertexes[2].pos = Vector4(200.f, 0.f, -200.f, 1.f);
		GridVertexes[2].uv = Vector2(1.f, 1.f);

		GridVertexes[3].pos = Vector4(-200.f, 0.f, -200.f, 1.f);
		GridVertexes[3].uv = Vector2(0.f, 1.f);

		Mesh* Gridmesh = new Mesh();
		GETSINGLE(ResourceMgr)->Insert<Mesh>(L"Gridmesh", Gridmesh);
		Gridmesh->CreateVertexBuffer(GridVertexes, 4);

		std::vector<UINT> indices = {};
		indices.emplace_back(0);
		indices.emplace_back(1);
		indices.emplace_back(2);
		indices.emplace_back(0);
		indices.emplace_back(2);
		indices.emplace_back(3);
		indices.emplace_back(0);
		Gridmesh->CreateIndexBuffer(indices.data(), static_cast<UINT>(indices.size()));
	}

	void CreateCircleMesh()
	{
		std::vector<Vertex>	CircleVertexes;
		Vertex center = {};
		center.pos = Vector4(0.f, 0.f, -0.00001f, 1.f);
		center.uv = Vector2::Zero;

		CircleVertexes.emplace_back(center);

		int slice = 80;
		float radius = 0.5f;
		float theta = XM_2PI / (float)slice;

		for (int i = 0; i < slice; i++)
		{
			Vertex vtx = {};
			vtx.pos = Vector4(radius * cosf(theta * (float)i)
				, radius * sinf(theta * (float)i)
				, -0.00001f, 1.f
			);

			CircleVertexes.emplace_back(vtx);
		}
		std::vector<UINT> indices = {};

		for (int i = 1; i <= slice; i++)
		{
			indices.emplace_back(i);
		}
		indices.emplace_back(1);

		Mesh* Circlemesh = new Mesh();
		GETSINGLE(ResourceMgr)->Insert<Mesh>(L"Circlemesh", Circlemesh);
		Circlemesh->CreateVertexBuffer(CircleVertexes.data(), static_cast<UINT>(CircleVertexes.size()));
		Circlemesh->CreateIndexBuffer(indices.data(), static_cast<UINT>(indices.size()));
	}
	void CreateCubeMesh()
	{
		Vertex arrCube[24] = {};

		// 윗면
		arrCube[0].pos = Vector4(-0.5f, 0.5f, 0.5f, 1.0f);
		arrCube[0].uv = Vector2(0.f, 0.f);
		arrCube[0].tangent = Vector3(1.0f, 0.0f, 0.0f);
		arrCube[0].normal = Vector3(0.f, 1.f, 0.f);

		arrCube[1].pos = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		arrCube[1].uv = Vector2(1.f, 0.f);
		arrCube[1].tangent = Vector3(1.0f, 0.0f, 0.0f);
		arrCube[1].normal = Vector3(0.f, 1.f, 0.f);

		arrCube[2].pos = Vector4(0.5f, 0.5f, -0.5f, 1.0f);
		arrCube[2].uv = Vector2(0.f, 1.f);
		arrCube[2].normal = Vector3(0.f, 1.f, 0.f);
		arrCube[2].tangent = Vector3(1.0f, 0.0f, 0.0f);

		arrCube[3].pos = Vector4(-0.5f, 0.5f, -0.5f, 1.0f);
		arrCube[3].uv = Vector2(1.f, 1.f);
		arrCube[3].normal = Vector3(0.f, 1.f, 0.f);
		arrCube[3].tangent = Vector3(1.0f, 0.0f, 0.0f);


		// 아랫 면	
		arrCube[4].pos = Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[4].uv = Vector2(0.f, 0.f);
		arrCube[4].normal = Vector3(0.f, -1.f, 0.f);
		arrCube[4].tangent = Vector3(-1.0f, 0.0f, 0.0f);

		arrCube[5].pos = Vector4(0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[5].uv = Vector2(1.f, 0.f);
		arrCube[5].normal = Vector3(0.f, -1.f, 0.f);
		arrCube[5].tangent = Vector3(-1.0f, 0.0f, 0.0f);

		arrCube[6].pos = Vector4(0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[6].uv = Vector2(0.f, 1.f);
		arrCube[6].normal = Vector3(0.f, -1.f, 0.f);
		arrCube[6].tangent = Vector3(-1.0f, 0.0f, 0.0f);

		arrCube[7].pos = Vector4(-0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[7].uv = Vector2(1.f, 1.f);
		arrCube[7].normal = Vector3(0.f, -1.f, 0.f);
		arrCube[7].tangent = Vector3(-1.0f, 0.0f, 0.0f);

		// 왼쪽 면
		arrCube[8].pos = Vector4(-0.5f, 0.5f, 0.5f, 1.0f);
		arrCube[8].uv = Vector2(0.f, 0.f);
		arrCube[8].normal = Vector3(-1.f, 0.f, 0.f);
		arrCube[8].tangent = Vector3(0.0f, 1.0f, 0.0f);

		arrCube[9].pos = Vector4(-0.5f, 0.5f, -0.5f, 1.0f);
		arrCube[9].uv = Vector2(1.f, 0.f);
		arrCube[9].normal = Vector3(-1.f, 0.f, 0.f);
		arrCube[9].tangent = Vector3(0.0f, 1.0f, 0.0f);

		arrCube[10].pos = Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[10].uv = Vector2(0.f, 1.f);
		arrCube[10].normal = Vector3(-1.f, 0.f, 0.f);
		arrCube[10].tangent = Vector3(0.0f, 1.0f, 0.0f);

		arrCube[11].pos = Vector4(-0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[11].uv = Vector2(1.f, 1.f);
		arrCube[11].normal = Vector3(-1.f, 0.f, 0.f);
		arrCube[11].tangent = Vector3(0.0f, 1.0f, 0.0f);

		// 오른쪽 면
		arrCube[12].pos = Vector4(0.5f, 0.5f, -0.5f, 1.0f);
		arrCube[12].uv = Vector2(0.f, 0.f);
		arrCube[12].normal = Vector3(1.f, 0.f, 0.f);
		arrCube[12].tangent = Vector3(0.0f, -1.0f, 0.0f);

		arrCube[13].pos = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		arrCube[13].uv = Vector2(1.f, 0.f);
		arrCube[13].normal = Vector3(1.f, 0.f, 0.f);
		arrCube[13].tangent = Vector3(0.0f, -1.0f, 0.0f);

		arrCube[14].pos = Vector4(0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[14].uv = Vector2(0.f, 1.f);
		arrCube[14].normal = Vector3(1.f, 0.f, 0.f);
		arrCube[14].tangent = Vector3(0.0f, -1.0f, 0.0f);

		arrCube[15].pos = Vector4(0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[15].uv = Vector2(1.f, 1.f);
		arrCube[15].normal = Vector3(1.f, 0.f, 0.f);
		arrCube[15].tangent = Vector3(0.0f, -1.0f, 0.0f);

		// 뒷 면
		arrCube[16].pos = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		arrCube[16].uv = Vector2(0.f, 0.f);
		arrCube[16].normal = Vector3(0.f, 0.f, 1.f);
		arrCube[16].tangent = Vector3(1.0f, 0.0f, 0.0f);

		arrCube[17].pos = Vector4(-0.5f, 0.5f, 0.5f, 1.0f);
		arrCube[17].uv = Vector2(1.f, 0.f);
		arrCube[17].normal = Vector3(0.f, 0.f, 1.f);
		arrCube[17].tangent = Vector3(1.0f, 0.0f, 0.0f);

		arrCube[18].pos = Vector4(-0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[18].uv = Vector2(0.f, 1.f);
		arrCube[18].normal = Vector3(0.f, 0.f, 1.f);
		arrCube[18].tangent = Vector3(1.0f, 0.0f, 0.0f);

		arrCube[19].pos = Vector4(0.5f, -0.5f, 0.5f, 1.0f);
		arrCube[19].uv = Vector2(1.f, 1.f);
		arrCube[19].normal = Vector3(0.f, 0.f, 1.f);
		arrCube[19].tangent = Vector3(1.0f, 0.0f, 0.0f);

		// 앞 면
		arrCube[20].pos = Vector4(-0.5f, 0.5f, -0.5f, 1.0f);;
		arrCube[20].uv = Vector2(0.f, 0.f);
		arrCube[20].normal = Vector3(0.f, 0.f, -1.f);
		arrCube[20].tangent = Vector3(1.0f, 0.0f, 0.0f);

		arrCube[21].pos = Vector4(0.5f, 0.5f, -0.5f, 1.0f);
		arrCube[21].uv = Vector2(1.f, 0.f);
		arrCube[21].normal = Vector3(0.f, 0.f, -1.f);
		arrCube[21].tangent = Vector3(1.0f, 0.0f, 0.0f);

		arrCube[22].pos = Vector4(0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[22].uv = Vector2(0.f, 1.f);
		arrCube[22].normal = Vector3(0.f, 0.f, -1.f);
		arrCube[22].tangent = Vector3(1.0f, 0.0f, 0.0f);

		arrCube[23].pos = Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
		arrCube[23].uv = Vector2(1.f, 1.f);
		arrCube[23].normal = Vector3(0.f, 0.f, -1.f);
		arrCube[23].tangent = Vector3(1.0f, 0.0f, 0.0f);

		std::vector<UINT> indices = {};
		for (int i = 0; i < 6; i++)
		{
			indices.emplace_back(i * 4);
			indices.emplace_back(i * 4 + 1);
			indices.emplace_back(i * 4 + 2);

			indices.emplace_back(i * 4);
			indices.emplace_back(i * 4 + 2);
			indices.emplace_back(i * 4 + 3);
		}

		// Crate GUIMesh
		Mesh* cubMesh = new Mesh();
		GETSINGLE(ResourceMgr)->Insert<Mesh>(L"Cubemesh", cubMesh);
		cubMesh->CreateVertexBuffer(arrCube, 24);
		cubMesh->CreateIndexBuffer(indices.data(), static_cast<UINT>(indices.size()));
	}

	void CreateSphereMesh()
	{
		Vertex v = {};
		float fRadius = 0.5f;
		std::vector<Vertex> sphereVtx;

		// Top
		v.pos = Vector4(0.0f, fRadius, 0.0f, 1.0f);
		v.uv = Vector2(0.5f, 0.f);
		v.normal = Vector3(0.0f, 1.f, 0.0f);
		v.normal.Normalize();
		v.tangent = Vector3(1.f, 0.f, 0.f);

		sphereVtx.emplace_back(v);

		// Body
		UINT iStackCount = 40;
		UINT iSliceCount = 40;

		float fStackAngle = XM_PI / iStackCount;
		float fSliceAngle = XM_2PI / iSliceCount;

		float fUVXStep = 1.f / (float)iSliceCount;
		float fUVYStep = 1.f / (float)iStackCount;

		for (UINT i = 1; i < iStackCount; ++i)
		{
			float phi = i * fStackAngle;

			for (UINT j = 0; j <= iSliceCount; ++j)
			{
				float theta = j * fSliceAngle;

				v.pos = Vector4(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
					, fRadius * cosf(i * fStackAngle)
					, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle), 1.0f);
				v.uv = Vector2(fUVXStep * j, fUVYStep * i);
				v.normal = Vector3(v.pos.x, v.pos.y, v.pos.z);
				//v.normal.Normalize();

				v.tangent.x = -fRadius * sinf(phi) * sinf(theta);
				v.tangent.y = 0.f;
				v.tangent.z = fRadius * sinf(phi) * cosf(theta);
				v.tangent.Normalize();

				sphereVtx.emplace_back(v);
			}
		}

		// Bottom
		v.pos = Vector4(0.f, -fRadius, 0.f, 1.0f);
		v.uv = Vector2(0.5f, 1.f);
		v.normal = Vector3(0.0f, -1.f, 0.0f);
		v.normal.Normalize();

		v.tangent = Vector3(1.f, 0.f, 0.f);
		sphereVtx.emplace_back(v);

		std::vector<UINT> indices = {};

		// North
		for (UINT i = 0; i < iSliceCount; ++i)
		{
			indices.emplace_back(0);
			indices.emplace_back(i + 2);
			indices.emplace_back(i + 1);
		}

		// Middle
		for (UINT i = 0; i < iStackCount - 2; ++i)
		{
			for (UINT j = 0; j < iSliceCount; ++j)
			{
				// + 
				// | \
				// +--+
				indices.emplace_back((iSliceCount + 1) * (i)+(j)+1);
				indices.emplace_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
				indices.emplace_back((iSliceCount + 1) * (i + 1) + (j)+1);

				// +--+
				//  \ |
				//    +
				indices.emplace_back((iSliceCount + 1) * (i)+(j)+1);
				indices.emplace_back((iSliceCount + 1) * (i)+(j + 1) + 1);
				indices.emplace_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			}
		}

		// South
		UINT iBottomIdx = static_cast<UINT>(sphereVtx.size()) - 1;

		for (UINT i = 0; i < iSliceCount; ++i)
		{
			indices.emplace_back(iBottomIdx);
			indices.emplace_back(iBottomIdx - (i + 2));
			indices.emplace_back(iBottomIdx - (i + 1));
		}

		Mesh* sphereMesh = new Mesh();
		GETSINGLE(ResourceMgr)->Insert<Mesh>(L"Spheremesh", sphereMesh);
		sphereMesh->CreateVertexBuffer(sphereVtx.data(), static_cast<UINT>(sphereVtx.size()));
		sphereMesh->CreateIndexBuffer(indices.data(), static_cast<UINT>(indices.size()));
	}

	void CreateCapsuleMesh()
	{
		Vertex v = {};
		float fRadius = 0.5f;
		float fHeight = 1.0f; // Total height of the capsule
		float fHalfHeight = fHeight * 0.5f; // Half of the capsule height
		UINT iStackCount = 40;
		UINT iSliceCount = 40;
		std::vector<Vertex> capsuleVtx;
		std::vector<UINT> indices;


		// Top
		v.pos = Vector4(0.0f, fRadius, 0.0f, 1.0f);
		v.uv = Vector2(0.5f, 0.f);
		v.normal = Vector3(0.0f, 1.f, 0.0f);
		v.normal.Normalize();
		v.tangent = Vector3(1.f, 0.f, 0.f);
		capsuleVtx.emplace_back(v);

		// Bottom
		v.pos = Vector4(0.f, -fRadius, 0.f, 1.0f);
		v.uv = Vector2(0.5f, 1.f);
		v.normal = Vector3(0.0f, -1.f, 0.0f);
		v.normal.Normalize();

		v.tangent = Vector3(1.f, 0.f, 0.f);
		capsuleVtx.emplace_back(v);

		// Create the cylindrical middle part of the capsule
		float fStackStep = fHeight / static_cast<float>(iStackCount); // Height step for each stack
		float fUVYStep = 1.0f / static_cast<float>(iStackCount); // UV step for each stack

		for (UINT i = 0; i < iStackCount; ++i)
		{
			float phi = i * fStackStep - XM_PIDIV2; // Start from the bottom hemisphere's pole

			for (UINT j = 0; j <= iSliceCount; ++j)
			{
				float theta = j * XM_2PI / iSliceCount;

				v.pos = Vector4(fRadius * cosf(phi) * cosf(theta),
					fRadius * sinf(phi) + fHalfHeight,
					fRadius * cosf(phi) * sinf(theta), 1.0f);

				v.uv = Vector2(static_cast<float>(j) / static_cast<float>(iSliceCount),
					static_cast<float>(i) / static_cast<float>(iStackCount));

				v.normal = Vector3(fRadius * cosf(phi) * cosf(theta),
					fRadius * sinf(phi),
					fRadius * cosf(phi) * sinf(theta));

				v.tangent.x = -fRadius * sinf(phi) * sinf(theta);
				v.tangent.y = 0.f;
				v.tangent.z = fRadius * sinf(phi) * cosf(theta);
				v.tangent.Normalize();


				capsuleVtx.emplace_back(v);
			}
		}

		// Connect the top hemisphere with the cylindrical middle part
		for (UINT i = 0; i < iSliceCount; ++i)
		{
			indices.emplace_back(0);
			indices.emplace_back(i + 2);
			indices.emplace_back(i + 1);
		}

		// Connect the bottom hemisphere with the cylindrical middle part
		UINT iBottomIdx = static_cast<UINT>(capsuleVtx.size()) - iSliceCount - 2;

		for (UINT i = 0; i < iSliceCount; ++i)
		{
			indices.emplace_back(iBottomIdx);
			indices.emplace_back(iBottomIdx + (i + 1));
			indices.emplace_back(iBottomIdx + (i + 2));
		}

		// Create the cylindrical middle part of the capsule
		for (UINT i = 0; i < iStackCount - 1; ++i)
		{
			for (UINT j = 0; j < iSliceCount; ++j)
			{
				indices.emplace_back((iSliceCount + 1) * i + j + 1);
				indices.emplace_back((iSliceCount + 1) * i + j + 2);
				indices.emplace_back((iSliceCount + 1) * (i + 1) + j + 2);

				indices.emplace_back((iSliceCount + 1) * i + j + 1);
				indices.emplace_back((iSliceCount + 1) * (i + 1) + j + 2);
				indices.emplace_back((iSliceCount + 1) * (i + 1) + j + 1);
			}
		}

		// Create the capsule mesh
		Mesh* capsuleMesh = new Mesh();
		GETSINGLE(ResourceMgr)->Insert<Mesh>(L"Capsulemesh", capsuleMesh);
		capsuleMesh->CreateVertexBuffer(capsuleVtx.data(), static_cast<UINT>(capsuleVtx.size()));
		capsuleMesh->CreateIndexBuffer(indices.data(), static_cast<UINT>(indices.size()));
	}

	void CreateMaterial(const std::wstring& textureKey, const std::wstring& shaderKey, const std::wstring& materialKeyName, eRenderingMode eRenderMode)
	{
		Texture* texture = GETSINGLE(ResourceMgr)->Find<Texture>(textureKey);
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(shaderKey);
		Material* material = new Material();
		material->SetRenderingMode(eRenderMode);
		material->SetShader(shader);
		material->SetTexture(eTextureSlot::Albedo, texture); // albedo Texture
		GETSINGLE(ResourceMgr)->Insert<Material>(materialKeyName, material);
	}

	void CreateUIMaterial()
	{
#pragma region UISprite Material
		Texture* mariotitle = GETSINGLE(ResourceMgr)->Find<Texture>(L"MarioTitle");
		Shader* uiSpriteShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"UISpriteShader");
		Material* uiSpriteMaterial = new Material();
		uiSpriteMaterial->SetRenderingMode(eRenderingMode::Transparent);
		uiSpriteMaterial->SetShader(uiSpriteShader);
		uiSpriteMaterial->SetTexture(eTextureSlot::Albedo, mariotitle); // albedo Texture
		GETSINGLE(ResourceMgr)->Insert<Material>(L"UISpriteMaterial", uiSpriteMaterial);
#pragma endregion
#pragma region LifeHeartMaterial
		Texture* lifeTexture = GETSINGLE(ResourceMgr)->Find<Texture>(L"Lifeheart");
		Material* lifeheartMaterial = new Material();
		lifeheartMaterial->SetRenderingMode(eRenderingMode::Transparent);
		lifeheartMaterial->SetShader(uiSpriteShader);
		lifeheartMaterial->SetTexture(eTextureSlot::Albedo, lifeTexture); // albedo Texture
		GETSINGLE(ResourceMgr)->Insert<Material>(L"LifeheartMaterial", lifeheartMaterial);
#pragma endregion

#pragma region LifeGaugeMaterial
		Texture* gaugeTexture = GETSINGLE(ResourceMgr)->Find<Texture>(L"Gauge_3");
		Material* gaugeMaterial = new Material();
		gaugeMaterial->SetRenderingMode(eRenderingMode::Transparent);
		gaugeMaterial->SetShader(uiSpriteShader);
		gaugeMaterial->SetTexture(eTextureSlot::Albedo, gaugeTexture); // albedo Texture
		GETSINGLE(ResourceMgr)->Insert<Material>(L"LifeGauge_3Material", gaugeMaterial);
#pragma endregion

#pragma region CoinMaterial
		Texture* coinTexture = GETSINGLE(ResourceMgr)->Find<Texture>(L"Coin");
		Material* coinMaterial = new Material();
		coinMaterial->SetRenderingMode(eRenderingMode::Transparent);
		coinMaterial->SetShader(uiSpriteShader);
		coinMaterial->SetTexture(eTextureSlot::Albedo, coinTexture); // albedo Texture
		GETSINGLE(ResourceMgr)->Insert<Material>(L"CoinMaterial", coinMaterial);
#pragma endregion

#pragma region CityCoinMaterial
		Texture* cityCoinTexture = GETSINGLE(ResourceMgr)->Find<Texture>(L"CityCoin");
		Material* cityCoinMaterial = new Material();
		cityCoinMaterial->SetRenderingMode(eRenderingMode::Transparent);
		cityCoinMaterial->SetShader(uiSpriteShader);
		cityCoinMaterial->SetTexture(eTextureSlot::Albedo, cityCoinTexture); // albedo Texture
		GETSINGLE(ResourceMgr)->Insert<Material>(L"CityCoinMaterial", cityCoinMaterial);
#pragma endregion

#pragma region BarMaterial
		Texture* barTexture = GETSINGLE(ResourceMgr)->Find<Texture>(L"Bar");
		Material* barMaterial = new Material();
		barMaterial->SetRenderingMode(eRenderingMode::Transparent);
		barMaterial->SetShader(uiSpriteShader);
		barMaterial->SetTexture(eTextureSlot::Albedo, barTexture); // albedo Texture
		GETSINGLE(ResourceMgr)->Insert<Material>(L"BarMaterial", barMaterial);
#pragma endregion

#pragma region DottedLineMaterial
		Texture* dottedLine = GETSINGLE(ResourceMgr)->Find<Texture>(L"DottedLine");
		Material* dottedLineMaterial = new Material();
		dottedLineMaterial->SetRenderingMode(eRenderingMode::Transparent);
		dottedLineMaterial->SetShader(uiSpriteShader);
		dottedLineMaterial->SetTexture(eTextureSlot::Albedo, dottedLine); // albedo Texture
		GETSINGLE(ResourceMgr)->Insert<Material>(L"DottedLineMaterial", dottedLineMaterial);
#pragma endregion
#pragma region LunaMaterial
		Texture* luna = GETSINGLE(ResourceMgr)->Find<Texture>(L"CityLuna");
		Material* lunaMaterial = new Material();
		lunaMaterial->SetRenderingMode(eRenderingMode::Transparent);
		lunaMaterial->SetShader(uiSpriteShader);
		lunaMaterial->SetTexture(eTextureSlot::Albedo, luna); // albedo Texture
		GETSINGLE(ResourceMgr)->Insert<Material>(L"LunaMaterial", lunaMaterial);
#pragma endregion
#pragma region WorldMaterial
		Texture* worldMapTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"WorldMap");
		Material* worldMapMaterial = new Material();
		worldMapMaterial->SetRenderingMode(eRenderingMode::Transparent);
		worldMapMaterial->SetShader(uiSpriteShader);
		worldMapMaterial->SetTexture(eTextureSlot::Albedo, worldMapTex); // albedo Texture
		GETSINGLE(ResourceMgr)->Insert<Material>(L"WorldMapMaterial", worldMapMaterial);
#pragma endregion
#pragma region FilterMaterial
		Texture* filterTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"RedFilter");
		Material* filterMaterial = new Material();
		filterMaterial->SetRenderingMode(eRenderingMode::Transparent);
		filterMaterial->SetShader(uiSpriteShader);
		filterMaterial->SetTexture(eTextureSlot::Albedo, filterTex); // albedo Texture
		GETSINGLE(ResourceMgr)->Insert<Material>(L"FilterMaterial", filterMaterial);
#pragma endregion

#pragma region TitleMaterial
		Texture* titleTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"MarioTitle");
		Material* titleMaterial = new Material();
		titleMaterial->SetRenderingMode(eRenderingMode::Transparent);
		titleMaterial->SetShader(uiSpriteShader);
		titleMaterial->SetTexture(eTextureSlot::Albedo, titleTex); // albedo Texture
		GETSINGLE(ResourceMgr)->Insert<Material>(L"TitleMaterial", titleMaterial);
#pragma endregion
#pragma region CapMaterial
		Texture* capTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"Cap");
		Material* capMaterial = new Material();
		capMaterial->SetRenderingMode(eRenderingMode::Transparent);
		capMaterial->SetShader(uiSpriteShader);
		capMaterial->SetTexture(eTextureSlot::Albedo, capTex); // albedo Texture
		GETSINGLE(ResourceMgr)->Insert<Material>(L"CapMaterial", capMaterial);
#pragma endregion
#pragma region UIBarMaterial
		Texture* uibarTex = GETSINGLE(ResourceMgr)->Find<Texture>(L"UIBar");
		Material* uibarMaterial = new Material();
		uibarMaterial->SetRenderingMode(eRenderingMode::Transparent);
		uibarMaterial->SetShader(uiSpriteShader);
		uibarMaterial->SetTexture(eTextureSlot::Albedo, uibarTex); // albedo Texture
		GETSINGLE(ResourceMgr)->Insert<Material>(L"UIBarMaterial", uibarMaterial);
#pragma endregion
#pragma region NumberMaterial
		for (size_t i = 0; i < 9; i++)
		{
			const std::wstring numberName = std::to_wstring(i);

			CreateMaterial(numberName, L"UISpriteShader", numberName + L"Material", eRenderingMode::Transparent);

			CreateMaterial(numberName, L"UISpriteShader", L"Coin" + numberName + L"Material", eRenderingMode::Transparent);
			CreateMaterial(numberName, L"UISpriteShader", L"Luna" + numberName + L"Material", eRenderingMode::Transparent);
		}

		CreateMaterial(L"0", L"UISpriteShader", L"CoinTextMaterial_0", eRenderingMode::Transparent);
		CreateMaterial(L"0", L"UISpriteShader", L"CoinTextMaterial_1", eRenderingMode::Transparent);
		CreateMaterial(L"0", L"UISpriteShader", L"CoinTextMaterial_2", eRenderingMode::Transparent);

		CreateMaterial(L"0", L"UISpriteShader", L"CityCoinTextMaterial_0", eRenderingMode::Transparent);
		CreateMaterial(L"0", L"UISpriteShader", L"CityCoinTextMaterial_1", eRenderingMode::Transparent);
		CreateMaterial(L"0", L"UISpriteShader", L"CityCoinTextMaterial_2", eRenderingMode::Transparent);


		for (size_t i = 'a'; i <= 'z'; i++)
		{
			const std::wstring numberName = L"lowercase_" + std::to_wstring(i);

			CreateMaterial(numberName, L"UISpriteShader", numberName + L"Material", eRenderingMode::Transparent);
		}

		for (size_t i = 'A'; i <= 'Z'; i++)
		{
			const std::wstring numberName = L"uppercase_" + std::to_wstring(i);

			CreateMaterial(numberName, L"UISpriteShader", numberName + L"Material", eRenderingMode::Transparent);
		}

		CreateMaterial(L"Exit", L"UISpriteShader", L"ExitMaterial", eRenderingMode::Transparent);
		CreateMaterial(L"Resume", L"UISpriteShader", L"ResumeMaterial", eRenderingMode::Transparent);
		CreateMaterial(L"Resume2P", L"UISpriteShader", L"Resume2PMaterial", eRenderingMode::Transparent);
		CreateMaterial(L"Start", L"UISpriteShader", L"StartMaterial", eRenderingMode::Transparent);

		CreateMaterial(L"3", L"UISpriteShader", L"LifeTextMaterial", eRenderingMode::Transparent);


		CreateMaterial(L"Compass", L"UISpriteShader", L"CompassMaterial", eRenderingMode::Transparent);
		CreateMaterial(L"CompassBar", L"UISpriteShader", L"CompassBarMaterial", eRenderingMode::Transparent);
		CreateMaterial(L"CompassNeedle", L"UISpriteShader", L"CompassNeedleMaterial", eRenderingMode::Transparent);
#pragma endregion
	}

	void CreateUITexture()
	{
		GETSINGLE(ResourceMgr)->Load<Texture>(L"MarioTitle", L"Textures/UI/CmTitleLogo.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"Gauge_1", L"Textures/UI/Life/Gauge_1.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"Gauge_2", L"Textures/UI/Life/Gauge_2.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"Gauge_3", L"Textures/UI/Life/Gauge_3.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"Lifeheart", L"Textures/UI/Life/heart.png");

		GETSINGLE(ResourceMgr)->Load<Texture>(L"Coin", L"Textures/UI/CoinUI/Coin.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"CityCoin", L"Textures/UI/CoinUI/CityCoin.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"Bar", L"Textures/UI/CoinUI/bar.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"DottedLine", L"Textures/UI/Luna/DottedLine.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"CityLuna", L"Textures/UI/Luna/CityLuna.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"WorldMap", L"Textures/UI/WorldMap/CmImageWorld2.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"RedFilter", L"Textures/UI/WorldMap/RedFilter.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"Cap", L"Textures/UI/CapUI/Cap.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"CapRotate", L"Textures/UI/CapUI/CapAnimation.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"CapMove", L"Textures/UI/CapUI/CapAnimation2.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"CapMove2", L"Textures/UI/CapUI/CapAnimation3.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"CapEye", L"Textures/UI/CapUI/CapEyeAnimation.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"UIBar", L"Textures/UI/UIBar.png");

		for (size_t i = 0; i <= 9; i++)
		{
			const std::wstring& key = std::to_wstring(i);
			GETSINGLE(ResourceMgr)->Load<Texture>(key, L"Textures/UI/Number/" + key + L".png");
		}


		for (size_t i = 'a'; i <= 'z'; i++)
		{
			const std::wstring& key = L"lowercase_" + std::to_wstring(i);
			GETSINGLE(ResourceMgr)->Load<Texture>(key, L"Textures/UI/Alphabet/lowercase/" + key + L".png");
		}

		for (size_t i = 'A'; i <= 'Z'; i++)
		{
			const std::wstring& key = L"uppercase_" + std::to_wstring(i);
			GETSINGLE(ResourceMgr)->Load<Texture>(key, L"Textures/UI/Alphabet/uppercase/" + key + L".png");
		}


		GETSINGLE(ResourceMgr)->Load<Texture>(L"Exit", L"Textures/UI/Text/Exit.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"Resume", L"Textures/UI/Text/Resume.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"Resume2P", L"Textures/UI/Text/Resume2P.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"Start", L"Textures/UI/Text/Start.png");

		GETSINGLE(ResourceMgr)->Load<Texture>(L"Compass", L"Textures/UI/Compass/Compass.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"CompassBar", L"Textures/UI/Compass/CompassBar.png");
		GETSINGLE(ResourceMgr)->Load<Texture>(L"CompassNeedle", L"Textures/UI/Compass/CompassNeedle.png");
	}

	////////////////////////////////////////////////////////

	void Initialize()
	{
		CreateRenderTargets();
		LoadMesh();
		LoadShader();
		SetUpState();
		LoadBuffer();
		LoadLoadingSceneTexture();
		LoadDefaultMaterial();
		BindPBRProprerties();
	}

	void release()
	{
		for (size_t i = 0; i < static_cast<UINT>(eCBType::End); i++)
		{
			delete constantBuffers[i];
			constantBuffers[i] = nullptr;
		}
		delete lightBuffer;
		lightBuffer = nullptr;


		for (size_t i = 0; i < static_cast<UINT>(eRenderTargetType::End); i++)
		{
			if (renderTargets[i] == nullptr)
			{
				continue;
			}

			delete renderTargets[i];
			renderTargets[i] = nullptr;
		}

		mainCamera = nullptr;
		UICamera = nullptr;
	}

	void Render()
	{
		//GetDevice()->OMSetRenderTarget();

		BindNoiseTexture();
		BindLight();

		UINT type = static_cast<UINT>(GETSINGLE(SceneMgr)->GetActiveScene()->GetType());

		for (Camera* cam : Cameras[type])
		{
			if (nullptr == cam)
				continue;

			cam->Render();
			//break;
		}
		Cameras[type].clear();
		renderer::lightAttributes.clear();
	}

}