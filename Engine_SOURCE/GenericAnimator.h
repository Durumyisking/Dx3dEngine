#pragma once
#include "Component.h"
#include <functional>

// 매 틱 혹은 애니메이션이 끝났을때 호출하는 함수 타입
typedef std::function<void(float)> FAnimEvent;

// 애니메이션이 어떻게 변화하는지
enum class eAnimType
{
    Linear,            // 일단 단순하게 선형보간 천천히 균일하게 증가
    Max,
};

typedef struct _AnimatorParam
{
    eAnimType AnimType;

    float StartValue;
    float EndValue;

    float DurationTime;

    FAnimEvent DurationFunc;
    FAnimEvent CompleteFunc;

    _AnimatorParam() : AnimType(eAnimType::Linear), StartValue(0.f), EndValue(1.f), DurationTime(1.f), DurationFunc(nullptr), CompleteFunc(nullptr) {}

}AnimatorParam;

class GenericAnimator : public Component
{
public:
    GenericAnimator();
    ~GenericAnimator();

    // 진행될 파라미터
    bool Start(const AnimatorParam& InParam);

    // 현재 진행중인지 여부
    bool IsRunning();

    // true 이면 CompleateFunc 호출 
    void Stop(bool InCompleteEvent = false);

    virtual void Initialize() override {};
    virtual void Update() override;
    virtual void FixedUpdate() override{};
    virtual void Render() override{};

private:
    void update(float InDeltaTime);
private:
    AnimatorParam Param;

    float StartValue;
    float CurValue;
    float EndValue;

    float AccTime;
    float EndTime;

    bool mbRunning;
};


/*

*      매 틱에서 MyAnimator.Update(InDeltaTime) 호출

      // 사용 전 동작중이라면 중지
*      if(MyAnimator.IsRunning())
         MyAnimator.Stop();

      // param 값 세팅 ( Handle 은 세팅 안해도 됨 )
*      AnimatorParam param;
      param.AnimType = EAnimType::Linear;
      param.StartValue = 0.f;
      param.EndValue = 200.f;
      param.DurationTime = 3.f;
      //param.DurationFunc = [](float InCurValue) {};
      
      ==================================================
      람다 함수 내부에서는 중단점이 걸리지 않기때문에
      복잡한 로직같은경우 클래스 내부 함수를 해당 람다함수에서
      호출하는식으로 구현하는것을 권장합니다.

      ex ) param.DurationFunc = [this](float InCurValue)
            {
                TestFunc(InCurValue);
            };
      ===================================================

      param.DurationFunc = [this](float InCurValue)
      {
         LOG(STRING("진행 : %f", InCurValue));
         Position = Vector2D(InCurValue, InCurValue);
      };
      param.CompleteFunc = [this](float InCurValue)
      {
         LOG(STRING("종료 : %f", InCurValue));
         Position = Vector2D(InCurValue, InCurValue);
      };

      // 멤버변수에 Start 함수를 호출해서 param 등록
*      MyAnimator.Start(param);

*/