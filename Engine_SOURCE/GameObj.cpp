#include "GameObj.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "Scene.h"

#include "Component.h"

#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "RigidBody.h"
#include "PhysXRigidBody.h"
#include "PhysicalMovement.h"
#include "Physical.h"
#include "PhysXCollider.h"
#include "Animator.h"
#include "BoneAnimator.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"
#include "PostProcessRenderer.h"
#include "ParticleSystem.h"
#include "Light.h"

std::map<std::string, GameObj*> GameObj::mObjectCDO;
	

GameObj::GameObj()
	:mState(eState::Active)
	, mType(eLayerType::Default)
	, mScripts{}
	, mbDestroy(true)
	, mbBlockRendering(false)
	, mPath{}
{
	mComponents.resize(static_cast<UINT>(eComponentType::End));
	this->AddComponent<Transform>(eComponentType::Transform);

	mObjectTypeName = "GameObj";
}

GameObj::GameObj(const GameObj& Obj)
	: Entity(Obj)
	, mbDestroy(Obj.mbDestroy)
	, mbBlockRendering(Obj.mbBlockRendering)
{
	mComponents.resize(static_cast<UINT>(eComponentType::End));
	this->AddComponent<Transform>(eComponentType::Transform);

	mObjectTypeName = Obj.mObjectTypeName;
}

GameObj::~GameObj()
{
	for (Component* comp : mComponents)
	{
		if (nullptr == comp)
			continue;

		delete comp;
		comp = nullptr;
	}

	for (Component* script : mScripts)
	{
		if (nullptr == script)
			continue;

		delete script;
		script = nullptr;
	}

}


GameObj* GameObj::Clone() const
{
	return new GameObj(*this);
}

void GameObj::Save(FILE* File)
{
	DruEntity::Save(File);	

	////컴포넌트 따로 저장할거면 사용(미완)
	//{
	//	auto	iter = mComponents.begin();
	//	auto	iterEnd = mComponents.end();
	//
	//	for (; iter != iterEnd; ++iter)
	//	{
	//		eComponentType compType = (*iter)->GetOrder();
	//		fwrite(&compType, sizeof(eComponentType), 1, File);
	//
	//		(*iter)->Save(File);
	//	}
	//}

	Transform* tr = GetComponent<Transform>();

	math::Vector3 pos = tr->GetPosition();
	math::Vector3 rotation = tr->GetRotation();
	math::Vector3 scale = tr->GetScale();
	float offscale = tr->GetOffsetScale();

	fwrite(&pos, sizeof(math::Vector3), 1, File);
	fwrite(&rotation, sizeof(math::Vector3), 1, File);
	fwrite(&scale, sizeof(math::Vector3), 1, File);
	fwrite(&offscale, sizeof(float), 1, File);
}

void GameObj::Load(FILE* File)
{
	DruEntity::Load(File);	
	
	Transform* tr = GetComponent<Transform>();

	math::Vector3 pos;
	math::Vector3 rotation;
	math::Vector3 scale;
	float offscale;

	fread(&pos, sizeof(math::Vector3), 1, File);
	fread(&rotation, sizeof(math::Vector3), 1, File);
	fread(&scale, sizeof(math::Vector3), 1, File);
	fread(&offscale, sizeof(float), 1, File);

	tr->SetPosition(pos);
	tr->SetRotation(rotation);
	tr->SetScale(scale);
	tr->SetOffsetScale(offscale);
}

void GameObj::Initialize()
{
	for (Component* comp : mComponents)
	{
		if (nullptr == comp)
			continue;
		comp->Initialize();		
	}

	for (Component* script : mScripts)
	{
		if (nullptr == script)
			continue;
		script->Initialize();
	}
}

void GameObj::Update()
{
	if (mState != eState::Active)
		return;

	for (Component* comp : mComponents)
	{
		if (nullptr == comp)
			continue;		
		if (GETSINGLE(SceneMgr)->GetActiveScene()->mbPause && (eComponentType::Renderer != comp->GetOrder() && eComponentType::Camera != comp->GetOrder() && eComponentType::Light != comp->GetOrder()))
		{
			if (eLayerType::UI != mType)
			{
				continue;
			}
		}
		if (comp->IsSwitchOn())
		{
			comp->Update();
		}
	}
	for (Component* script : mScripts)
	{
		if (nullptr == script || GETSINGLE(SceneMgr)->GetActiveScene()->mbPause )
			continue;

		if (script->IsSwitchOn())
		{
			script->Update();
		}
	}
	std::list<GameObj*> list = GetCollisionObjs();
	for (GameObj* obj : list)
	{
		if (obj->GetPhysXCollider())
			OnTriggerPersist(obj);
	}

}

void GameObj::FixedUpdate()
{
	if (mState != eState::Active)
		return;

	for (Component* comp : mComponents)
	{
		if (nullptr == comp)
			continue;
		if (GETSINGLE(SceneMgr)->GetActiveScene()->mbPause && (eComponentType::Renderer != comp->GetOrder() && eComponentType::Camera != comp->GetOrder() && eComponentType::Light != comp->GetOrder()))
		{
			if (eLayerType::UI != mType)
			{
				continue;
			}
		}
		if (comp->IsSwitchOn())
		{
			comp->FixedUpdate();
		}
	}

	for (Component* script : mScripts)
	{
		if (nullptr == script || GETSINGLE(SceneMgr)->GetActiveScene()->mbPause)
			continue;
		if (script->IsSwitchOn())
		{
			script->FixedUpdate();
		}
	}
}

void GameObj::Render()
{
	if (mState != eState::Active)
		return;

	for (Component* comp : mComponents)
	{
		if (nullptr == comp)
			continue;
		if (comp->IsSwitchOn())
		{
			comp->Render();
		}
	}
		
	for (Component* script : mScripts)
	{
		if (nullptr == script)
			continue;
		if (script->IsSwitchOn())
		{
			script->Render();
		}
	}
}

void GameObj::PrevRender()
{
	if (eState::Active == mState)
	{
		MeshRenderer* mr = GetComponent<MeshRenderer>();
		if (mr)
		{
			if (mr->IsSwitchOn())
			{
				mr->PrevRender();
			}
		}
	}
}

void GameObj::FontRender()
{
	for (Script* script : mScripts)
	{
		if (nullptr == script)
			continue;
		script->fontRender();
	}
}

void GameObj::OnTriggerEnter(GameObj* gameObject)
{
	mCollisionObj.push_back(gameObject);
}

void GameObj::OnTriggerExit(GameObj* gameObject)
{
	mCollisionObj.erase(std::remove(mCollisionObj.begin(), mCollisionObj.end(), gameObject), mCollisionObj.end());
}

void GameObj::AddComponent(Component* component)
{
	eComponentType order = component->GetOrder();


	if (order != eComponentType::Script)
	{
		mComponents[static_cast<UINT>(order)] = component;
		mComponents[static_cast<UINT>(order)]->SetOwner(this);
	}
	else
	{
		mScripts.push_back(dynamic_cast<Script*>(component));
		component->SetOwner(this);
	}
}

void GameObj::SetPos(Vector3 value)
{
	Transform* tr = GetComponent<Transform>();

	tr->SetPosition(value);		
}

void GameObj::SetPosAbs(Vector3 value)
{
	Transform* tr = GetComponent<Transform>();
	tr->SetPosition(value);
}

void GameObj::SetScale(Vector3 value)
{
	if (nullptr != GetComponent<Transform>())
	{
		GetComponent<Transform>()->SetScale(value);
	}
	if (nullptr != GetComponent<Physical>())
	{
		GetComponent<Physical>()->SetGeometrySize(value);	
	}
}

void GameObj::SetRotation(Vector3 value)
{
	if (nullptr != GetComponent<Transform>())
	{
		GetComponent<Transform>()->SetRotation(value);
	}
}

Vector3 GameObj::GetPos()
{
	assert(GetComponent<Transform>());
	return GetComponent<Transform>()->GetPosition();
}
Vector3 GameObj::GetWorldPos()
{
	assert(GetComponent<Transform>());
	return GetComponent<Transform>()->GetWorldPosition();
}

Vector3 GameObj::GetUIWorldPos()
{
	Vector3 UIWorldPos = {};
	if (this->GetLayerType() == eLayerType::UI)
	{
		Vector3 UIPos = GetComponent<Transform>()->GetWorldPosition();
		Vector3 CamPos = renderer::mainCamera->GetOwner()->GetWorldPos();
		UIWorldPos.z = UIPos.z;
		UIWorldPos.x = UIPos.x + CamPos.x;
		UIWorldPos.y = UIPos.y + CamPos.y;
	}

	return UIWorldPos;
}

Vector3 GameObj::GetScale()
{
	return GetComponent<Transform>()->GetScale();
}

Vector3 GameObj::GetRotation()
{
	return GetComponent<Transform>()->GetRotation();
}

void GameObj::SetMaterial(Material* material)
{
	if (nullptr != GetComponent<SpriteRenderer>())
	{
		GetComponent<SpriteRenderer>()->SetMaterial(material);
	}
}

Material* GameObj::GetMaterial()
{
	return this->GetComponent<SpriteRenderer>()->GetMaterial();
}

void GameObj::SetMesh(Mesh* mesh)
{
	if (nullptr != GetComponent<SpriteRenderer>())
	{
		GetComponent<SpriteRenderer>()->SetMesh(mesh);
	}
}

float GameObj::Calculate_RelativeDirection_ByCosTheta(GameObj* otherObj)
{
	Vector3 thisToOther = otherObj->GetWorldPos() - GetWorldPos();
	thisToOther.Normalize();
	Vector3 thisUpVector = GetTransform()->WorldUp();

	float cosTheta = thisToOther.Dot(thisUpVector);

	return cosTheta;
}

bool GameObj::IsObjectInFOV(GameObj* otherObj, float FOV)
{
	Vector3 faceDir = -GetTransform()->WorldForward();
	Vector3 a = otherObj->GetTransform()->GetWorldPosition();
	Vector3 b = GetTransform()->GetWorldPosition();

	Vector3 thisToOther = a - b;
	thisToOther.Normalize();

	float costTheta = faceDir.Dot(thisToOther);
	float FovRadian = cosf(toRadian(FOV));

	if (costTheta > FovRadian)
	{
		return true;
	}

	return false;
}

bool GameObj::MoveToTarget_Smooth_bool(GameObj* target, float speed, bool zOn, eDir dir)
{

	if (!target)
		return false;

	Vector3 TargetPos;
	Vector3 ObjPos;
	Vector3 result;

	if (zOn)
	{
		TargetPos = target->GetPos();
		ObjPos = GetPos();
		result = TargetPos;
	}
	else
	{
		TargetPos = Vector3(target->GetPos().x, target->GetPos().y, 0.f);
		ObjPos = Vector3(GetPos().x, GetPos().y, 0.f);
		result = Vector3(TargetPos.x, TargetPos.y, GetPos().z);
	}


	float Distance = 0.f;
	float Speed = 0.f;

	Distance = (TargetPos - ObjPos).Length();

	Vector3 Dir = (TargetPos - ObjPos);
	Dir.Normalize();

	switch (dir)
	{
	case enums::eDir::UP:
	case enums::eDir::DOWN:
		Dir.x = 0;
		break;
	case enums::eDir::LEFT:
	case enums::eDir::RIGHT:
		Dir.y = 0;
		break;
	default:
		break;
	}

	Speed = Distance / speed;

	if (Distance < 0.01f)
	{
		Speed = 1.f;
	}

	float Step = Speed * GETSINGLE(TimeMgr)->DeltaTimeConstant();

	if (Step < Distance)
	{

		ObjPos += Dir * Step;
		this->SetPos(ObjPos);

		return false;
	}

	this->SetPos(result);
	return true;

}

Vector3 GameObj::MoveToTarget_Smooth_vector3(GameObj* target, float speed, bool zOn, eDir dir)
{
	if (!target)
		return Vector3::Zero;

	Vector3 TargetPos = Vector3(target->GetPos().x, target->GetPos().y, 0.f);
	Vector3 ObjPos = Vector3(GetPos().x, GetPos().y, 0.f);
	Vector3 result = Vector3(TargetPos.x, TargetPos.y, GetPos().z);

	float Distance = 0.f;
	float Speed = 0.f;

	Distance = (TargetPos - ObjPos).Length();

	Vector3 Dir = (TargetPos - ObjPos);
	Dir.Normalize();

	switch (dir)
	{
	case enums::eDir::UP:
	case enums::eDir::DOWN:
		Dir.x = 0;
		Distance = fabs(TargetPos.y - ObjPos.y);
		result.x = GetPos().x;
		break;
	case enums::eDir::LEFT:
	case enums::eDir::RIGHT:
		Distance = fabs(ObjPos.x - TargetPos.x);
		Dir.y = 0;
		result.y = GetPos().y;
		break;
	default:
		break;
	}

	Speed = Distance / speed;

	if (Distance < 0.001f)
	{
		Speed = 1.f;
	}

	float Step = Speed * GETSINGLE(TimeMgr)->DeltaTimeConstant();

	if (Step < Distance)
	{

		ObjPos += Dir * Step;
		this->SetPos(ObjPos);

		return ObjPos;
	}
		
	this->SetPos(result);
	return result;
}

void GameObj::ReorganizePosition(AXIS axis, eLayerType layerType, bool oneSide)
{
	assert(GetComponent<Physical>());
	assert(GetComponent<PhysXCollider>());
	assert(GetComponent<PhysXRigidBody>());

	const auto& gameObjects = GETSINGLE(SceneMgr)->GetActiveScene()->GetGameObjects(layerType);

	Vector3 vResult = {};
	for (const auto& gameObject : gameObjects)
	{
		vResult = GetPhysXCollider()->ComputePenetration(gameObject);
		if (vResult != Vector3::Zero)
		{
			switch (axis)
			{
			case enums::AXIS::X:
				vResult.y = 0.f;
				vResult.z = 0.f;
				GetPhysXRigidBody()->SetVelocity(AXIS::X, Vector3(0.f, 0.f, 0.f));
				GetTransform()->SetPhysicalPosition(GetTransform()->GetPhysicalPosition() + vResult);
				break;
			case enums::AXIS::Y:
				vResult.x = 0.f;
				vResult.z = 0.f;
				GetPhysXRigidBody()->SetVelocity(AXIS::Y, Vector3(0.f, 0.f, 0.f));
				if (oneSide)
				{
					vResult.y = vResult.y > 0.f ? vResult.y : -vResult.y;
					GetTransform()->SetPhysicalPosition(GetTransform()->GetPhysicalPosition() + vResult);
				}
				else
				{
					GetTransform()->SetPhysicalPosition(GetTransform()->GetPhysicalPosition() + vResult);
				}
				break;
			case enums::AXIS::Z:
				vResult.x = 0.f;
				vResult.y = 0.f;
				GetPhysXRigidBody()->SetVelocity(AXIS::Z, Vector3(0.f, 0.f, 0.f));
				break;
			case enums::AXIS::XY:
				vResult.z = 0.f;
				GetPhysXRigidBody()->SetVelocity(AXIS::XY, Vector3(0.f, 0.f, 0.f));
				break;
			case enums::AXIS::XZ:
				vResult.y = 0.f;
				GetPhysXRigidBody()->SetVelocity(AXIS::XZ, Vector3(0.f, 0.f, 0.f));
				GetTransform()->SetPhysicalPosition(GetTransform()->GetPhysicalPosition() + vResult);
				break;
			case enums::AXIS::YZ:
				vResult.x = 0.f;
				GetPhysXRigidBody()->SetVelocity(AXIS::YZ, Vector3(0.f, 0.f, 0.f));
				break;
			case enums::AXIS::XYZ:
				GetPhysXRigidBody()->SetVelocity(AXIS::XYZ, Vector3(0.f, 0.f, 0.f));
				GetTransform()->SetPhysicalPosition(GetTransform()->GetPhysicalPosition() + vResult);
				break;
			case enums::AXIS::END:
				break;
			default:
				break;
			}


			return;
		}
	}
}

Transform* GameObj::GetTransform()
{	
	Transform* comp = dynamic_cast<Transform*>( mComponents[static_cast<UINT>(eComponentType::Transform)]);
	assert(comp);
	return comp;
}

Camera* GameObj::GetCamera()
{
	Camera* comp = dynamic_cast<Camera*>(mComponents[static_cast<UINT>(eComponentType::Camera)]);
	assert(comp);
	return comp;
}


RigidBody* GameObj::GetRigidBody()
{	
	RigidBody* comp = dynamic_cast<RigidBody*>(mComponents[static_cast<UINT>(eComponentType::RigidBody)]);
	assert(comp);
	return comp;
}

PhysXRigidBody* GameObj::GetPhysXRigidBody()
{
	PhysXRigidBody* comp = dynamic_cast<PhysXRigidBody*>(mComponents[static_cast<UINT>(eComponentType::RigidBody)]);
	assert(comp);
	return comp;
}

PhysicalMovement* GameObj::GetMovement()
{
	PhysicalMovement* comp = dynamic_cast<PhysicalMovement*>(mComponents[static_cast<UINT>(eComponentType::Movement)]);
	assert(comp);
	return comp;
}

Physical* GameObj::GetPhysical()
{
	Physical* comp = dynamic_cast<Physical*>(mComponents[static_cast<UINT>(eComponentType::Physical)]);
	assert(comp);
	return comp;
}

PhysXCollider* GameObj::GetPhysXCollider()
{
	PhysXCollider* comp = dynamic_cast<PhysXCollider*>(mComponents[static_cast<UINT>(eComponentType::Collider)]);
	assert(comp);
	return comp;
}

Animator* GameObj::GetAnimator()
{
	Animator* comp = dynamic_cast<Animator*>(mComponents[static_cast<UINT>(eComponentType::Animator)]);
	assert(comp);
	return comp;
}

BoneAnimator* GameObj::GetBoneAnimator()
{
	BoneAnimator* comp = dynamic_cast<BoneAnimator*>(mComponents[static_cast<UINT>(eComponentType::BoneAnimator)]);
	assert(comp);
	return comp;
}

MeshRenderer* GameObj::GetMeshRenderer()
{
	MeshRenderer* comp = dynamic_cast<MeshRenderer*>(mComponents[static_cast<UINT>(eComponentType::MeshRenderer)]);
	assert(comp);
	return comp;
}

SpriteRenderer* GameObj::GetSpriteRenderer()
{
	SpriteRenderer* comp = dynamic_cast<SpriteRenderer*>(mComponents[static_cast<UINT>(eComponentType::Renderer)]);
	assert(comp);
	return comp;
}


ParticleSystem* GameObj::GetParticle()
{
	ParticleSystem* comp = dynamic_cast<ParticleSystem*>(mComponents[static_cast<UINT>(eComponentType::Particle)]);
	assert(comp);
	return comp;
}

Light* GameObj::GetLight()
{
	Light* comp = dynamic_cast<Light*>(mComponents[static_cast<UINT>(eComponentType::Light)]);
	assert(comp);
	return comp;
}