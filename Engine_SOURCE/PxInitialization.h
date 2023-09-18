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

	PxCooking* GetCooking() { return mCooking; }
	void CreateCooking();

	void	CreateFoundation();
	void	CreatePhysics();
	void	CreateVisualDebugger();
	void	ConntectVisualDebugger();

	PxDefaultAllocator& GetAllocator() { return mAllocCallback; }
	PxDefaultErrorCallback& GetErrorCallback() { return mErrorCallback; }

private:
	PxDefaultAllocator		mAllocCallback; // ������ �޸� �Ҵ� �� ���� ���� (���� �ǵ��ʿ����)
	PxDefaultErrorCallback  mErrorCallback; // ���� �� ��� �޽��� ó��
	PxFoundation* mFoundation; // ������ �ý��� �⺻���� �ʱ�ȭ �� ���� ����
	PxPhysics* mPhysics; // �������� ��ü���� ����� ������ �ִ� Ŭ����
	PxPvd* mPvd;
	PxPvdTransport* mTransport;

	PxCooking* mCooking;
};



