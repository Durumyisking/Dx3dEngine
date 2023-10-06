#pragma once
#include "Engine.h"



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
		None = -1,
		Title,
		Play,
		Test,
		//Ending,
		//Temp,
		//Loading,
		End,
	};


public:
	void Initialize();
	void Update();
	void FixedUpdate();
	void Render();
	void FontRender();
	void Destory();
	void Release();
	void LoadScene(eSceneType type);
	void LateEvent(); // ���������� ������ ������Ʈ �߰��ϴ� �Լ�

	bool SaveSceneFile(eSceneType type, const std::wstring& filePath);
	bool LoadSceneFile(const std::wstring& filePath);

	void DontDestroyOnLoad(GameObj* gameObj);

	void CreateCDO();

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
