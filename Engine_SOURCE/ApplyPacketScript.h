#pragma once
#include "GameNetObject.h"
#include "Script.h"

namespace dru
{
    class ApplyPacketScript :
        public Script, public server::GameNetObject
    {

	public:
		ApplyPacketScript();
		virtual ~ApplyPacketScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
		virtual void fontRender();



    };
}


