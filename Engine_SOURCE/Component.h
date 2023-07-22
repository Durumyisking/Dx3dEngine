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

	math::Vector3 GetOwnerPos() const; // component script에서 코드 사용 편하게 하기위함
	math::Vector3 GetOwnerWorldPos() const; 

private:
	const eComponentType mType; // 타입이 바뀌면 안되기 때문
	GameObj* mOwner;

};



