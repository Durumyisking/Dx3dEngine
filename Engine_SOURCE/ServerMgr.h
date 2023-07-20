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

		void OpenServer(); // ������ �� ��ǻ�Ϳ��� ���
		void ConnectAsClient(); // Ŭ��� �� �� ���

		bool IsNetworkOn() const { return mbNetworkOn; }

	public:
		GameNetServer mServer;
		GameNetClient mClient;
		GameNet*	  mNet;
		
		bool		  mbIsServer;
		bool		  mbNetworkOn;
	};
}


