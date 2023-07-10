#include "CollisionMgr.h"
#include "Scene.h"
#include "SceneMgr.h"
#include "GameObj.h"

namespace dru
{
	CollisionMgr::CollisionMgr()
	{

	}

	CollisionMgr::~CollisionMgr()
	{

	}

	void CollisionMgr::Initialize()
	{

	}

	void CollisionMgr::update()
	{
		Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();

		for (UINT row = 0; row < static_cast<UINT>(eLayerType::End); row++)
		{
			for (UINT col = 0; col < static_cast<UINT>(eLayerType::End); col++)
			{
				if (mLayerCollisionMatrix[row][col])
				{
					LayerCollision(scene, static_cast<eLayerType>(col), static_cast<eLayerType>(row));
				}
			}
		}
	}

	void CollisionMgr::fixedUpdate()
	{
	}

	void CollisionMgr::render()
	{
	}


	void CollisionMgr::CollisionLayerCheck(eLayerType left, eLayerType right, bool benable)
	{
		int row = 0;
		int col = 0;

		// Matrix 절반만 사용
		if (static_cast<UINT>(left) <= static_cast<UINT>(right))
		{
			row = static_cast<UINT>(left);
			col = static_cast<UINT>(right);
		}
		else
		{
			row = static_cast<UINT>(right);
			col = static_cast<UINT>(left);
		}

		mLayerCollisionMatrix[row][col] = benable;
	}

	void CollisionMgr::LayerCollision(Scene* _scene, eLayerType left, eLayerType right)
	{
		const std::vector<GameObj*>& lefts = _scene->GetGameObj(left);
		const std::vector<GameObj*>& rights = _scene->GetGameObj(right);

		for (GameObj* left : lefts)
		{

			if (left->GetState() != GameObj::eState::Active)
				continue;

			if (!left->GetComponent<Collider2D>())
				continue;

			for (GameObj* right : rights)
			{
				if (right->GetState() != GameObj::eState::Active)
					continue;

				if (!right->GetComponent<Collider2D>())
					continue;

				if (left == right)
					continue;


				ColliderCollision(left->GetComponent<Collider2D>(), right->GetComponent<Collider2D>());

			}
		}
	}

	void CollisionMgr::ColliderCollision(Collider2D* left, Collider2D* right)
	{
		ColliderID colliderID;
		colliderID.left = static_cast<UINT>(left->GetColliderID());
		colliderID.right = static_cast<UINT>(right->GetColliderID());

		std::map<UINT64, bool>::iterator iter = mCollisionMap.find(colliderID.id);

		if (mCollisionMap.end() == iter) // 순회했는데 해당 id의 true인애가 없었다. 충돌중이지 않았다.
		{
			mCollisionMap.insert(std::make_pair(colliderID.id, false));
			iter = mCollisionMap.find(colliderID.id);
		}

		// 충돌체크
		if (Intersect(left, right)) // 충돌을 한 상태
		{
			// 첫 충돌
			if (iter->second == false)
			{
				if (!left->IsOn() || !right->IsOn())
				{
					return;
				}

				if (left->IsTrigger())
					left->OnTriggerEnter(right);
				else
					left->OnCollisionEnter(right);

				if (right->IsTrigger())
					right->OnTriggerEnter(left);
				else
					right->OnCollisionEnter(left);

				iter->second = true;
				left->SetState(eCollisionState::CollisionEnter);
				right->SetState(eCollisionState::CollisionEnter);

			}
			else // 충돌 중
			{
				if (!left->IsOn() || !right->IsOn())
				{
					left->OnCollisionExit(right);				
					right->OnCollisionExit(left);
					left->SetState(eCollisionState::CollisionExit);
					right->SetState(eCollisionState::CollisionExit);

					return;
				}

				if (left->IsTrigger())
					left->OnTrigger(right);
				else
					left->OnCollision(right);

				if (right->IsTrigger())
					right->OnTrigger(left);
				else
					right->OnCollision(left);

				left->SetState(eCollisionState::CollisionStay);
				right->SetState(eCollisionState::CollisionStay);

			}
		}
		else // 충돌 X
		{
			if (iter->second) // 충돌 빠져나감
			{
				if (left->IsTrigger())
					left->OnTriggerExit(right);
				else
					left->OnCollisionExit(right);

				if (right->IsTrigger())
					right->OnTriggerExit(left);
				else
					right->OnCollisionExit(left);

				iter->second = false;

				left->SetState(eCollisionState::CollisionExit);
				right->SetState(eCollisionState::CollisionExit);

			}
			else
			{
				left->SetState(eCollisionState::CollisionNot);
				right->SetState(eCollisionState::CollisionNot);

				mCollisionMap.erase(iter);
			}
		}
	}

	bool CollisionMgr::Intersect(Collider2D* left, Collider2D* right)
	{

	#pragma region RectVsRect

		if (eColliderType::Rect == left->GetType() && eColliderType::Rect == right->GetType())
		{
			Vector3 arrLocalPos[4] =
			{
				Vector3{-0.5f, 0.5f, 0.0f},
				Vector3{0.5f, 0.5f, 0.0f},
				Vector3{0.5f, -0.5f, 0.0f},
				Vector3{-0.5f, -0.5f, 0.0f}
			};

			Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
			Transform* rightTr = right->GetOwner()->GetComponent<Transform>();

			Matrix leftMatrix = leftTr->GetWorldMatrix();
			Matrix rightMatrix = rightTr->GetWorldMatrix();

			// 분리축 벡터 (투영벡터)
			Vector3 Axis[4] = {};
		
			Vector3 leftScale = Vector3(left->GetScale().x, left->GetScale().y, 1.0f);
			Matrix finalLeft = Matrix::CreateScale(leftScale);
			finalLeft *= leftMatrix;

			Vector3 rightScale = Vector3(right->GetScale().x, right->GetScale().y, 1.0f);
			Matrix finalRight = Matrix::CreateScale(rightScale);
			finalRight *= rightMatrix;

			Axis[0] = (Vector3::Transform(arrLocalPos[1], finalLeft));
			Axis[1] = (Vector3::Transform(arrLocalPos[3], finalLeft));
			Axis[2] = (Vector3::Transform(arrLocalPos[1], finalRight));
			Axis[3] = (Vector3::Transform(arrLocalPos[3], finalRight));

			Axis[0] -= Vector3::Transform(arrLocalPos[0], finalLeft);
			Axis[1] -= Vector3::Transform(arrLocalPos[0], finalLeft);
			Axis[2] -= Vector3::Transform(arrLocalPos[0], finalRight);
			Axis[3] -= Vector3::Transform(arrLocalPos[0], finalRight);
			

			for (size_t i = 0; i < 4; ++i)
				Axis[i].z = 0.f;

			Vector3 vc = leftTr->GetWorldPosition() - rightTr->GetWorldPosition();
			vc.z = 0.f;

			Vector3 centerDir = vc;
			for (size_t i = 0; i < 4; i++)
			{
				Vector3 vObjectAxis = Axis[i];
				//vA.Normalize();

				float projDist = 0.f;
				for (size_t j = 0; j < 4; j++)
				{
					projDist += fabsf(Axis[j].Dot(vObjectAxis) / 2.f);
				}

				if (projDist < fabsf(centerDir.Dot(vObjectAxis)))
				{
					return false;
				}
			}
		}

	#pragma endregion

	#pragma region CircleVsCircle
		
		else if (eColliderType::Circle == left->GetType() && eColliderType::Circle == right->GetType())
		{
			Vector2 leftPos = { left->GetColliderPos().x, left->GetColliderPos().y };
			Vector2 rightPos = { right->GetColliderPos().x, right->GetColliderPos().y };

			float Gap = (leftPos - rightPos).Length();

			float leftScale = left->GetRadius() * 0.5f;
			float rightScale = right->GetRadius() * 0.5f;

			if ((leftScale + rightScale) < Gap)
			{
				return false;
			}
		}



	#pragma endregion


	#pragma region RectVsLine
		else if (eColliderType::Line == left->GetType() && eColliderType::Rect == right->GetType())
		{
			return lineRect(left, right);
		}

	#pragma endregion


		return true;
	}

	bool CollisionMgr::lineLine(Vector2 lineA_p1, Vector2 lineA_p2, Vector2 lineB_p1, Vector2 lineB_p2)
	{
		// calculate the distance to intersection point
		  // uA의 분모는 직선의 기울기
		  // uA는 선분 A위에 있는 B 즉 A와 B의 교차점이 A선 위의 어느정도 비율인가
		  // (uA가 0.5면 선분 B는 A의 중간지점에서 겹치고 0이면 A의 시작지점에서 겹친다)

		float uA =
			((lineB_p2.x - lineB_p1.x) 
				* (lineA_p1.y - lineB_p1.y)
				- (lineB_p2.y - lineB_p1.y)
				* (lineA_p1.x - lineB_p1.x))
				/ ((lineB_p2.y - lineB_p1.y)
				* (lineA_p2.x - lineA_p1.x)
				- (lineB_p2.x - lineB_p1.x) 
				* (lineA_p2.y - lineA_p1.y));

		float uB =
			((lineA_p2.x - lineA_p1.x)
				* (lineA_p1.y - lineB_p1.y)
				- (lineA_p2.y - lineA_p1.y) 
				* (lineA_p1.x - lineB_p1.x))
				/ ((lineB_p2.y - lineB_p1.y) 
				* (lineA_p2.x - lineA_p1.x)
				- (lineB_p2.x - lineB_p1.x)
				* (lineA_p2.y - lineA_p1.y));

		// uA와 uB가 둘 다 0~1사이면 충돌중이다! 
		if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
		{
			return true;
		}
		return false;

	}

	bool CollisionMgr::lineRect(Collider2D* left, Collider2D* right)
	{
		Vector3 leftPos = left->GetColliderPos();
		Vector2 leftScale = left->GetScale() / 2.f;

		Vector2 lineP1 = {};
		Vector2 lineP2 = {};

		lineP1.x = leftPos.x - leftScale.x;
		lineP2.x = leftPos.x + leftScale.x;
		lineP1.y = leftPos.y;
		lineP2.y = leftPos.y;

		//// 회전
		//float rot = _left->GetOwner()->GetRotation().z;
		//lineP1.


		Vector3 rightPos = right->GetColliderPos();
		Vector2 rightScale = right->GetScale() / 2.f;

		Vector2 rectlineP1 = {};
		Vector2 rectlineP2 = {};

		rectlineP1.x = rightPos.x - rightScale.x;
		rectlineP2.x = rightPos.x + rightScale.x;
		rectlineP1.y = rightPos.y + rightScale.y;
		rectlineP2.y = rightPos.y + rightScale.y;

		bool bTop = lineLine(lineP1, lineP2, rectlineP1, rectlineP2);

		rectlineP1.x = rightPos.x - rightScale.x;
		rectlineP2.x = rightPos.x + rightScale.x;
		rectlineP1.y = rightPos.y - rightScale.y;
		rectlineP2.y = rightPos.y - rightScale.y;

		bool bBottom = lineLine(lineP1, lineP2, rectlineP1, rectlineP2);

		rectlineP1.x = rightPos.x - rightScale.x;
		rectlineP2.x = rightPos.x - rightScale.x;
		rectlineP1.y = rightPos.y - rightScale.y;
		rectlineP2.y = rightPos.y + rightScale.y;
		
		bool bLeft = lineLine(lineP1, lineP2, rectlineP1, rectlineP2);
		
		rectlineP1.x = rightPos.x + rightScale.x;
		rectlineP2.x = rightPos.x + rightScale.x;
		rectlineP1.y = rightPos.y - rightScale.y;
		rectlineP2.y = rightPos.y + rightScale.y;
		
		bool bRight = lineLine(lineP1, lineP2, rectlineP1, rectlineP2);


		if (bTop || bBottom || bLeft || bRight)
		{
			return true;
		}

		return false;
	}
}