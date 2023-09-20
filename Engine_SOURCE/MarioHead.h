#pragma once
#include "MarioParts.h"
class MarioHead :
    public MarioParts
{
public:
	MarioHead();
	virtual ~MarioHead();

	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;
	virtual void Render() final;
	virtual void FontRender() final;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) final;
	virtual void OnTriggerEnter(GameObj* gameObject) final;
	virtual void OnTriggerExit(GameObj* gameObject) final;
};

