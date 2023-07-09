#include "Controller.h"
#include "GameObj.h"
#include "TimeMgr.h"

namespace dru
{
	Controller::Controller()
		: Component(eComponentType::Controller)
	{
	}

	Controller::~Controller()
	{
	}

	void Controller::Initialize()
	{
	}

	void Controller::update()
	{
	}

	void Controller::fixedUpdate()
	{
	}

	void Controller::render()
	{
	}

	void Controller::Move(const PxVec3& vec, PxQueryFilterCallback* callback, PxFilterData filtData)
	{
		PxControllerFilters filters = {};

		filters.mFilterCallback = callback;
		filters.mFilterData = &filtData;
		PxController* controller = GetOwner()->GetComponent<Physical>()->GetController();

		// vec�� �̵� ����� �ӵ��� ��Ÿ����
		if (controller)
		{
			controller->move(vec, 0.001f, DT, filters);
		}
	}

}
