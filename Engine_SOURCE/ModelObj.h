#pragma once
#include "GameObj.h"
#include "MeshRenderer.h"
#include "Model.h"

class ModelObj : public GameObj
{
public:
	ModelObj();
	ModelObj(const ModelObj& Obj);
	virtual ~ModelObj();

	virtual ModelObj* Clone() const;
	virtual void Save(FILE* File) override;
	virtual void Load(FILE* File) override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void SetModel(std::wstring ModelName) { mModelName = ModelName; }
	void SetPhysical(eGeometryType GeomType, math::Vector3 PhysicalScale
		, eActorType ActorType = eActorType::Static)
	{
		mbPhysical = true;
		mGeomType = GeomType;
		mActorType = ActorType;
		mPhysicalScale = PhysicalScale;
	}
	void SetAllState(std::wstring ModelName, bool Physical, eGeometryType GeomType
		, math::Vector3 PhysicalScale, eActorType ActorType = eActorType::Static)
	{
		mModelName = ModelName;
		mbPhysical = Physical;
		mGeomType = GeomType;
		mActorType = ActorType;
		mPhysicalScale = PhysicalScale;
	}

public:
	virtual void OnCollisionEnter(GameObj* gameObject)  override {};
	virtual void OnTriggerEnter(GameObj* gameObject)    override {};
	virtual void OnTriggerPersist(GameObj* gameObject)		override {};
	virtual void OnTriggerExit(GameObj* gameObject)		override {};

private:
	std::wstring mModelName;
	bool mbPhysical;
	eGeometryType mGeomType;
	eActorType mActorType;
	math::Vector3 mPhysicalScale;
};

