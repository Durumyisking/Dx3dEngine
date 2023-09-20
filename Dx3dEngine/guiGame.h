#pragma once
#include "guiWidget.h"

class Texture;
namespace gui
{
	class Game : public Widget
	{
	public:
		Game();
		~Game();


		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		virtual void Render() override;

	private:
		Texture* mTexture;
	};
}
