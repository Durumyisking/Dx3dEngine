#pragma once
#include "GameObj.h"

class Texture;
class SkyBox : public GameObj
{
public:
	SkyBox();
	SkyBox(const std::wstring& texName, const std::wstring& path);
	virtual ~SkyBox();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void TextureLoad(const std::wstring& key, const std::wstring& path);

	GETSET(Texture*, mSkyTexture, SkyTexture)
	GETSET(GameObj*, mTraceObj, TraceObj)
private:
	GameObj* mTraceObj;
	Texture* mSkyTexture;
};

