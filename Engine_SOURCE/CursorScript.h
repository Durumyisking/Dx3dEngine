#pragma once
#include "Script.h"

namespace dru
{
	class CursorScript :
		public Script
	{

	public:
		CursorScript();
		virtual ~CursorScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;


	private:
		Transform* mTransform;
		Vector3 mPos;
		Vector3 mRot;

	};
}