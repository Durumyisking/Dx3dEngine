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
	// 라바콘 처럼 특수하게 분리될 오브젝트는
	// 해당객체내부에서 함수 재정의


}


