#pragma once
#include "Engine.h"



enum class eKeyCode
{
	//Alphabet
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,

	//Special Key
	ENTER, ESC, LSHIFT, LALT, LCTRL,
	SPACE, LEFT, RIGHT, UP, DOWN,

	// mouse
	LBTN, RBTN, WHEELUP, WHEELDOWN,

	//Num Pad
	NUM_0, NUM_1, NUM_2,
	NUM_3, NUM_4, NUM_5,
	NUM_6, NUM_7, NUM_8,
	NUM_9,

	//Num Line
	N_0, N_1, N_2, N_3, N_4, N_5,
	N_6, N_7, N_8, N_9,

	//Function Key
	F_1, F_2, F_3, F_4, F_5, F_6,
	F_7, F_8, F_9, F_10, F_11, F_12,

	END,
};
enum class eKeyState
{
	TAP,
	DOWN,
	UP,
	NONE,
};

class InputMgr
{
	SINGLE(InputMgr)

public:
	struct Key
	{
		eKeyCode  eType;
		eKeyState eState;
		bool	  bPressed;
	};

	void Initialize();
	void Update();
	void Render(HDC hdc);
	void ComputeMousePos();

	__forceinline eKeyState GetKeyState(eKeyCode keyCode)
	{
		return mKeys[static_cast<UINT>(keyCode)].eState;
	}

	__forceinline math::Vector3 GetMousePosition()
	{
		return mMousePosition;
	}
	__forceinline math::Vector3 GetPrevMousePosition()
	{
		return mPrevMousePosition;
	}
	__forceinline math::Vector3 GetMousePosition_world()
	{
		return mWorldMousePosition;
	}
	__forceinline math::Vector2 GetMousePosition_client()
	{
		return mClientMousePosition;
	}
	__forceinline math::Vector2 GetPrevMousePosition_client()
	{
		return mPrevClientMousePosition;
	}

	__forceinline math::Vector3 GetMouseLTapPosition()
	{
		return mMouseLClickPosition;
	}
	__forceinline math::Vector3 GetMouseRTapPosition()
	{
		return mMouseRClickPosition;
	}
	__forceinline math::Vector3 GetMouseMove()
	{
		return mMouseMove;
	}

	__forceinline bool GetKeyDown(eKeyCode keyCode)
	{
		return mKeys[static_cast<UINT>(keyCode)].eState == eKeyState::DOWN;
	}

	__forceinline bool GetKeyTap(eKeyCode keyCode)
	{
		return mKeys[static_cast<UINT>(keyCode)].eState == eKeyState::TAP;
	}

	__forceinline bool GetKeyUp(eKeyCode keyCode)
	{
		return mKeys[static_cast<UINT>(keyCode)].eState == eKeyState::UP;
	}

	__forceinline bool GetKeyNone(eKeyCode keyCode)
	{
		return mKeys[static_cast<UINT>(keyCode)].eState == eKeyState::NONE;
	}

private:
	std::vector<Key> mKeys;
	math::Vector3 mMousePosition;
	math::Vector3 mPrevMousePosition;
	math::Vector3 mMouseMove;

	math::Vector3 mWorldMousePosition;
	math::Vector3 mMouseLClickPosition;
	math::Vector3 mMouseRClickPosition;

	float mWinWidthCenter;
	float mWinHeightCenter;

	math::Vector2 mClientMousePosition;
	math::Vector2 mPrevClientMousePosition;
};
