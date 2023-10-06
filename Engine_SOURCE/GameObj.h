#pragma once
#include "Entity.h"
#include "Transform.h"
#include "Script.h"



using namespace math;

class Material;
class Mesh;
class Component;

class Transform;
class Camera;
class RigidBody;
class PhysXRigidBody;
class PhysicalMovement;
class Physical;
class PhysXCollider;
class Animator;
class BoneAnimator;
class MeshRenderer;
class SpriteRenderer;
class ParticleSystem;
class Light;


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
	GameObj(const GameObj& Obj);
	virtual ~GameObj();

	virtual GameObj* Clone() const;
	virtual void Save(FILE* File) override;
	virtual void Load(FILE* File) override;

	virtual void Initialize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();
	virtual void PrevRender();
	virtual void FontRender();

public:
	virtual void OnCollisionEnter(GameObj* gameObject) {};
	virtual void OnTriggerEnter(GameObj* gameObject) {};
	virtual void OnTriggerStay(GameObj* gameObject) {};
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

	template <typename T>
	T* AddComponent(eComponentType eType, Material* material, Mesh* mesh)
	{
		T* comp = new T(material, mesh);

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

	bool IsRenderingBlock() const { return mbBlockRendering; }
	void RenderingBlockOn() { mbBlockRendering = true; }
	void RenderingBlockOff() { mbBlockRendering = false; }

	bool MoveToTarget_Smooth_bool(GameObj* target, float speed, bool zOn, eDir dir = eDir::END);
	Vector3 MoveToTarget_Smooth_vector3(GameObj* target, float speed, bool zOn, eDir dir = eDir::END);


	std::vector<Component*> GetComponentsVec() { return mComponents; }

	void ReorganizePosition(AXIS axis, eLayerType layerType); // 충돌체 겹친 크기만큼 밀어냄


	Transform* GetTransform();
	Camera* GetCamera();
	RigidBody* GetRigidBody();
	PhysXRigidBody* GetPhysXRigidBody();
	PhysicalMovement* GetMovement();
	Physical* GetPhysical();
	PhysXCollider* GetPhysXCollider();
	Animator* GetAnimator();
	BoneAnimator* GetBoneAnimator();
	MeshRenderer* GetMeshRenderer();
	SpriteRenderer* GetSpriteRenderer();
	ParticleSystem* GetParticle();
	Light* GetLight();


private:
	static std::map<std::string, GameObj*> mObjectCDO;

public:
	static void AddObjectCDO(const std::string& Name, GameObj* CDO)
	{
		mObjectCDO.insert(std::make_pair(Name, CDO));
	}

	static GameObj* FindObjectCDO(const std::string& Name)
	{
		auto	iter = mObjectCDO.find(Name);

		if (iter == mObjectCDO.end())
			return nullptr;

		return iter->second;
	}

	static void ClearObjectCDO()
	{
		for (auto& iter : mObjectCDO)
		{
			if (iter.second == nullptr)
				continue;

			delete iter.second;
			iter.second = nullptr;
		}

		mObjectCDO.clear();
	}

	static void GetAllCDOType(std::vector<std::string>* objectTypeVec)
	{
		for (const auto& pair : mObjectCDO) 
		{
			objectTypeVec->push_back(pair.first);
		}
	}


	const std::string& GetObjectTypeName()	const { return mObjectTypeName; }

protected:
	std::vector<Component*> mComponents;
	std::string				mObjectTypeName;

private:
	eLayerType mType;
	eState mState;
	std::vector<Script*> mScripts;
	bool mbDestroy;

	bool mbBlockRendering;

public:
	void SetPos(Vector3 value);
	void SetPosAbs(Vector3 value);

	void SetScale(Vector3 value);
	void SetRotation(Vector3 value);


	Vector3 GetPos();
	Vector3 GetWorldPos();

	// ui           
	Vector3 GetUIWorldPos();

	Vector3 GetScale();
	Vector3 GetRotation();


	void SetMaterial(Material* material);
	Material* GetMaterial();

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

	bool IsDestroy() { return mbDestroy; }
	void SetDestroyOff() { mbDestroy = false; }
	void SetDestroyOn() { mbDestroy = true; }

	eLayerType GetLayerType() const { return mType; }
	void SetLayerType(eLayerType type) { mType = type; }

};