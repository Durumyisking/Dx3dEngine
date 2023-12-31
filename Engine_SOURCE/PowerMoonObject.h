#pragma once
#include "InstantiativeObject.h"

class Panal;
class InstancingContainer;
class PowerMoonObject : public InstantiativeObject
{
	enum class ePowerMoonState
	{
		None,
		Appear,
		Down,
		Wait,
	};
public:
	PowerMoonObject();
	PowerMoonObject(const PowerMoonObject& Obj);
	virtual ~PowerMoonObject();

	virtual PowerMoonObject* Clone() const;
	virtual void Save(FILE* File) override;
	virtual void Load(FILE* File) override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;
	virtual void PrevRender() override;
	virtual void FontRender() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override;
	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerPersist(GameObj* gameObject) override;
	virtual void OnTriggerExit(GameObj* gameObject) override;

	void IsHide(bool isbool) { mbHide = isbool; }

private:
	bool mbHide;
};