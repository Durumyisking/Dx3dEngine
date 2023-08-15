#pragma once
//#include "Entity.h"
#include "GameObj.h"

using namespace math;
class UIBase : public GameObj
{
public:
	UIBase(eUIType type);
	virtual ~UIBase();

	void Initialize() override;
	void Update() override;
	void FixedUpdate() override;
	void Render() override;
	void FontRender() override;


	// UI가 활성화되면 불리는 함수
    void Active();
	// UI가 비활성화되면 불리는 함수
    void InActive();
	//UI가 사라질때 호출되는 함수
	void UIClear();

	eUIType GetUIType() { return mUIType; }
	bool GetIsFullScreen() { return mUIbFullScreen; }
	void SetIsFullScreen(bool enable) { mUIbFullScreen = enable; }
	void SetUIParent(UIBase* parent) { mUIParent = parent; }

private:
	virtual void OnInit() {};
	virtual void OnActive() {};
	virtual void OnInActive() {};
	virtual void OnUpdate() {};
	virtual void OnFixedUpdate() {};
	virtual void OnRender() {};
	virtual void OnFontRender() {};
	virtual void OnClear() {};

private:
	UIBase* mUIParent;
	eUIType mUIType;
	bool mUIbFullScreen;
	bool mbUIEnable;

	//Vector3 mUIScreenPos;
	//Vector3 mUIPos;
	//Vector3 mUISize;
};

