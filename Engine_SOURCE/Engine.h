#pragma once
#include <string>
#include <iostream>
#include <Windows.h>

#include <vector>
#include <queue>
#include <stack>

#include <list>
#include <map>
#include <bitset>
#include <set>

#include <cmath>
#include <algorithm>
#include <numeric>
#include <limits>
#include <memory>
#include <filesystem>
#include <thread>
#include <future>

//#include <PxPhysicsAPI.h>
#include "../External/PhysX/Include/PxPhysicsAPI.h"


#ifdef _DEBUG


#pragma comment(lib, "../External/PhysX/lib/debug/PhysX_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXFoundation_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXCommon_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXExtensions_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/debug/PhysXCharacterKinematic_static_64.lib")
#else


#pragma comment(lib, "../External/PhysX/lib/release/PhysX_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXFoundation_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXCommon_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXExtensions_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "../External/PhysX/lib/release/PhysXCharacterKinematic_static_64.lib")
#endif

using namespace physx;

// ImGui_internal.h
// ImGui.h 보다 먼저 선언 되어야 하므로 미리 선언 해둠
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif


#include "CommonInclude.h"
#include "druEnums.h"
#include "SimpleMath.h"
#include "StringFunctions.h"
#include "Macro.h"