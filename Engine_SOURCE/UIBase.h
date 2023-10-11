#pragma once
//#include "Entity.h"
#include "GameObj.h"

using namespace math;
class UIBase : public GameObj
{
public:
	struct Event
	{
		void operator=(std::function<void()> func)
		{
			mEvent = std::move(func);
		}

		void operator()()
		{
			if (mEvent)
				mEvent();
		}


		std::function<void()> mEvent;
	};


	UIBase(eUIType type);
	virtual ~UIBase();

	void Initialize() override;
	void Update() override;
	void FixedUpdate() override;
	void Render() override;
	void FontRender() override;


	// UI가 활성화되면 불리는 함수
	void Activate();
	// UI가 비활성화되면 불리는 함수
	void InActivate();
	//UI가 사라질때 호출되는 함수
	void UIClear();


	eUIType GetUIType() { return mUIType; }
	bool GetIsFullScreen() { return mUIbFullScreen; }
	bool GetUIEnable() { return mbUIEnable; }
	void SetIsFullScreen(bool enable) { mUIbFullScreen = enable; }
	void SetUIParent(UIBase* parent) { mUIParent = parent; }

	Vector3 GetUIScreenPos() { return mUIScreenPos; }
	Vector3 GetUIPos() { return mUIPos; }
	Vector3 GetUISize() { return mUISize; }
	std::vector<UIBase*> GetChilds() { return mChilds; }
	void SetUIScreenPos(Vector3 pos) { mUIScreenPos = pos; }
	void SetUIPos(Vector3 pos) { mUIPos = pos; }
	void SetUISize(Vector3 size) { mUISize = size; }
	void Addchild(UIBase* uiBase);
	virtual void SetColor(Vector4 color, bool isColor) {};

	void ChangeTexture(const std::wstring& key);

	void SetUIOn(bool isBool) { mbUIOn = isBool; }
	bool GetUIIsOn() { return mbUIOn; }
protected:
	UIBase* mUIParent;
	Vector3 mUIScreenPos;
	Vector3 mUIPos;
	Vector3 mUISize;

	bool mbColor;
	bool mbUIEnable;
	bool mbUIOn;

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
	eUIType mUIType;
	bool mUIbFullScreen;

	//백터가 더 메모리 친화적이기 떄문에 사용
	std::vector<UIBase*> mChilds;
};