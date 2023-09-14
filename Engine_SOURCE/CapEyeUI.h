#pragma once
#include "HUD.h"

class CapEyeUI : public HUD
{
public:
	CapEyeUI();
	CapEyeUI(const std::wstring& key, eUIType type);
	~CapEyeUI();

	void OnUpdate() override;
	void OnClear() override;


	bool mbEyeblink;
};