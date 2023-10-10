
#include "druEnums.h"

namespace enums
{
	//enum class eComponentType
	//{
	//	None,
	//	GUITransform, // 위치 데이터 수정하는 컴포넌트
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
		"None",
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
		L"None",
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
}