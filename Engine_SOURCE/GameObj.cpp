#include "GameObj.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "Scene.h"

namespace dru
{
	
	GameObj::GameObj()
		:mState(eState::Active)
		, mType(eLayerType::None)
		, mScripts{}
		, mbDestroy(false)
		, mbIsLeft(false)
		, mbBlockRendering(false)
		, mbOnFloor(false)
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

	void GameObj::update()
	{
		for (Component* comp : mComponents)
		{
			if (nullptr == comp)
				continue;		
			if (SceneMgr::mActiveScene->mbPause && (eComponentType::Renderer != comp->GetOrder() && eComponentType::Camera != comp->GetOrder() && eComponentType::Light != comp->GetOrder()))
			{
				if (eLayerType::UI != mType)
				{
					continue;
				}
			}

			comp->update();
		}
		for (Component* script : mScripts)
		{
			if (nullptr == script || SceneMgr::mActiveScene->mbPause )
				continue;
			script->update();
		}
	}

	void GameObj::fixedUpdate()
	{
		for (Component* comp : mComponents)
		{
			if (nullptr == comp)
				continue;
			if (SceneMgr::mActiveScene->mbPause && (eComponentType::Renderer != comp->GetOrder() && eComponentType::Camera != comp->GetOrder() && eComponentType::Light != comp->GetOrder()))
			{
				if (eLayerType::UI != mType)
				{
					continue;
				}
			}
			comp->fixedUpdate();
		}

		for (Component* script : mScripts)
		{
			if (nullptr == script || SceneMgr::mActiveScene->mbPause)
				continue;
			script->fixedUpdate();
		}
	}

	void GameObj::render()
	{
		for (Component* comp : mComponents)
		{
			if (nullptr == comp)
				continue;
			comp->render();
		}
		
		for (Component* script : mScripts)
		{
			if (nullptr == script)
				continue;
			script->render();
		}
	}

	void GameObj::fontRender()
	{
		for (Script* script : mScripts)
		{
			if (nullptr == script)
				continue;
			script->fontRender();
		}
	}

	void GameObj::AddComponent(Component* _Component)
	{
		eComponentType order = _Component->GetOrder();


		if (order != eComponentType::Script)
		{
			mComponents[(UINT)order] = _Component;
			mComponents[(UINT)order]->SetOwner(this);
		}
		else
		{
			mScripts.push_back(dynamic_cast<Script*>(_Component));
			_Component->SetOwner(this);
		}
	}

	void GameObj::SetPos(Vector3 _Value)
	{
		Transform* tr = GetComponent<Transform>();

		if (nullptr != tr)
		{

			if (eProjectionType::Orthographic == renderer::mainCamera->GetProjectionType())
			{
				switch (mType)
				{
				case dru::enums::eLayerType::Camera:
					break;
				case dru::enums::eLayerType::Grid:
					break;
				case dru::enums::eLayerType::BackGround:
					_Value.z = 5.f;
					break;
				case dru::enums::eLayerType::Objects:
					_Value.z = 4.9f;
					break;
				case dru::enums::eLayerType::Monster:
					_Value.z = 3.1f;
					break;
				case dru::enums::eLayerType::Boss:
					_Value.z = 3.1f;
					break;
				case dru::enums::eLayerType::MonsterGun:
					_Value.z = 0.f;
					break;
				case dru::enums::eLayerType::Player:
					_Value.z = 3.f;
					break;
				case dru::enums::eLayerType::Bullet:
					_Value.z = 2.81f;
					break;
				case dru::enums::eLayerType::AfterImage:
//					_Value.z = 3.000001f;
					break;
				case dru::enums::eLayerType::Platforms:
					_Value.z = 4.9f;
					break;
				case dru::enums::eLayerType::Ray:
					_Value.z = 4.f;
					break;
				case dru::enums::eLayerType::Particle:
					_Value.z = 2.8f;
					break;
				case dru::enums::eLayerType::FX:
					_Value.z = 2.000001f;
					break;
				case dru::enums::eLayerType::UI:
					_Value.z = 0.1f;
					break;
				case dru::enums::eLayerType::PostProcess:
					break;
				case dru::enums::eLayerType::MainMenu:
					_Value.z = 0.05f;
					break;
				case dru::enums::eLayerType::End:
					break;
				default:
					break;
				}
				if (tr->GetParent())
				{
					_Value.z = 0.0001f;
				}
			}
			tr->SetPosition(_Value);

		}
	}

	void GameObj::SetPosAbs(Vector3 _Value)
	{
		Transform* tr = GetComponent<Transform>();
		tr->SetPosition(_Value);
	}

	void GameObj::SetScale(Vector3 _Value)
	{
		if (nullptr != GetComponent<Transform>())
		{
			GetComponent<Transform>()->SetScale(_Value);
		}
		if (nullptr != GetComponent<SpriteRenderer>())
		{
			GetComponent<SpriteRenderer>()->ChangeSize();
		}
		if (nullptr != GetComponent<MeshRenderer>())
		{
			GetComponent<MeshRenderer>()->ChangeSize();
		}
	}

	void GameObj::SetRotation(Vector3 _Value)
	{
		if (nullptr != GetComponent<Transform>())
		{
			GetComponent<Transform>()->SetRotation(_Value);
		}
	}

	Vector3 GameObj::GetPos()
	{
		return GetComponent<Transform>()->GetPosition();
	}
	Vector3 GameObj::GetWorldPos()
	{
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

	void GameObj::SetMaterial(std::shared_ptr<Material> _Material)
	{
		if (nullptr != GetComponent<SpriteRenderer>())
		{
			GetComponent<SpriteRenderer>()->SetMaterial(_Material);
		}
	}

	void GameObj::SetMesh(std::shared_ptr<Mesh> _Mesh)
	{
		if (nullptr != GetComponent<SpriteRenderer>())
		{
			GetComponent<SpriteRenderer>()->SetMesh(_Mesh);
		}
	}

	void GameObj::Flip()
	{
		BaseRenderer* baseRenderer = GetComponent<BaseRenderer>();
		if (baseRenderer)
		{
			std::shared_ptr<Material> mtrl = baseRenderer->GetMaterial();
			if (mtrl)
			{

#define INVERSE -1
#define NORMAL 1


				int isInverse = mbIsLeft ? INVERSE : NORMAL;

				mtrl->SetData(eGPUParam::Int_1, &isInverse);
			}
		}

	}
	bool GameObj::MoveToTarget_Smooth_bool(GameObj* _target, float _speed, bool _zOn, eDir _dir)
	{

		if (!_target)
			return false;

		Vector3 TargetPos;
		Vector3 ObjPos;
		Vector3 result;

		if (_zOn)
		{
			TargetPos = _target->GetPos();
			ObjPos = GetPos();
			result = TargetPos;
		}
		else
		{
			TargetPos = Vector3(_target->GetPos().x, _target->GetPos().y, 0.f);
			ObjPos = Vector3(GetPos().x, GetPos().y, 0.f);
			result = Vector3(TargetPos.x, TargetPos.y, GetPos().z);
		}


		float Distance = 0.f;
		float Speed = 0.f;

		Distance = (TargetPos - ObjPos).Length();

		Vector3 Dir = (TargetPos - ObjPos);
		Dir.Normalize();

		switch (_dir)
		{
		case dru::enums::eDir::UP:
		case dru::enums::eDir::DOWN:
			Dir.x = 0;
			break;
		case dru::enums::eDir::LEFT:
		case dru::enums::eDir::RIGHT:
			Dir.y = 0;
			break;
		default:
			break;
		}

		Speed = Distance / _speed;

		if (Distance < 0.01f)
		{
			Speed = 1.f;
		}

		float Step = Speed * TimeMgr::DeltaTimeConstant();

		if (Step < Distance)
		{

			ObjPos += Dir * Step;
			this->SetPos(ObjPos);

			return false;
		}

		this->SetPos(result);
		return true;

	}

	Vector3 GameObj::MoveToTarget_Smooth_vector3(GameObj* _target, float _speed, bool _zOn, eDir _dir)
	{
		if (!_target)
			return Vector3::Zero;

		Vector3 TargetPos = Vector3(_target->GetPos().x, _target->GetPos().y, 0.f);
		Vector3 ObjPos = Vector3(GetPos().x, GetPos().y, 0.f);
		Vector3 result = Vector3(TargetPos.x, TargetPos.y, GetPos().z);

		float Distance = 0.f;
		float Speed = 0.f;

		Distance = (TargetPos - ObjPos).Length();

		Vector3 Dir = (TargetPos - ObjPos);
		Dir.Normalize();

		switch (_dir)
		{
		case dru::enums::eDir::UP:
		case dru::enums::eDir::DOWN:
			Dir.x = 0;
			Distance = fabs(TargetPos.y - ObjPos.y);
			result.x = GetPos().x;
			break;
		case dru::enums::eDir::LEFT:
		case dru::enums::eDir::RIGHT:
			Distance = fabs(ObjPos.x - TargetPos.x);
			Dir.y = 0;
			result.y = GetPos().y;
			break;
		default:
			break;
		}

		Speed = Distance / _speed;

		if (Distance < 0.001f)
		{
			Speed = 1.f;
		}

		float Step = Speed * TimeMgr::DeltaTimeConstant();

		if (Step < Distance)
		{

			ObjPos += Dir * Step;
			this->SetPos(ObjPos);

			return ObjPos;
		}
		
		this->SetPos(result);
		return result;
	}
}