#pragma once
#include "def.h"
#include "TimeMgr.h"
#include "Input.h"

#define DT GETSINGLE(TimeMgr)->DeltaTime()

#define KEY_TAP(key) (GETSINGLE(Input)->GetKeyState(eKeyCode::key) == eKeyState::TAP)
#define KEY_DOWN(key) (GETSINGLE(Input)->GetKeyState(eKeyCode::key) == eKeyState::DOWN)
