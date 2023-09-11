#pragma once
#include "guiWidget.h"
#include "Texture.h"
#include "Engine.h"


template <typename... Args>
class Callback {
public:
	template <typename Lambda>
	Callback(Lambda&& lambda) : mFunction(std::forward<Lambda>(lambda)) {}

	void operator()(Args... args) {
		mFunction(args...);
	}

private:
	std::function<void(Args...)> mFunction;
};

namespace gui
{
	class ButtonWidget :
		public Widget
	{
	public:
		ButtonWidget();
		ButtonWidget(std::string name);
		virtual ~ButtonWidget();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetText(const std::string& text) { mText = text; mbText = true; }

		void SetTexture(Texture* tex, float sizeX = 50.f, float sizeY = 50.f);
		void SetTexture(std::wstring& name, float sizeX = 50.f, float sizeY = 50.f);

	//public:
	//	template <typename T>
	//	void SetClickCallback(T* Obj, void(T::* Func)())
	//	{
	//		mClickCallback = std::bind(Func, Obj);
	//	}

	//private:
	//	std::function<void()>	mClickCallback;
	//	bool					mClick;

	//	bool mbText;
	//	std::string mText;
	//	Texture* mTexture;

	public:
		template <typename T, typename... Args>
		void SetClickCallback(T* obj, void(T::* func)(Args...)) {
			mClickCallback = Callback<Args...>(
				[obj, func](Args... args) {
					(obj->*func)(args...);
				}
			);
		}

	private:
		Callback<void*> mClickCallback;
		bool					mClick;

		bool mbText;
		std::string mText;
		Texture* mTexture;


		//void Test()
		//{
		//	// 캡처 이벤트 구현부
		//	bool able = false;

		//	std::vector<std::function<bool(eKeyCode)>> keyEvent;
		//	keyEvent.resize((static_cast<UINT>(eKeyState::NONE) + 1));

		//	keyEvent[static_cast<UINT>(eKeyState::TAP)] = std::bind(&InputMgr::GetKeyTap, GETSINGLE(InputMgr), std::placeholders::_1);
		//	keyEvent[static_cast<UINT>(eKeyState::DOWN)] = std::bind(&InputMgr::GetKeyDown, GETSINGLE(InputMgr), std::placeholders::_1);
		//	keyEvent[static_cast<UINT>(eKeyState::UP)] = std::bind(&InputMgr::GetKeyUp, GETSINGLE(InputMgr), std::placeholders::_1);
		//	keyEvent[static_cast<UINT>(eKeyState::NONE)] = std::bind(&InputMgr::GetKeyNone, GETSINGLE(InputMgr), std::placeholders::_1);

		//	// 키 입력 이벤트 처리하는 람다식
		//	std::function<void(eKeyState, eKeyCode, eMonsterState)> stateEvent =
		//		[&]
		//	(eKeyState keyState, eKeyCode curPress, eMonsterState nextState) ->void
		//	{
		//		if (able)
		//			return;
		//		if (keyEvent[static_cast<UINT>(keyState)](curPress))
		//		{
		//			SetMonsterState(nextState);
		//			able = true;
		//		}
		//	};

		//	// 이동
		//	stateEvent(eKeyState::DOWN, eKeyCode::UP, eMonsterState::Move);
		//	stateEvent(eKeyState::DOWN, eKeyCode::DOWN, eMonsterState::Move);
		//	stateEvent(eKeyState::DOWN, eKeyCode::LEFT, eMonsterState::Move);
		//	stateEvent(eKeyState::DOWN, eKeyCode::RIGHT, eMonsterState::Move);

		//	// 점프
		//	able = false;
		//	stateEvent(eKeyState::TAP, eKeyCode::SPACE, eMonsterState::Jump);

		//	// 특수
		//	//able = false;
		//	//stateEvent(eKeyState::TAP, eKeyCode::SPACE, eMonsterState::SpecialCast);
		//}
	};
}


