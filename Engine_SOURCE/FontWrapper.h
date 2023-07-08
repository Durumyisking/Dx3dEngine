#pragma once
#include "Engine.h"

#include "..\\External\\FW1FontWrapper\\Include\\FW1FontWrapper.h"
#include "..\\External\\FW1FontWrapper\\Include\\FW1CompileSettings.h"
//#include "..\\External\\Include\\Fmod\\common_platform.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\FW1FontWrapper\\lib\\Debug\\FW1FontWrapperL.lib")
#else
#pragma comment(lib, "..\\External\\FW1FontWrapper\\lib\\Release\\FW1FontWrapper.lib")
#endif

//#include <SpriteFont.h>
//#include <SpriteBatch.h>


namespace dru
{
#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)
	class FontWrapper
	{
		SINGLE(FontWrapper)

	public:
		bool Initialize();
		void DrawFont(const wchar_t* str, float x, float y, float size, UINT rgb);
		void DrawFont(const wchar_t* str, dru::math::Vector3 pos, float size, UINT rgb);
		void Release();


	private:
		IFW1Factory* mFW1Factory;
		IFW1FontWrapper* mFontWrapper;

	};
}
