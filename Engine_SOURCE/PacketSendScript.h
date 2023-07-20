#pragma once
#include "GameNetObject.h"
#include "Script.h"

namespace dru
{
    class PacketSendScript :
        public Script, public server::GameNetObject
    {
	public:
		PacketSendScript();
		virtual ~PacketSendScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
		virtual void fontRender();

	private:
		Timer* mTimer;
    };

}
