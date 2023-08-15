#pragma once

namespace enums
{

	enum class eLayerType
	{
		None = 0,
		Camera,
		Grid,
		BackGround,
		Objects,
		PhysicalObject,
		Bullet,
		Monster,
		Boss,
		Player,
		AfterImage,
		Platforms,
		Particle,
		FX,
		UI,
		PostProcess,
		End = 16,
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
		Static, // 정적인 물체 (물리적으로 움직이지 않을 물체)
		Dynamic, // 동적인 물체 (물리엔진에 직접적인 영향을 받는다)
		Kinematic, // 프로그래밍 운동제어 물리 시뮬레이션 영향을 받지 않고 스크립트로 움직인다.
		Character, // 일반적인 액터들과 다른 움직임을 처리하기 위해 사용한다. (컨트롤러를 붙여줘야한다)
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

	enum class eUIType
	{
		HP,
		Coin,
		Option,
		Crosshair,

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
