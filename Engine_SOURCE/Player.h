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

		virtual void Initialize() override;
		virtual void update() override;
		virtual void fixedUpdate() override;
		virtual void render() override;
		virtual void fontRender() override;


	private:

	public:

	};
}