#pragma once
#include "Engine.h"
#include "GameObj.h"
namespace dru
{
	class Scene;
	class SceneMgr
	{
	public:
		enum class eSceneType
		{
			Title,
			//Main,
			//Ending,
			//Temp,
			//Loading,
			End,
		};


	public:
		// Ŭ���� ���ο� �ִ°� ó�� �������� �����Լ��� ���� ��������� �� ���� ����
		static void Initialize();
		static void update();
		static void fixedUpdate();
		static void render();
		static void fontRender();
		static void destory();
		static void release();
		static void LoadScene(eSceneType _Type);

		static void DontDestroyOnLoad(GameObj* _GameObj);

		template <typename T>
		static T* GetScene (eSceneType _Type)
		{
			return dynamic_cast<T*>(mScenes[(static_cast<UINT>(_Type))]);
		}

	public:
		static Scene* mScenes[(static_cast<UINT>(eSceneType::End))];	// ��� �� ���
		static Scene* mActiveScene;

	};
}

