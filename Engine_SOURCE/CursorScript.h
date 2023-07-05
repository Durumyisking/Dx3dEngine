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
		virtual void update() override;
		virtual void fixedUpdate() override;
		virtual void render() override;


	private:
		Transform* mTrans;
		Vector3 mPos;
		Vector3 mRot;

	};


}