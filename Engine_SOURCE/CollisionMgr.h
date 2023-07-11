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

		void CollisionLayerCheck(eLayerType left, eLayerType right, bool benable = true);
		void LayerCollision(class Scene* scene, eLayerType left, eLayerType right);
		void ColliderCollision(Collider2D* left, Collider2D* right);
		bool Intersect(Collider2D* left, Collider2D* right);

		bool lineLine(Vector2 lineA_p1, Vector2 lineA_p2, Vector2 lineB_p1, Vector2 lineB_p2);
		bool lineRect(Collider2D* left, Collider2D* right);

	private:
		std::bitset<static_cast<UINT>(eLayerType::End)> mLayerCollisionMatrix[static_cast<UINT>(eLayerType::End)];
		std::map<UINT64, bool> mCollisionMap;
	};
}
