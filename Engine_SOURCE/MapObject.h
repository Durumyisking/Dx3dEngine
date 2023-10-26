#pragma once
#include "GameObj.h"

class MeshRenderer;
class MapObject : public GameObj
{
public:
	MapObject(std::wstring modelName, Vector3 physicalScale, Vector3 colliderOffectPos);
	MapObject(const MapObject& Obj);
	virtual ~MapObject();


	virtual MapObject* Clone() const;
	virtual void Save(FILE* File) override;
	virtual void Load(FILE* File) override;
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override;
	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerPersist(GameObj* gameObject) override;
	virtual void OnTriggerExit(GameObj* gameObject) override;

private:
	std::wstring mModelName;
	bool mbPhysical;
	Vector3 mPhysicalScale;
	eGeometryType mGeometryType;
	eActorType mActorType;
	Vector3 mColliderOffsetPos;
};