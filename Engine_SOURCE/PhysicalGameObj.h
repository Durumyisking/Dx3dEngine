#pragma once
#include "GameObj.h"
#include "MeshRenderer.h"

namespace dru
{
	class PhysicalGameObj
		: public GameObj
	{

	public:
		PhysicalGameObj();
		virtual ~PhysicalGameObj();

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
}

