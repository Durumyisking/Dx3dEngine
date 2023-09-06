#include "guiOutLine.h"

namespace gui
{
	OutLine::OutLine()
		: Widget()
		, mTargetGameObject(nullptr)
		, mTargetResource(nullptr)
		, mComponents{}
		, mResources{}
	{
		mComponents.resize(static_cast<UINT>(eComponentType::End));
		mResources.resize(static_cast<UINT>(eResourceType::End));
	}
	OutLine::~OutLine()
	{
		for (gui::GUIComponent* comp : mComponents)
		{
			delete comp;
			comp = nullptr;
		}

		for (gui::GUIResource* res : mResources)
		{
			delete res;
			res = nullptr;
		}
	}

	void OutLine::FixedUpdate()
	{
	}
	void OutLine::Update()
	{
	}
	void OutLine::LateUpdate()
	{
	}

	void OutLine::ClearTarget()
	{
		mTargetGameObject = nullptr;
		mTargetResource = nullptr;
		mComponents.clear();
		mResources.clear();
	}

	void OutLine::InitializeTargetGameObject()
	{
		ClearTarget();
		
		for (size_t i = 0; i < static_cast<UINT>(eComponentType::End); i++)
		{

		}
	}
	void OutLine::InitializeTargetResource()
	{
	}
}