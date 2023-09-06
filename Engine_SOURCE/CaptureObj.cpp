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
	// 마리오와 오브젝트가 분리 될 때 이벤트
	// 가상 함수이기 때문에 라바콘 특수하게 분리될 오브젝트는
	// 자식 객체에서 직접 구현


}


