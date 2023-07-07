#pragma once

#define DT TimeMgr::DeltaTime()

#define KEY_TAP(key) (Input::GetKeyState(eKeyCode::key) == eKeyState::TAP)
#define KEY_DOWN(key) (Input::GetKeyState(eKeyCode::key) == eKeyState::DOWN)

#define SINGLE(type)						\
public:										\
	static type* GetInstance()				\
	{										\
		static type* mInstance;				\
		if (mInstance == nullptr)			\
		{									\
			mInstance = new type();			\
			return mInstance;				\
		}									\
		return mInstance;					\
	}										\
											\
	static void DestroyInstance()			\
	{										\
	if (GetInstance())						\
		delete GetInstance();				\
	}										\
											\
private:									\
	type();									\
	~type();								\

#define GETSINGLE(type)						\
type::GetInstance()							\

// 반환형, 타겟, 생성되는 메소드명
#define GETSET(__TYPE__, __TARGET__, __METHODNAME__) \
__TYPE__ Get##__METHODNAME__()	const				 \
{													 \
	return __TARGET__;								 \
}													 \
													 \
void Set##__METHODNAME__(__TYPE__ value)			 \
{													 \
	__TARGET__ = value;								 \
}	