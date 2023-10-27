#include "Layer.h"
#include "Renderer.h"
#include "Transform.h"
#include "GameObj.h"

#include "Physical.h"



//static bool CompareGameObjectByZAxis(GameObj* a, GameObj* b)
//{
//	GUITransform* aTr = a->GetComponent<GUITransform>();
//	GUITransform* bTr = b->GetComponent<GUITransform>();

//	if (aTr->GetPosition().z <= bTr->GetPosition().z)
//	{
//		return true;
//	}

//	return false;
//}


Layer::Layer()
	: mGameObjs{}
	, mAddedObjects{}

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
	mAddedObjects.clear();
}

void Layer::update()
{
	for (GameObj* Obj : mGameObjs)
	{
		if (nullptr == Obj)
			continue;

		//std::wstring DebugName = Obj->GetName();
		//eLayerType LayerType = Obj->GetLayerType();
		//std::string ObjectTypeName = Obj->GetObjectTypeName();

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

		//std::wstring DebugName = Obj->GetName();
		//eLayerType LayerType = Obj->GetLayerType();
		//std::string ObjectTypeName = Obj->GetObjectTypeName();

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

	for (GameObj* Obj : mAddedObjects)
	{
		Obj->Initialize();
	}

	if (!mAddedObjects.empty())
	{
		mAddedObjects.clear();
	}
}

void Layer::DeleteObject()
{
	for (GameObj* Obj : mGameObjs)
	{
		if (Obj->IsDestroy())
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

		if (!(*iter)->IsDestroy())
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

void Layer::PushAddedObject(GameObj* gameObj, eLayerType type)
{
	gameObj->SetLayerType(type);
	mAddedObjects.push_back(gameObj);
}

