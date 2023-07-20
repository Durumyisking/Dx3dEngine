#include "PacketSendScript.h"
#include "TimerMgr.h"
#include "GlobalPacket.h"
#include "ServerMgr.h"



using namespace server;

PacketSendScript::PacketSendScript()
	: mTimer(nullptr)
{
}

PacketSendScript::~PacketSendScript()
{
	mTimer->SetFinish(true);
	mTimer->SetDestroy(true);
}

void PacketSendScript::Initialize()
{

	// 패킷 타입마다 세분화 해야한다.
	mTimer = new Timer(1.f / 24.f);
	mTimer->SetDestroy(false);
	mTimer->Event() = [this]
	{
		std::shared_ptr<ObjectUpdatePacket> objectPtr = std::make_shared<ObjectUpdatePacket>();
		objectPtr->SetObjectID(GetObjectID());
		objectPtr->ObjectType = 999;
		objectPtr->Pos = GetOwnerWorldPos();

		GETSINGLE(ServerMgr)->mNet->PacketSend(objectPtr);
	};			
	GETSINGLE(TimerMgr)->AddTimer(mTimer);
}

void PacketSendScript::Update()
{


}

void PacketSendScript::FixedUpdate()
{
}

void PacketSendScript::Render()
{
}

void PacketSendScript::fontRender()
{
}
