#pragma once
#include "Entity.h"
#include "Transform.h"
#include "Script.h"



using namespace math;

class Material;
class Mesh;
class Component;

class GameObj : public DruEntity
{
public:
	enum class eState
	{
		Active,
		Paused,
		Dead,
	};


public:
	GameObj();
	virtual ~GameObj();

	virtual void Initialize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();
	virtual void FontRender();

public:
	virtual void OnCollisionEnter(GameObj* gameObject) {};
	virtual void OnTriggerEnter(GameObj* gameObject) {};
	virtual void OnTriggerExit(GameObj* gameObject) {};

	template <typename T>
	T* AddComponent(eComponentType eType)
	{			
		T* comp = new T();

		if (eType != eComponentType::Script)
		{
			mComponents[static_cast<UINT>(eType)] = comp;
			mComponents[static_cast<UINT>(eType)]->SetOwner(this);
		}
		else
		{
			mScripts.push_back(dynamic_cast<Script*>(comp));
			comp->SetOwner(this);
		}

		return comp;
	}

	void AddComponent(Component* component);

	template <typename T>
	T* GetComponent()
	{
		T* component;
		for (auto c : mComponents)
		{
			component = dynamic_cast<T*>(c);

			if (nullptr != component)
				return component;
		}
		return nullptr;
	}

	template <typename T>
	T* GetComponents()
	{
		std::vector<T*> components = {};

		T* component;
		for (auto c : mComponents)
		{
			component = dynamic_cast<T*>(c);

			if (nullptr != component)
				components.push_back(component);
		}
		return components;
	}

	const std::vector<Script*>& GetScripts() { return mScripts; }

	template <typename T>
	T* GetScript()
	{
		T* returnScript;
		for (auto* script : mScripts)
		{
			returnScript = dynamic_cast<T*>(script);

			if (nullptr != returnScript)
				return returnScript;
		}
		return nullptr;
	}

	void Flip();
		
	bool IsRenderingBlock() const { return mbBlockRendering; }
	void RenderingBlockOn() { mbBlockRendering = true; }
	void RenderingBlockOff() { mbBlockRendering = false; }

	bool MoveToTarget_Smooth_bool(GameObj* target, float speed, bool zOn, eDir dir = eDir::END);
	Vector3 MoveToTarget_Smooth_vector3(GameObj* target, float speed, bool zOn, eDir dir = eDir::END);


protected:
	std::vector<Component*> mComponents;


private:
	eLayerType mType;
	eState mState;
	std::vector<Script*> mScripts;
	bool mbDestroy;

	bool mbIsLeft;
	bool mbOnFloor;
	bool mbBlockRendering;

public:
	void SetPos(Vector3 value);
	void SetPosAbs(Vector3 value);

	void SetScale(Vector3 value);
	void SetRotation(Vector3 value);


	Vector3 GetPos();
	Vector3 GetWorldPos();

	// ui만 사용 가능
	Vector3 GetUIWorldPos();

	Vector3 GetScale();
	Vector3 GetRotation();


	void SetMaterial(Material* material);
	void SetMesh(Mesh* mesh);

	bool IsDead()
	{
		if (eState::Dead == mState)
			return true;
		return false;
	}

	void Active()
	{
		if (!this)
		{
			return;
		}
		mState = eState::Active;
	}
	void Pause()
	{
		if (!this)
		{
			return;
		}
		mState = eState::Paused;
	}
	void Die()
	{
		if (!this)
		{
			return;
		}

		mState = eState::Dead;
	}

	eState GetState() const { return mState; }

	bool IsDontDestroy() { return mbDestroy; }
	void DontDestroy() { mbDestroy = true; }

	eLayerType GetLayerType() const { return mType; }
	void SetLayerType(eLayerType type) { mType = type; }

	bool IsLeft() { return mbIsLeft; }
	void SetLeft() { mbIsLeft = true; }

	void SetRight() { mbIsLeft = false; }

	bool IsOnFloor() const { return mbOnFloor; }
	void SetFloorOn() { mbOnFloor = true; }
	void SetFloorOff() { mbOnFloor = false; }
};

