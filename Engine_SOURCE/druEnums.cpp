
#include "druEnums.h"

namespace enums
{
	//enum class eComponentType
	//{
	//	None,
	//	GUITransform, // ��ġ ������ �����ϴ� ������Ʈ
	//	Camera,
	//	GUIMesh,
	//	Collider,
	//	//Collider2,
	//	MeshRenerer,
	//	SpriteRenderer,
	//	Animator,
	//	ParticleSystem,
	//	Light,
	//	UI,
	//	//FadeOut,FadeIn
	//	Script,
	//	End,
	//};

	const char* charComponentType[(int)eComponentType::End] =
	{
		"Default",
		"Camera",
		"RigidBody",
		"Movement",
		"GenericAnimator",
		"Transform",
		"Physical",
		//Controller,
		"Collider",
		"Animator",
		"BoneAnimator",
		"MeshRenderer",
		"Renderer",
		"Particle",
		"Light",
		"UI",
		"AudioListener",
		"AudioSource",
		"Script",
	};

	const wchar_t* wcharComponentType[(int)eComponentType::End] =
	{
		L"Default",
		L"Camera",
		L"RigidBody",
		L"Movement",
		L"GenericAnimator",
		L"Transform",
		L"Physical",
		//Controller,
		L"Collider",
		L"Animator",
		L"BoneAnimator",
		L"MeshRenderer",
		L"Renderer",
		L"Particle",
		L"Light",
		L"UI",
		L"AudioListener",
		L"AudioSource",
		L"Script",
	};

	const char* charResourceType[(int)eResourceType::End] =
	{
		"Mesh",
		"Texture",
		"Material",
		"Sound",
		//"/*Font,*/
		"Prefab",
		"MeshData",
		"GraphicShader",
		"ComputeShader",
		"Script",
	};

	const wchar_t* wcharResourceType[(int)eResourceType::End] =
	{
		L"Mesh",
		L"Texture",
		L"Material",
		L"Sound",
		//"/*Font,*/
		L"Prefab",
		L"MeshData",
		L"GraphicShader",
		L"ComputeShader",
		L"Script",
	};

	const char* charLayerType[(int)eLayerType::End] =
	{
		"Default",
		"Camera",
		//"Grid",
		"Objects",
		"ObjectsContainer",
		"NonePhysical",
		"Monster",
		"Boss",
		"Player",
		"Cap",
		"Platforms",
		"Particle",
		"FX",
		//"SkySphere",
		"CubeMap",
		"PostProcess",
		"UI",
		"End"
	};

	const wchar_t* wcharLayerType[(int)eLayerType::End] =
	{
		L"Default",
		L"Camera",
		//L"Grid",
		L"Objects",
		L"ObjectsContainer",
		L"NonePhysical",
		L"Monster",
		L"Boss",
		L"Player",
		L"Cap",
		L"Platforms",
		L"Particle",
		L"FX",
		//L"SkySphere",
		L"CubeMap",
		L"PostProcess",
		L"UI",
		L"End"
	};
}