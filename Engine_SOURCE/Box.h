#pragma once
#include "PhysicalGameObj.h"



class Box :
    public PhysicalGameObj
{
public:
	Box();
	Box(const Box& Obj);
	virtual ~Box();

	virtual Box* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

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
