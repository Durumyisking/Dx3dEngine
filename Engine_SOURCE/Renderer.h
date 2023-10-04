#pragma once
#include "CommonInclude.h"
#include "druMath.h"
#include "GraphicDevice.h"

#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "Camera.h"
#include "Light.h"
#include "StructedBuffer.h"
#include "MultiRenderTarget.h"

using namespace math;


#define RED		Vector4{1.f, 0.f, 0.f, 1.f}
#define GREEN	Vector4{0.f, 1.f, 0.f, 1.f}
#define BLUE	Vector4{0.f, 0.f, 1.f, 1.f}
#define GOLD	Vector4{0.75f, 0.55f, 0.15f, 1.f}
#define MAGENTA Vector4{1.f, 0.f, 1.f, 1.f}
#define PEARL	Vector4{0.f, 1.f, 1.f, 1.f}
#define ORANGE	Vector4{0.9f, 0.72f, 0.31f, 1.f}
#define YELLOW	Vector4{0.9569f, 0.6672f, 0.4588f, 1.f}
#define ORANGE_YELLOW	Vector4{0.9569f, 0.6672f, 0.4588f, 1.f}

#define LIGHT_RED		Vector4{1.f, 0.f, 0.f, 0.5f}
#define LIGHT_GREEN		Vector4{0.f, 1.f, 0.f, 0.5f}
#define LIGHT_BLUE		Vector4{0.f, 0.f, 1.f, 0.5f}
#define LIGHT_MAGENTA	Vector4{1.f, 0.f, 1.f, 0.5f}
#define LIGHT_PEARL		Vector4{0.f, 1.f, 1.f, 0.5f}
#define LIGHT_ORANGE	Vector4{0.9f, 0.72f, 0.31f, 0.5f}
#define LIGHT_YELLOW	Vector4{0.9569f, 0.6672f, 0.4588f, 0.5f}
#define LIGHT_ORANGE_YELLOW	Vector4{0.9569f, 0.6672f, 0.4588f, 1.f}


namespace renderer
{


CBUFFER(TransformCB, CBSLOT_TRANSFORM) // ����ü ����°���
	{
		Matrix world;
		Matrix inverseWorld;
		Matrix worldIT; // world inverse and transpose
		Matrix view;
		Matrix inverseView;
		Matrix projection;
		Matrix fovForSkySphere;
		Vector4 cameraWorldPos;
	};

	CBUFFER(MaterialCB, CBSLOT_MATERIAL)
	{
		int iData1;
		int iData2;
		int iData3;
		int iData4;

		float fData1;
		float fData2;
		float fData3;
		float fData4;
		float fData5;
		float fData6;
		float metallic;
		float roughness;

		Vector2 xy1;
		Vector2 xy2;
		Vector2 xy3;
		Vector2 xy4;

		Vector3 xyz1;
		float	xyzPadding1;
		Vector3 xyz2;
		float	xyzPadding2;
		Vector3 FresnelCoeff;
		float	xyzPadding3;
		Vector3 CamPosition;
		float	xyzPadding4;

		Vector4 xyzw1;
		Vector4 xyzw2;
		Vector4 xyzw3;
		Vector4 xyzw4;

		Matrix matrix1;
		Matrix matrix2;
		Matrix matrix3;
		Matrix matrix4;

		int bAlbedo;
		int bNormal;
		int bMetallic;
		int bRoughness;

		int bEmissive;
		int bool1;
		int bool2;
		int bool3;
	};

	CBUFFER(GridCB, CBSLOT_GRID)
	{
		Vector3 cameraPosition;
		Vector2 GridOffset;
		Vector2 Resolution;
		float Thickness;
	};

	CBUFFER(ColorCB, CBSLOT_COLOR)
	{
		Vector4 RGBA;
	};

	CBUFFER(AnimationCB, CBSLOT_ANIMATION)
	{
		Vector2 LT;
		Vector2 size;
		Vector2 offset;
		Vector2 atlasSize;

		UINT type;
	};

	CBUFFER(LightCB, CBSLOT_LIGHTCOUNT)
	{
		UINT lightCount;
		UINT lightIndex;
	};

	CBUFFER(ParticleSystemCB, CBSLOT_LIGHTCOUNT)
	{
		Vector4 worldPosition;
		Vector4 startSize;
		Vector4 startColor;
		Vector4 endColor;

		UINT maxParticles;
		UINT simulationSpace;
		float radius;
		float deltaTime;

		float startSpeed;
		float endSpeed;
		float maxLifeTime;
		float minLifeTime;

		float startAngle;
		float endAngle;
		float elapsedTime;
		float gravity;

		float force;
		float radian;
		Vector2 padding;

		math::Matrix particleWorld;
	};
	CBUFFER(NoiseCB, CBSLOT_NOISE)
	{
		Vector4 noiseSize;
		float noiseTime;
	};
	CBUFFER(PostProcessCB, CBSLOT_POSTPROCESS)
	{
		Vector2 Resolution;

		float ElapsedTime;

		float wave_amount;
		float wave_speed;
		float wave_distortion;
	};

	CBUFFER(SkyCB, CBSLOT_SKY)
	{
		Matrix matrix;
	};

	CBUFFER(LightMatrixCB, CBSLOT_LIGHTMATRIX)
	{
		Matrix lightView;
		Matrix lightProjection;
	};


	// vertex data
	extern Vertex	RectVertexes[4];

	extern ConstantBuffer* constantBuffers[];
	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState[];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState[];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState[];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState> blendState[];

	extern std::vector<Camera*> Cameras[];

	extern Camera* mainCamera;
	extern Camera* UICamera;
	extern std::vector<DebugMesh> debugMeshes;
	extern std::vector<Light*> lights;
	extern std::vector<LightAttribute> lightAttributes;

	extern StructedBuffer* lightBuffer;

	extern GameObj* outlineGameObject;

	extern MultiRenderTarget* renderTargets[]; //MultiRenderTargets
	extern std::vector<std::function<void()>> ParticleFunCArr;

	void Initialize();
	void release(); // �׸��� ����� �������϶� �������� �Ҵ��ϴ°� �ƴ϶�
					// �׸��� ����� �����Ҷ� �Ҵ�� ���� �׸��� ����� ��ü���� ��ü�� ���ش� -> �����ɸ�
					// ���� gpu�� vram�� �뷮�� ram���� ���� �۾Ƽ��׷�

	void Render();

	// MultiRenderTargets
	void CreateRenderTargets();
	void ClearRenderTargets();

	// Renderer
	void PushLightAttribute(LightAttribute attribute);
	void BindLight();
	void BindNoiseTexture();
	void CopyRenderTarget();

	void BindPBRProprerties();

	// mesh create
	void CreatePointMesh();
	void CreateLineMesh();
	void CreateRectMesh();
	void CreateGridMesh();
	void CreateCircleMesh();
	void CreateCubeMesh();
	void CreateSphereMesh();
	void CreateCapsuleMesh();

	void CreateMaterial(const std::wstring& textureKey, const std::wstring& shaderKey, const std::wstring& keyName, eRenderingMode eRenderMode = eRenderingMode::Transparent);
	void CreateUIMaterial();
	void CreateUITexture();
}

