#pragma once
#include "Entity.h"


using namespace enums;

class GameObj;
class Component : public DruEntity
{
public:
	Component(eComponentType type);
	virtual ~Component();

	virtual void Initialize()	= 0;
	virtual void Update()		= 0;
	virtual void FixedUpdate()	= 0;
	virtual void Render()		= 0;

	eComponentType GetOrder() { return mType; }

	void SetOwner(GameObj* owner) { mOwner = owner; }
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



