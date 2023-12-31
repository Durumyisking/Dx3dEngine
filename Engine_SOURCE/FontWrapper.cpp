#include "FontWrapper.h"
#include "GraphicDevice.h"
#include "Renderer.h"
#include "SimpleMath.h"



extern Microsoft::WRL::ComPtr<ID3D11SamplerState> renderer::samplerState[];

FontWrapper::FontWrapper()
	: mFW1Factory(nullptr)
	, mFontWrapper(nullptr)
{

}

FontWrapper::~FontWrapper()
{

}

bool FontWrapper::Initialize()
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &mFW1Factory)))
		return false;

	ID3D11Device* pDevice = GetDevice()->GetID3D11Device();
	if (FAILED(mFW1Factory->CreateFontWrapper(pDevice, L"Arial", &mFontWrapper)))
		return false;

	return true;
}

void FontWrapper::DrawFont(const wchar_t* str, float x, float y, float size, UINT rgb)
{
	//RGB();
	ID3D11DeviceContext* pContext = GetDevice()->GetDeviceContext().Get();
	mFontWrapper->DrawString(
		pContext,
		str, // String
		size,// Font size
		x,// X position
		y,// Y position
		rgb,// Text color, 0xAaBbGgRr
		0      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);

	GetDevice()->BindSamplers(static_cast<UINT>(eSamplerType::Linear), 1, renderer::samplerState[static_cast<UINT>(eSamplerType::Linear)].GetAddressOf());
}

void FontWrapper::DrawFont(const wchar_t* str, math::Vector3 pos, float size, UINT rgb)
{
	POINT pt = WorldToWindowPos(pos);

	//RGB();
	ID3D11DeviceContext* pContext = GetDevice()->GetDeviceContext().Get();
	mFontWrapper->DrawString(
		pContext,
		str, 
		size,
		static_cast<FLOAT>(pt.x),
		static_cast<FLOAT>(pt.y),
		rgb,
		0 
	);

	GetDevice()->BindSamplers(static_cast<UINT>(eSamplerType::Linear), 1, renderer::samplerState[static_cast<UINT>(eSamplerType::Linear)].GetAddressOf());
}

void FontWrapper::Release()
{
	mFW1Factory->Release();
	mFW1Factory = nullptr;

	mFontWrapper->Release();
	mFontWrapper = nullptr;
}
