#pragma once
#include "GameObj.h"

class MeshRenderer;
class CreateObject : public GameObj
{
public:
	CreateObject(const std::wstring& modelKey, const std::wstring& materialKey, Vector3 size);
	CreateObject(const CreateObject& Obj);
	virtual ~CreateObject();

	virtual CreateObject* Clone() const;
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override;
	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerPersist(GameObj* gameObject) override;
	virtual void OnTriggerExit(GameObj* gameObject) override;


private:
	std::wstring mModelKey;
	std::wstring mMaterialKey;
	Vector3 mColliderSize;

};