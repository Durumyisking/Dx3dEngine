#include "Button.h"
#include "InputMgr.h"

Button::Button()
	:UIBase(eUIType::None)
	,mbMouseOn(false)
{
	mOnClick = std::bind(&Button::Click, this);
}

Button::Button(Vector3 pos)
	:UIBase(eUIType::None)
	, mbMouseOn(false)
{
	mUIScreenPos = pos;
	mUISize = this->GetComponent<Transform>()->GetScale();
}

Button::Button(eUIType type)
	:UIBase(type)
	, mbMouseOn(false)
{

}

Button::~Button()
{

}

void Button::OnInit()
{

}

void Button::OnActive()
{

}

void Button::OnInActive()
{

}


void Button::OnUpdate()
{
	Vector3 pos = GETSINGLE(InputMgr)->GetMousePosition();
	Vector3 size = GetUISize();


	//glm::vec2 screenCoord(pos.x, pos.y);
	//glm::mat4 viewProjectionMatrix(1.0f);

	//int screenWidth = 1600;
	//int screenHeight = 900;

	//glm::vec2 cameraCoorcd = ScreenToCamera(screenCoord, viewProjectionMatrix, screenWidth, screenHeight);

	//Vector3 mousePos = Vector3(cameraCoorcd.x, cameraCoorcd.y, 0.0f);

	//if (mScreenPos.x <= mousePos.x && mousePos.x < mScreenPos.x + size.x
	//	&& mScreenPos.y <= mousePos.y && mousePos.y < mScreenPos.y + size.y)
	//{
	//	mbMouseOn = true;
	//}
	//else
	//{
	//	mbMouseOn = false;
	//}

	//if (GETSINGLE(InputMgr)->GetKeyDown((eKeyCode::LBTN)) && mbMouseOn == true)
	//{
	//	mOnClick();
	//}

}

void Button::OnFixedUpdate()
{

}

void Button::OnRender()
{

}

void Button::OnFontRender()
{

}

void Button::OnClear()
{

}

void Button::Click()
{
	int a = 0;
}

//glm::vec2 Button::ScreenToCamera(const glm::vec2& screenCoord, const glm::mat4& viewProjectionMatrix, int screenWidth, int screenHeight)
//{
//	glm::vec4 normalizedCoord
//	(
//		(screenCoord.x / screenWidth) * 2,
//		(screenCoord.y / screenHeight) * 2,
//		0.0f,
//		1.0f
//	);
//
//	glm::vec4 cameraCoord = viewProjectionMatrix * normalizedCoord;
//
//	return glm::vec2(cameraCoord);
//}