#pragma once
#include "Engine.h"


class PxInitialization
{
public:
	PxInitialization();
	~PxInitialization();

public:
	PxFoundation* GetFoundation() { return mFoundation; }
	PxPhysics* GetPhysics() { return mPhysics; }

	void	CreateFoundation();
	void	CreatePhysics();
	void	CreateVisualDebugger();
	void	ConntectVisualDebugger();

	PxDefaultAllocator& GetAllocator() { return mAllocCallback; }
	PxDefaultErrorCallback& GetErrorCallback() { return mErrorCallback; }

private:
	PxDefaultAllocator		mAllocCallback; // 피직스 메모리 할당 및 해제 관리 (굳이 건들필요없음)
	PxDefaultErrorCallback  mErrorCallback; // 에러 및 경고 메시지 처리
	PxFoundation* mFoundation; // 피직스 시스템 기본적인 초기화 및 종료 관리
	PxPhysics* mPhysics; // 피직스의 전체적인 기능을 가지고 있는 클래스
	PxPvd* mPvd;
	PxPvdTransport* mTransport;

};



