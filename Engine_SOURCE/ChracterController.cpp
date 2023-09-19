#include "ChracterController.h"
#include "TimeMgr.h"
#include "PhysicsMgr.h"
#include "SimpleMath.h"
#include "GameObj.h"
#include "Physical.h"

ChracterController::ChracterController()
	: Component(eComponentType::End)
	, mRadius(0.5f)
	, mHeight(1.f)
	, mbUseGravity(false)
{
}

ChracterController::~ChracterController()
{
}

void ChracterController::Initialize()
{
}

void ChracterController::Update()
{
	if (!mbUseGravity)
	{
		Move(math::Vector3(0.f, 0.5f, 0.f));
	}
}

void ChracterController::FixedUpdate()
{
}

void ChracterController::Render()
{
}

void ChracterController::Create()
{
	assert(GetPhysical());
	
	GETSINGLE(PhysicsMgr)->GetEnvironment()->CreateCharacterController(this, GetPhysical()->GetProperties()->GetMaterial());
}

void ChracterController::Move(math::Vector3 displacement)
{
	auto disp = convert::Vector3ToPxVec3(displacement);
	PxControllerFilters filters;
	mController->move(disp, 0.001f, DT, filters);

	mController->getActor()->addForce(PxVec3(50.f, 50.f, 50.f), PxForceMode::eIMPULSE);
	auto& controllerPos = mController->getPosition();
	//GetOwner()->SetPos(math::Vector3(controllerPos.x, controllerPos.y, controllerPos.z));
}
