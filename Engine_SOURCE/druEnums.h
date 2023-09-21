#pragma once

namespace enums
{

	enum class eLayerType
	{
		None = 0,
		Camera,
		Grid,
		Objects,
		PhysicalObject,
		Monster,
		Boss,
		Player,
		AfterImage,
		Platforms,
		Particle,
		FX,
		SkySphere,
		CubeMap,
		PostProcess,
		UI,
		End = 16,
	};


	enum class eComponentType
	{
		None,
		Transform,
		Camera,
		RigidBody,
		Movement, 
		Physical,
		Collider,
		Animator,
		BoneAnimator,
		MeshRenderer,
		Renderer,
		Particle,
		Light, 
		UI,
		AudioListener,
		AudioSource,
		Script,
		End,

	};

	enum class eResourceType
	{
		Mesh,
		Texture,
		Material,
		Model,
		Sound,
		//Font,
		Prefab,
		MeshData,
		GraphicShader,
		ComputeShader,
		AudioClip,
		Script,
		End,

	};

	enum class eColliderType
	{
		Rect,
		Circle,
		Line,
		Box,
		Sphere,
		Capsule,
		End,

	};

	enum class eCollisionState
	{
		CollisionNot = 0,
		CollisionEnter,
		CollisionStay,
		CollisionExit,
		CollisionOff,
		End,
	};

	enum class eAnimationType
	{
		None,
		SecondDimension,
		ThirdDimension,
		End,

	};

	enum class eLightType
	{
		Directional,
		Point,
		Spot, 
		End,

	};

	enum class eDir
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		END,
	};

	enum class eActorType
	{
		Static, // ������ ��ü (���������� �������� ���� ��ü)
		Dynamic, // ������ ��ü (���������� �������� ������ �޴´�)
		Kinematic, // ���α׷��� ����� ���� �ùķ��̼� ������ ���� �ʰ� ��ũ��Ʈ�� �����δ�.
		Character, // �Ϲ����� ���͵�� �ٸ� �������� ó���ϱ� ���� ����Ѵ�. (��Ʈ�ѷ��� �ٿ�����Ѵ�)
		End,
	};

	enum class eGeometryType
	{
		Box,
		Capsule,
		Sphere,
		Plane,
		ConvexMesh,
		TriangleMesh,
		End,
	};

	enum class ePxRigidType
	{
		Dynamic,
		Static,
		Articulation,
		ParticleFluid,
		End,
	};

	enum class eUIType
	{
		HP,
		Coin,
		Option,
		Crosshair,
		Button,
		Image,
		End,
	};

	enum class AXIS
	{
		X,
		Y,
		Z,
	};

	enum class eHUDState
	{
		None,
		MoveBlink,
		MoveTowards,
		Rotate,
		Size,
		TitleCapMove,
		End
	};

	extern const char* charComponentType[(int)eComponentType::End];
	extern const wchar_t* wcharComponentType[(int)eComponentType::End];
	extern const char* charResourceType[(int)eResourceType::End];
	extern const wchar_t* wcharResourceType[(int)eResourceType::End];

}
