#pragma once
#include "Engine.h"

namespace dru
{
	class GameObj;
	class Scene;
	class SceneMgr
	{
		SINGLE(SceneMgr)
	public:
		using LateEventVector = std::vector<GameObj*>;

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
		void LoadScene(eSceneType type);
		void LateEvent(); // ���������� ������ ������Ʈ �߰��ϴ� �Լ�

		void DontDestroyOnLoad(GameObj* gameObj);

		void AddEvent(GameObj* obj) { mLateEvent.emplace_back(obj); }

		template <typename T>
		T* GetScene (eSceneType type)
		{
			return dynamic_cast<T*>(mScenes[(static_cast<UINT>(type))]);
		}


		GETSET(Scene*, mActiveScene, ActiveScene)
	private:
		Scene* mScenes[(static_cast<UINT>(eSceneType::End))];	// ��� �� ���
		Scene* mActiveScene;

		LateEventVector mLateEvent;
	};
}

