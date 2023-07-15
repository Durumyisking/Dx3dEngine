#include "Layer.h"
#include "Renderer.h"
#include "Transform.h"
#include "GameObj.h"

namespace dru
{
	//static bool CompareGameObjectByZAxis(GameObj* a, GameObj* b)
	//{
	//	Transform* aTr = a->GetComponent<Transform>();
	//	Transform* bTr = b->GetComponent<Transform>();

	//	if (aTr->GetPosition().z <= bTr->GetPosition().z)
	//	{
	//		return true;
	//	}

	//	return false;
	//}


	Layer::Layer()
	{
	}

	Layer::~Layer()
	{
		for (GameObj* Obj : mGameObjs)
		{
			if (nullptr == Obj)
				continue;

			delete Obj;
			Obj = nullptr;
		}

	}

	void Layer::Initialize()
	{
		for (GameObj* Obj : mGameObjs)
		{
			if (nullptr == Obj)
				continue;
			Obj->Initialize();
		}
	}

	void Layer::update()
	{
		for (GameObj* Obj : mGameObjs)
		{
			if (nullptr == Obj)
				continue;

			if(GameObj::eState::Active == Obj->GetState())
				Obj->Update();
		}
	}

	void Layer::fixedUpdate()
	{
		for (GameObj* Obj : mGameObjs)
		{
			if (nullptr == Obj)
				continue;

			if (GameObj::eState::Active == Obj->GetState())
				Obj->FixedUpdate();
		}

		//// sort z axis
		//std::sort(mGameObjs.begin(), mGameObjs.end(), CompareGameObjectByZAxis);

	}

	void Layer::render()
	{
		for (GameObj* Obj : mGameObjs)
		{
			if (nullptr == Obj)
				continue;

			if (GameObj::eState::Active == Obj->GetState())
				Obj->Render();

		}
	}

	void Layer::fontRender()
	{
		for (GameObj* Obj : mGameObjs)
		{
			if (nullptr == Obj)
				continue;

			if (GameObj::eState::Active == Obj->GetState())
				Obj->FontRender();

		}
	}

	void Layer::destroy()
	{
		std::set<GameObj*> DeadObjs;
		for (GameObj* Obj : mGameObjs)
		{
			if (Obj->IsDead())
				DeadObjs.insert(Obj);
		}

		for (GameObjectIter iter = mGameObjs.begin(); iter != mGameObjs.end(); )
		{
			std::set<GameObj*>::iterator deleteIter = DeadObjs.find(*iter);

			if (deleteIter != DeadObjs.end())
			{
				iter = mGameObjs.erase(iter);
			}
			else
			{
				++iter;
			}
		}

		for (GameObj* Obj : DeadObjs)
		{
			delete Obj;
			Obj = nullptr;
		}

	}

	void Layer::DeleteObject()
	{
		for (GameObj* Obj : mGameObjs)
		{
			if (!Obj->IsDontDestroy())
				Obj->Die();
		}
	}

	void Layer::AddGameObject(GameObj* gameObj, eLayerType type)
	{
		if (nullptr == gameObj)	
			return;
		
		gameObj->SetLayerType(type);
		mGameObjs.push_back(gameObj);
 	}

	std::vector<GameObj*> Layer::GetDontDestroyObjects()
	{
		std::vector<GameObj*> donts;

		for (GameObjectIter iter = mGameObjs.begin();  iter != mGameObjs.end(); )
		{
			if (nullptr == (*iter))
				continue;

			if ((*iter)->IsDontDestroy())
			{
				donts.push_back((*iter));
				iter = mGameObjs.erase(iter);
			}
			else
			{
				++iter;
			}
		}

		return donts;
	}

}