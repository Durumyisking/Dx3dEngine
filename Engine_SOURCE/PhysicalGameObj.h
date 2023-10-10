#pragma once
#include "GameObj.h"
#include "MeshRenderer.h"



class PhysicalGameObj
	: public GameObj
{

public:
	PhysicalGameObj();
	PhysicalGameObj(const PhysicalGameObj& Obj);
	virtual ~PhysicalGameObj();

	virtual PhysicalGameObj* Clone() const;
	virtual void Save(FILE* File) override;
	virtual void Load(FILE* File) override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;
	virtual void FontRender() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override {};
	virtual void OnTriggerEnter(GameObj* gameObject)   override {};
	virtual void OnTriggerExit(GameObj* gameObject)    override {};

private:
	math::Vector3 mPhysicalScale;
};
