#include "GameObj.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "Scene.h"

#include "Component.h"
#include "Script.h"
#include "Mesh.h"
#include "Shader.h"
#include "AudioListener.h"
#include "AudioSource.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"
#include "Physical.h"
#include "Transform.h"

#include "RigidBody.h"


	
GameObj::GameObj()
	:mState(eState::Active)
	, mType(eLayerType::None)
	, mScripts{}
	, mbDestroy(false)
	, mbBlockRendering(false)
{
	mComponents.resize(static_cast<UINT>(eComponentType::End));
	this->AddComponent<Transform>(eComponentType::Transform);
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

		comp->Update();
	}
	for (Component* script : mScripts)
	{
		if (nullptr == script || GETSINGLE(SceneMgr)->GetActiveScene()->mbPause )
			continue;
		script->Update();
	}
}

void GameObj::FixedUpdate()
{
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
		comp->FixedUpdate();
	}

	for (Component* script : mScripts)
	{
		if (nullptr == script || GETSINGLE(SceneMgr)->GetActiveScene()->mbPause)
			continue;
		script->FixedUpdate();
	}
}

void GameObj::Render()
{
	for (Component* comp : mComponents)
	{
		if (nullptr == comp)
			continue;
		comp->Render();
	}
		
	for (Component* script : mScripts)
	{
		if (nullptr == script)
			continue;
		script->Render();
	}
}

void GameObj::PrevRender()
{
	for (Component* comp : mComponents)
	{
		if (comp == nullptr)
			continue;

		comp->PrevRender();
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

void GameObj::SetMesh(Mesh* mesh)
{
	if (nullptr != GetComponent<SpriteRenderer>())
	{
		GetComponent<SpriteRenderer>()->SetMesh(mesh);
	}
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