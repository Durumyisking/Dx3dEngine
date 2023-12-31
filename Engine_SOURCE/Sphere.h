#pragma once
#include "PhysicalGameObj.h"



class Sphere :
	public PhysicalGameObj
{

public:
	Sphere();
	Sphere(const Sphere& Obj);
	virtual ~Sphere();

	virtual Sphere* Clone() const;
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
};
