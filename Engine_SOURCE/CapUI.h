#pragma once
#include "HUD.h"

class CapUI : public HUD
{
public:
	CapUI();
	CapUI(const std::wstring& key, eUIType type);
	~CapUI();

	void OnUpdate() override;
	void OnClear() override;

	void Move();
};