#pragma once
#include "Component.h"
#include <functional>

// Invalid ���̶�°Ŵ� �۵����� �ʴ� ���ǹ��� ���¸� ��Ÿ���� ���̵�
const   int InvalidHandleId = 0;
static   int HandleId = 0;

// �� ƽ Ȥ�� �ִϸ��̼��� �������� ȣ���ϴ� �Լ� Ÿ��
typedef std::function<void(float)> FAnimEvent;

// �ִϸ��̼��� ��� ��ȭ�ϴ���
enum class eAnimType {
    Linear,            // �ϴ� �ܼ��ϰ� �������� õõ�� �����ϰ� ����
    Max,
};

typedef struct _AnimatorParam
{
    int Handle;
    eAnimType AnimType;

    float StartValue;
    float EndValue;

    float DurationTime;

    FAnimEvent DurationFunc;
    FAnimEvent CompleteFunc;

    _AnimatorParam() : Handle(InvalidHandleId), AnimType(eAnimType::Linear), StartValue(0.f), EndValue(1.f), DurationTime(1.f), DurationFunc(nullptr), CompleteFunc(nullptr) {}

}AnimatorParam;

class GenericAnimator : public Component
{
public:
    GenericAnimator();
    ~GenericAnimator();

    bool Start(const AnimatorParam& InParam);
    void Update(float InDeltaTime);

    bool IsRunning();
    void Stop(bool InCompleteEvent = false);

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void FixedUpdate() override;
    virtual void Render() override;

private:
    AnimatorParam Param;

    float StartValue;
    float CurValue;
    float EndValue;

    float AccTime;
    float EndTime;
};


/*

*      �� ƽ���� MyAnimator.Update(InDeltaTime) ȣ��

      // ��� �� �������̶�� ����
*      if(MyAnimator.IsRunning())
         MyAnimator.Stop();

      // param �� ���� ( Handle �� ���� ���ص� �� )
*      AnimatorParam param;
      param.AnimType = EAnimType::Linear;
      param.StartValue = 0.f;
      param.EndValue = 200.f;
      param.DurationTime = 3.f;
      //param.DurationFunc = [](float InCurValue) {};
      param.DurationFunc = [this](float InCurValue)
      {
         LOG(STRING("���� : %f", InCurValue));
         Position = Vector2D(InCurValue, InCurValue);
      };
      param.CompleteFunc = [this](float InCurValue)
      {
         LOG(STRING("���� : %f", InCurValue));
         Position = Vector2D(InCurValue, InCurValue);
      };

      // ��������� Start �Լ��� ȣ���ؼ� param ���
*      MyAnimator.Start(param);

*/