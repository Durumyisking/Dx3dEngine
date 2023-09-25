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
		Camera,
		RigidBody,
		Movement, 
		GenericAnimator,
		Transform,
		Physical,
		//Controller,
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
		Static, // 정적인 물체 (물리적으로 움직이지 않을 물체)
		Dynamic, // 동적인 물체 (물리엔진에 직접적인 영향을 받는다)
		Kinematic, // 프로그래밍 운동제어 물리 시뮬레이션 영향을 받지 않고 스크립트로 움직인다.
		Character, // 일반적인 액터들과 다른 움직임을 처리하기 위해 사용한다. (컨트롤러를 붙여줘야한다)
		Monster, // 커스텀
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
		None,
		TitleImg,
		MainMenu,
		TitleText,
		Start,
		Start2P,
		Option,
		Exit,
		Bar,
		HP,
		HPText,
		Coin,
		CoinText,
		CityCoin,
		CityCoinText,
		Luna,
		LunaText,
		End,
	};

	enum class currentUI
	{
		None,

		MainMenu,
		Play,


		End,
	};

	enum class AXIS
	{
		X,
		Y,
		Z,
		XY,
		XZ,
		YZ,
		XYZ,
		END,
	};

	enum class eHUDState
	{
		None,
		MoveBlink,
		MoveTowards,
		Rotate,
		Size,
		TitleCapMove,
		Hit,
		End
	};

	extern const char* charComponentType[(int)eComponentType::End];
	extern const wchar_t* wcharComponentType[(int)eComponentType::End];
	extern const char* charResourceType[(int)eResourceType::End];
	extern const wchar_t* wcharResourceType[(int)eResourceType::End];

}
