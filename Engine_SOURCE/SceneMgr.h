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
		// 클래스 내부에 있는것 처럼 보이지만 전역함수임 따라서 멤버변수를 알 수가 없음
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
		Scene* mScenes[(static_cast<UINT>(eSceneType::End))];	// 모든 씬 목록
		Scene* mActiveScene;
	};
}

