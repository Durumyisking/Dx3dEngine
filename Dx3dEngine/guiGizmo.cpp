#include "guiGizmo.h"

namespace gui
{
	Gizmo::Gizmo()
		: EditorObject()
		, mTargetGameObject(nullptr)
		, mFoward(nullptr)
		, mRight(nullptr)
		, mUp(nullptr)
	{
	}
	Gizmo::~Gizmo()
	{
	}

	void Gizmo::Initialize()
	{
	}
	void Gizmo::update()
	{
	}
	void Gizmo::fixedUpdate()
	{
	}
	void Gizmo::render()
	{
	}

	void Gizmo::MoveTarget()
	{
		EditorObject* basis = GetSelectedBasis();

		if (mFoward == basis)
		{
			
		}
		else if (mRight == basis)
		{

		}
		else if (mUp == basis)
		{

		}
	}

	EditorObject* Gizmo::GetSelectedBasis()
	{
		return nullptr;
	}

	void Gizmo::ClearTarget()
	{
	}

	void Gizmo::InitializeTargetGameObject()
	{
	}
}