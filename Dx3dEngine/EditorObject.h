#pragma once
#include "GameObj.h"

namespace dru
{
    class EditorObject :
        public GameObj
    {
	public:
		EditorObject();
		virtual ~EditorObject();

		virtual void Initialize();
		virtual void update();
		virtual void fixedUpdate();
		virtual void render();

    };

}