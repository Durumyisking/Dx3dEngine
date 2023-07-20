#pragma once
#include "def.h"
#include "Macro.h"
#include "GameNetServer.h"
#include "GameNetClient.h"


namespace dru::server
{
	class ServerMgr
	{
		SINGLE(ServerMgr)

	public:
		void Update();
		void FixedUpdate();


		void NetPacketProcessInit(GameNet* net);

		void OpenServer(); // 서버가 될 컴퓨터에서 사용
		void ConnectAsClient(); // 클라로 들어갈 때 사용

		bool IsNetworkOn() const { return mbNetworkOn; }

	public:
		GameNetServer mServer;
		GameNetClient mClient;
		GameNet*	  mNet;
		
		bool		  mbIsServer;
		bool		  mbNetworkOn;
	};
}


