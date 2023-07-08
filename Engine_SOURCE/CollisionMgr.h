#pragma once
#include "CommonInclude.h"
#include "Collider2D.h"

namespace dru
{
	union ColliderID
	{
		struct
		{
			UINT32 left;
			UINT32 right;
		};
		UINT64 id;
	};

	class CollisionMgr
	{
		SINGLE(CollisionMgr)

	public:
		void Initialize();
		void update();
		void fixedUpdate();
		void render();

		void CollisionLayerCheck(eLayerType _left, eLayerType _right, bool _benable = true);
		void LayerCollision(class Scene* _scene, eLayerType _left, eLayerType _right);
		void ColliderCollision(Collider2D* _left, Collider2D* _right);
		bool Intersect(Collider2D* _left, Collider2D* _right);

		bool lineLine(Vector2 _lineA_p1, Vector2 _lineA_p2, Vector2 _lineB_p1, Vector2 _lineB_p2);
		bool lineRect(Collider2D* _left, Collider2D* _right);

	private:
		std::bitset<static_cast<UINT>(eLayerType::End)> mLayerCollisionMatrix[static_cast<UINT>(eLayerType::End)];
		std::map<UINT64, bool> mCollisionMap;
	};
}
