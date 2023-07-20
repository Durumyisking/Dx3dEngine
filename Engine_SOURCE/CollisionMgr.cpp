#include "CollisionMgr.h"
#include "Scene.h"
#include "SceneMgr.h"
#include "GameObj.h"



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

	// Matrix ���ݸ� ���
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

void CollisionMgr::LayerCollision(Scene* scene, eLayerType _left, eLayerType right)
{
	const std::vector<GameObj*>& lefts = scene->GetGameObj(_left);
	const std::vector<GameObj*>& rights = scene->GetGameObj(right);

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

	if (mCollisionMap.end() == iter) // ��ȸ�ߴµ� �ش� id�� true�ξְ� ������. �浹������ �ʾҴ�.
	{
		mCollisionMap.insert(std::make_pair(colliderID.id, false));
		iter = mCollisionMap.find(colliderID.id);
	}

	// �浹üũ
	if (Intersect(left, right)) // �浹�� �� ����
	{
		// ù �浹
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
		else // �浹 ��
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
	else // �浹 X
	{
		if (iter->second) // �浹 ��������
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

		// �и��� ���� (��������)
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
		// uA�� �и�� ������ ����
		// uA�� ���� A���� �ִ� B �� A�� B�� �������� A�� ���� ������� �����ΰ�
		// (uA�� 0.5�� ���� B�� A�� �߰��������� ��ġ�� 0�̸� A�� ������������ ��ģ��)

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

	// uA�� uB�� �� �� 0~1���̸� �浹���̴�! 
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

	//// ȸ��
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
