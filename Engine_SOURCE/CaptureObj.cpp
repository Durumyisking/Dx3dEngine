#include "CaptureObj.h"
#include "InputMgr.h"

CaptureObj::~CaptureObj()
{
}

void CaptureObj::Update()
{
	if (!mbCapture)
		return;

	if (KEY_DOWN(P))
		Divide();

	CaptureEvent();
}

void CaptureObj::Divide()
{
	// �������� ������Ʈ�� �и� �� �� �̺�Ʈ
	// ���� �Լ��̱� ������ ����� Ư���ϰ� �и��� ������Ʈ��
	// �ڽ� ��ü���� ���� ����


}


