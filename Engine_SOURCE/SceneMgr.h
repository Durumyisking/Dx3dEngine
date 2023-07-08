#pragma once
#include "Engine.h"
#include "GameObj.h"
namespace dru
{
	class Scene;
	class SceneMgr
	{
		SINGLE(SceneMgr)

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
		void Initialize();
		void update();
		void fixedUpdate();
		void render();
		void fontRender();
		void destory();
		void release();
		void LoadScene(eSceneType _Type);

		void DontDestroyOnLoad(GameObj* _GameObj);

		template <typename T>
		T* GetScene (eSceneType _Type)
		{
			return dynamic_cast<T*>(mScenes[(static_cast<UINT>(_Type))]);
		}


		GETSET(Scene*, mActiveScene, ActiveScene)
	private:
		Scene* mScenes[(static_cast<UINT>(eSceneType::End))];	// ��� �� ���
		Scene* mActiveScene;
	};
}

