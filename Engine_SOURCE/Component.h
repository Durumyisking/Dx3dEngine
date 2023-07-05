#pragma once
#include "Entity.h"
#include "druMath.h"


namespace dru
{
	using namespace enums;

	class GameObj;
	class Component : public Entity
	{
	public:

	public:
		Component(eComponentType _Type);
		virtual ~Component();

		virtual void Initialize()	= 0;
		virtual void update()		= 0;
		virtual void fixedUpdate()	= 0;
		virtual void render()		= 0;

		eComponentType GetOrder() { return mType; }

		void SetOwner(GameObj* _Owner) { mOwner = _Owner; }
		GameObj* GetOwner() { return mOwner; }

		template <typename T>
		T* GetOwnerType()
		{
			GameObj* owner = GetOwner();
			return dynamic_cast<T*>(owner);

		}

		math::Vector3 GetOwnerPos() const; // component script���� �ڵ� ��� ���ϰ� �ϱ�����
		math::Vector3 GetOwnerWorldPos() const; 

	private:
		const eComponentType mType; // Ÿ���� �ٲ�� �ȵǱ� ����
		GameObj* mOwner;

	};

}

