#include "guiGroupWidget.h"

namespace gui
{
	GroupWidget::GroupWidget()
		: Widget()
		, mSpace{10.f, 0.f}
		, mNextLine(10)
		, mbCollapse(false)
		, mbGroupOpen(false)
	{
	}
	GroupWidget::GroupWidget(std::string name)
		: Widget()
		, mSpace(10.f, 0.f)
		, mNextLine(10)
		, mbCollapse(false)
		, mbGroupOpen(false)
	{
		SetName(name);
	}

	GroupWidget::~GroupWidget()
	{
		for (size_t i = 0; i < mChilds.size(); ++i)
		{
			if (mChilds[i])
			{
				delete mChilds[i];
				mChilds[i] = nullptr;
			}
		}

		mChilds.clear();
	}

	void GroupWidget::FixedUpdate()
	{
	}

	void GroupWidget::Update()
	{
	}

	void GroupWidget::LateUpdate()
	{
	}

	void GroupWidget::Render()
	{
		ImGui::BeginGroup();

		if (mbCollapse)
		{
			if (ImGui::CollapsingHeader(GetName().c_str(), &mbGroupOpen))
			{
				for (size_t i = 0; i < mChilds.size(); i++)
				{
					mChilds[i]->Render();

					if (i != 0)
					{
						if (i % mNextLine == 0)
							continue;
					}
						ImGui::SameLine();
						ImGui::Dummy(mSpace);
						//ImGui::Indent(float);
						ImGui::SameLine();
				}
			}
		}
		else
		{
			for (size_t i = 0; i < mChilds.size(); i++)
			{
				mChilds[i]->Render();

				if (i != 0) 
				{
					if (i % mNextLine == 0)
						continue;
				}

				ImGui::SameLine();
				ImGui::Dummy(mSpace);
				//ImGui::Indent(float);
				ImGui::SameLine();
			}
		}

		ImGui::EndGroup();
	}
	void GroupWidget::Clear()
	{
		for (size_t i = 0; i < mChilds.size(); ++i)
		{
			if (mChilds[i])
			{
				delete mChilds[i];
				mChilds[i] = nullptr;
			}
		}

		mChilds.clear();
	}
}