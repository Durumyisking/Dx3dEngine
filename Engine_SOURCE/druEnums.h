#pragma once

namespace dru::enums
{

	enum class eLayerType
	{
		None = 0,
		Camera,
		Grid,
		BackGround,
		Objects,
		Bullet,
		Monster,
		MonsterGun,
		Boss,
		Player,
		AfterImage,
		Platforms,
		Ray,
		Particle,
		FX,
		UI,
		PostProcess,
		MainMenu,
		End = 18,
	};


	enum class eComponentType
	{
		None,
		Transform,
		Camera,
		Mesh,
		RigidBody,
		Movement, 
		Physical,
		Collider,
		MeshRenderer,
		Renderer,
		Animator,
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

	enum class AXIS
	{
		X,
		Y,
		Z,
	};


	extern const char* charComponentType[(int)eComponentType::End];
	extern const wchar_t* wcharComponentType[(int)eComponentType::End];
	extern const char* charResourceType[(int)eResourceType::End];
	extern const wchar_t* wcharResourceType[(int)eResourceType::End];

}
