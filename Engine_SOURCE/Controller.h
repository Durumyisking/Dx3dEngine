#pragma once
#include "Component.h"
namespace dru
{
	class Controller :
		public Component
	{

    public:
        Controller();
        virtual ~Controller();

        virtual void Initialize();
        virtual void update();
        virtual void fixedUpdate();
        virtual void render();

    public:
        void Move(const PxVec3& vec, PxQueryFilterCallback* callback, PxFilterData filtData);
	};
}
