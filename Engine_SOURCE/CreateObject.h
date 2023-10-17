#pragma once
#include "GameObj.h"

class MeshRenderer;
class CreateObject : public GameObj
{
public:
	CreateObject(const std::wstring& modelKey, std::vector<std::wstring> array);
	virtual ~CreateObject();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override;
	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerPersist(GameObj* gameObject) override;
	virtual void OnTriggerExit(GameObj* gameObject) override;

private:
	std::vector<std::wstring> MaterialArray;
};