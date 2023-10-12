#pragma once
#include <wrl.h>
#include "druMath.h"
#include "druEnums.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "../External/DirectXTK/lib/Debug/DirectXTK.lib")
#else
#pragma comment(lib, "../External/DirectXTK/lib/Release/DirectXTK.lib")
#endif

#define CB_GETBINDSLOT(name) __CBUFFERBINDSLOT__##name##__
#define CBUFFER(name, slot) static const int CB_GETBINDSLOT(name) = slot; struct alignas(16) name

#define CBSLOT_TRANSFORM 0
#define CBSLOT_MATERIAL 1
#define CBSLOT_GRID 2
#define CBSLOT_COLOR 3
#define CBSLOT_ANIMATION 4
#define CBSLOT_LIGHTCOUNT 5
#define CBSLOT_PARTICLESYSTEM 6
#define CBSLOT_NOISE			7
#define CBSLOT_POSTPROCESS		8
#define CBSLOT_SKY 9


enum class eValidationMode
{
	Disabled,
	Enabled,
	GPU,
};

enum class eShaderStage
{
	VS,
	HS,
	DS,
	GS,
	PS,
	CS,
	All,
	End,

};

enum class eBufferStage
{
	VB,
	IB,
	CB,

	End,
};

enum class eSamplerType
{
	//Point,
	Linear,
	//Anisotropic,
	Skybox,
	Clamp,
	ShadowPoint,
	ShadowCompare,
	End,
};

enum class eRasterizerType
{
	SolidBack,
	SolidFront,
	SolidNone, // �ø� ����
	WireframeNone, // �����θ� �׸��� Topology�� ������ �׳� gpu���� ó�����ִ°�
	End,
};

enum class eDepthStencilType
{
	UI,
	Less, // �Ϲ����� ����
	Greater, // ���� �Ųٷ�
	NoWrite, // ��ġ�� �ƿ� �ȱ׸�
	None, // ���̹��� ��� ����
	End,
};

enum class eBlendStateType
{
	Default,
	AlphaBlend,
	OneOne, // ���İ� ���� ��ü�� �� ����
	End,
};

enum class eRenderTargetType
{
	Swapchain,
	Deferred,
	Light,
	Shadow,
	End,
};

enum class eRenderingMode
{
	DeferredOpaque, // ������
	DeferredMask,
	Light, // ���� ó��
	Opaque, // ������
	Cutout, // �Ϻθ� ����
	Transparent,
	PostProcess,

	None,
	End,
};

struct GpuBuffer
{
	enum class eType
	{
		Buffer,
		Texture,
		UnknownType,
	} type = eType::UnknownType;

	D3D11_BUFFER_DESC desc;
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;


	GpuBuffer() = default;
	virtual ~GpuBuffer() = default;

};

enum class eTextureSlot
{
	Albedo,				// colorTexture
	Normal,				// normalTexture
	Metallic,			// metalTexture
	Roughness,			// roughnessTexture
	Emissive,			// emissiveTexture

	PositionTarget = 5,			// positionTarget
	AlbedoTarget,				// albedoTarget
	NormalTarget,				// normalTarget
	MRDTarget,					// metallic roughness Depth target
	EmissiveTarget,				// emissivetarget

	DiffuseLightTarget = 10,		// diffuseLightTarget
	SpecularLightTarget,		// specularLightTarget

	Cubemap = 12,
	IrradianceMap,
	PrefilteredMap,		

	BRDF = 15,		

	ParticleStructuredBuffer = 16,		
	NoiseTexture,		

	SkySphere = 18,

	ShadowMap = 19,

	PostProcess = 20,					// atlasTexture


	DepthMap = 21,

	End,
};

enum class eCBType
{
	Transform,
	Material,
	Grid,
	Color,
	Animation,
	Light,
	ParticleSystem,
	Noise, 
	PostProcess,
	CubeMapProj,
	End,
};


enum class eGPUParam
{
	Int_1,
	Int_2,
	Int_3,
	Int_4,
	Float_1,
	Float_2,
	Float_3,
	Float_4,
	Float_5,
	Float_6,
	Metallic,
	Roughness,
	Vector2_1,
	Vector2_2,
	Vector2_3,
	Vector2_4,
	Vector3_1,
	Vector3_2,
	FresnelCoeff,
	CamPosition,
	Vector4_1,
	Vector4_2,
	Vector4_3,
	Vector4_4,
	Matrix_1,
	Matrix_2,
	Matrix_3,
	Matrix_4,
	bAlbedo,
	bNormal,
	bMetallic,
	bRoughness,
	bEmissive,
	Bool_1,
	Bool_2,
	Bool_3,
};


enum class eSRVType
{
	SRV,
	UAV,
	End,
};

struct DebugMesh
{
	enums::eColliderType type;
	enums::eCollisionState state;
	math::Vector3 position;
	math::Vector3 rotation;
	math::Vector3 scale;
	float radius;
	float duration;
	float time;
};

struct LightAttribute
{
	math::Vector4 diffuse;
	math::Vector4 specular;
	math::Vector4 ambient;

//		math::Vector4 emissive;

	math::Vector4 position;
	math::Vector4 direction;

	float radius;
	float fallOffStart = 0.f;
	float fallOffEnd = 0.f;
	float spotPower;

	enums::eLightType type;
	math::Vector3 padding; // ������� �е�

	math::Matrix view;
	math::Matrix projection;
	math::Matrix inverseProjection;
};

struct Particle
{
	math::Vector4 position;
	math::Vector4 direction;
	math::Vector4 startColor;
	math::Vector4 endColor;

	math::Vector3  startScale;
	math::Vector3  endScale;

	float lifeTime;
	float elapsedTime;

	math::Vector4 q_startRotation;
	math::Vector4 q_endRotation;

	float speed;
	float radian;
	float gravityAcc;
	UINT active;

	math::Matrix particleWorld = {};

	UINT texture_x_index;
	UINT texture_y_index;

	float wakeUpTime;
};
struct ParticleShared
{
	UINT activeCount = 1;
};

struct Vertex
{
	math::Vector4 pos;
	math::Vector2 uv;
	math::Vector3 tangent;
	math::Vector3 normal;

	math::Vector4 BlendID;
	math::Vector4 BlendWeight;
};