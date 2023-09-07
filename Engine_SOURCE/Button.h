#pragma once
#include "UIBase.h"

//#include "GLM/glm.hpp"
//#include "GLM/gtc/matrix_transform.hpp"

class Button : public UIBase
{
public:
	Button();
	Button(Vector3 pos);
	Button(eUIType type);
	virtual ~Button();

	virtual void OnInit() override;
	virtual void OnActive() override;
	virtual void OnInActive() override;
	virtual void OnUpdate() override;
	virtual void OnFixedUpdate() override;
	virtual void OnRender() override;
	virtual void OnFontRender() override;
	virtual void OnClear() override;
	
	virtual void Click();


//	glm::vec2 ScreenToCamera(const glm::vec2& screenCoord, const glm::mat4& viewProjectionMatrix, int screenWidth, int screenHeight);

private:
	Event mOnClick;
	bool mbMouseOn;

	Vector3 mScreenPos;
};