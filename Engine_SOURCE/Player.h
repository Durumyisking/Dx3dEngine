#pragma once
#include "GameObj.h"

namespace dru
{
	class Player :
		public GameObj
	{

	public:
		Player();
		virtual ~Player();

		virtual void Initialize();
		virtual void update();
		virtual void fixedUpdate();
		virtual void render();
		virtual void fontRender();


	private:

	public:

	};
}