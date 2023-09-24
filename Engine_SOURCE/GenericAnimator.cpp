#include "GenericAnimator.h"
#include "TimeMgr.h"

GenericAnimator::GenericAnimator()
	: Component(eComponentType::GenericAnimator)
	, Param(AnimatorParam())
	, StartValue(0.f)
	, CurValue(0.f)
	, EndValue(0.f)
	, AccTime(0.f)
	, EndTime(0.f)
	, mbRunning(false)
{

}

GenericAnimator::~GenericAnimator()
{

}

bool GenericAnimator::Start(const AnimatorParam& InParam)
{
	// �ִϸ��̼�Ÿ���� ���� �����ȵȰŸ� ����
	if (InParam.AnimType == eAnimType::Max)
		return false;

	// �Լ��� ������ �ȵǾ������� �ߴ�
	if (InParam.DurationFunc == nullptr)
		return false;

	Param = InParam;

	StartValue = InParam.StartValue;
	CurValue = InParam.StartValue;
	EndValue = InParam.EndValue;

	AccTime = 0.f;
	EndTime = InParam.DurationTime;

	mbRunning = true;

	return true;
}

void GenericAnimator::update(float InDeltaTime)
{
	if (!IsRunning())
		return;

	AccTime += InDeltaTime;

	if (AccTime >= EndTime)
	{
		Stop(true);
		return;
	}

	if (Param.AnimType == eAnimType::Linear)
	{
		CurValue = ((StartValue * (EndTime - AccTime)) + (EndValue * AccTime)) / EndTime;
	}

	if (Param.DurationFunc)
		Param.DurationFunc(CurValue);
}

bool GenericAnimator::IsRunning()
{
	return mbRunning;
}

void GenericAnimator::Stop(bool InCompleteEvent)
{
	mbRunning = false;

	if (InCompleteEvent)
	{
		if (Param.CompleteFunc)
		{
			Param.CompleteFunc(EndValue);
			return;
		}
	}

	Param = AnimatorParam();
}

void GenericAnimator::Update()
{
	update(DT);
}

