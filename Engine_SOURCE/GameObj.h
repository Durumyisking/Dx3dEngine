#pragma once
#include "Transform.h"


namespace dru
{
	using namespace math;

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
		virtual void update();
		virtual void fixedUpdate();
		virtual void render();
		virtual void fontRender();

		template <typename T>
		T* AddComponent(eComponentType _eType)
		{			
			T* comp = new T();

			if (_eType != eComponentType::Script)
			{
				mComponents[static_cast<UINT>(_eType)] = comp;
				mComponents[static_cast<UINT>(_eType)]->SetOwner(this);
			}
			else
			{
				mScripts.push_back(dynamic_cast<Script*>(comp));
				comp->SetOwner(this);
			}

			return comp;
		}

		void AddComponent(Component* _Component);

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

		const std::vector<class Script*>& GetScripts() { return mScripts; }

		template <typename T>
		T* GetScript()
		{
			T* component;
			for (auto* script : mScripts)
			{
				component = dynamic_cast<T*>(script);

				if (nullptr != component)
					return component;
			}
			return nullptr;
		}
		Vector3 Forward() { return GetComponent<Transform>()->Forward(); }
		Vector3 Right() { return GetComponent<Transform>()->Right(); }
		Vector3 Up() { return GetComponent<Transform>()->Up(); }

		void Flip();

		
		bool IsRenderingBlock() const { return mbBlockRendering; }
		void RenderingBlockOn() { mbBlockRendering = true; }
		void RenderingBlockOff() { mbBlockRendering = false; }

		bool MoveToTarget_Smooth_bool(GameObj* _target, float _speed, bool _zOn, eDir _dir = eDir::END);
		Vector3 MoveToTarget_Smooth_vector3(GameObj* _target, float _speed, bool _zOn, eDir _dir = eDir::END);

		GameObj* GetParent() 
		{
			Transform* tr = GetComponent<Transform>()->GetParent();
			if (nullptr != tr)
			{
				return tr->GetOwner();
			}
			return nullptr; 
		}

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
		void SetPos(Vector3 _Value);
		void SetPosAbs(Vector3 _Value);

		void SetPosZ(float _Z) 
		{
			Vector3 pos = GetPos();
			pos.z = _Z;
			GetComponent<Transform>()->SetPosition(pos);
		};
		void SetScale(Vector3 _Value);
		void SetRotation(Vector3 _Value);


		Vector3 GetPos();
		Vector3 GetWorldPos();

		// ui만 사용 가능
		Vector3 GetUIWorldPos();

		Vector3 GetScale();
		Vector3 GetRotation();


		void SetMaterial(Material* _Material);
		void SetMesh(Mesh* _Mesh);

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
		void SetLayerType(eLayerType _Type) { mType = _Type; }

		bool IsLeft() { return mbIsLeft; }
		void SetLeft() { mbIsLeft = true; }

		void SetRight() { mbIsLeft = false; }

		bool IsOnFloor() const { return mbOnFloor; }
		void SetFloorOn() { mbOnFloor = true; }
		void SetFloorOff() { mbOnFloor = false; }
	};

}
