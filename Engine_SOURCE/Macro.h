#pragma once
#include "def.h"
#define DT GETSINGLE(TimeMgr)->DeltaTime()
#define KEY_TAP(key) (GETSINGLE(InputMgr)->GetKeyState(eKeyCode::key) == eKeyState::TAP)
#define KEY_DOWN(key) (GETSINGLE(InputMgr)->GetKeyState(eKeyCode::key) == eKeyState::DOWN)
