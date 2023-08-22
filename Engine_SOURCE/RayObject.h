#pragma once
#include "GameObj.h"

class RayObject :
    public GameObj
{
public:
	RayObject();
	RayObject(Vector3 mousePos, Vector3 camDir);
	virtual ~RayObject();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;
	virtual void FontRender() override;

	void SetRaycast(Vector3 mousePos, Vector3 camDir)
	{
		mDir = camDir;
		GetComponent<Transform>()->SetPosition(mousePos);
		GetComponent<Transform>()->SetRotation(camDir);
	}

	GameObj* GetPickObj() { return mPickObj; }

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override;
	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerExit(GameObj* gameObject) override;

private:
	Vector3 mDir;
	GameObj* mPickObj;
};

