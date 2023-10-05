#include "ServerMgr.h"
#include "ServerHeader.h"
#include "ServerToClient.h"
#include "ClientToServer.h"
#include "GlobalPacket.h"
#include "GameNetObject.h"
#include "GameObj.h"
#include "Object.h"
#include "Sphere.h"
#include "Box.h"
#include "Player.h"
#include "PlayerScript.h"
#include "Physical.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"
#include "PhysicalMovement.h"
#include "MeshRenderer.h"

#include "PacketSendScript.h"
#include "ApplyPacketScript.h"

namespace server
{
	ServerMgr::ServerMgr()
		: mServer{}
		, mClient{}
		, mNet(nullptr)
		, mbIsServer(false)
		, mbNetworkOn(false)
	{

	}

	ServerMgr::~ServerMgr()
	{
		GameObj::ClearObjectCDO();
	}

	void ServerMgr::Update()
	{
	}

	void ServerMgr::FixedUpdate()
	{
		if (nullptr != mNet)
		{
			// 클라면 클라 서버면 서버의 PacketProcess를 진행
			mNet->PacketProcess();
		}

	}

	void ServerMgr::NetPacketProcessInit(GameNet* net)
	{
		if (mbIsServer)
		{
			net->mDispatcher.AddHandler<ObjectUpdatePacket>(PacketEnumType::ObjectUpdatePacket,
				[this](std::shared_ptr<ObjectUpdatePacket> packet)
			{
				if (nullptr == mNet)
				{
					MsgBoxAssert("서버 모드도 안켰는데 어케 받음?");
				}

				if (false == GameNetObject::IsNetObject(packet->GetObjectID()))
				{
					Box* otherObj = object::Instantiate<Box>(eLayerType::PhysicalObject);
					otherObj->SetPos(Vector3(-5.f, 20.f, 5.f));
					otherObj->SetScale({ 2.5f, 2.5f, 2.5f });
					otherObj->SetName(L"Sphere");
					otherObj->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");

					ApplyPacketScript* Other = otherObj->AddComponent<ApplyPacketScript>(eComponentType::Script);
					int ObjectID = packet->GetObjectID();
					Other->ClientObjectInit(packet->GetObjectID());
				}

				GameNetObject::PushObjectPacket(packet);

				// 다른 모든애들에게도 보낸다.
				mNet->PacketSend(packet);

			});

		}
		else
		{
			net->mDispatcher.AddHandler<UserIDPacket>(PacketEnumType::UserIDPacket,
				[=](std::shared_ptr<UserIDPacket> packet)
			{
				Player* player = object::Instantiate<Player>(eLayerType::Player);
				player->SetPos(Vector3(5.f, 5.f, 5.f));
				player->SetScale({ 5.f, 5.f, 5.f });
				player->SetName(L"Player");
				Material* mat = GETSINGLE(ResourceMgr)->CreateMaterial(L"dirt_color", L"dirt_normal", L"PhongShader", L"mat_dirt");
				player->GetComponent<MeshRenderer>()->SetMaterial(mat);
				//player->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");
				player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");
				player->AddComponent<PlayerScript>(eComponentType::Script);

				Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
				physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Sphere, Vector3(2.5f, 2.5f, 2.5f));
				PxRigidDynamic* dy = physical->GetActor<PxRigidDynamic>();

				PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

				player->AddComponent<PhysXCollider>(eComponentType::Collider);
				player->AddComponent<PhysicalMovement>(eComponentType::Movement);

				PacketSendScript* SendToServerScript = player->AddComponent<PacketSendScript>(eComponentType::Script);
				int ObjectId = packet->GetObjectID();
				SendToServerScript->ClientObjectInit(packet->GetObjectID());
			});

			net->mDispatcher.AddHandler<ObjectUpdatePacket>(PacketEnumType::ObjectUpdatePacket,
				[=](std::shared_ptr<ObjectUpdatePacket> packet)
			{
				if (false == GameNetObject::IsNetObject(packet->GetObjectID()))
				{
					Box* otherObj = object::Instantiate<Box>(eLayerType::PhysicalObject);
					otherObj->SetPos(Vector3(-5.f, 20.f, 5.f));
					otherObj->SetScale({ 2.5f, 2.5f, 2.5f });
					otherObj->SetName(L"Box");
					otherObj->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");

					ApplyPacketScript* Other = otherObj->AddComponent<ApplyPacketScript>(eComponentType::Script);
					int ObjectID = packet->GetObjectID();
					Other->ClientObjectInit(packet->GetObjectID());
				}
				GameNetObject::PushObjectPacket(packet);
			});
		}
	}

	void ServerMgr::OpenServer()
	{
		// 유저를 Accept했을때 실행할 함수
		mServer.OpenHost(30000, 
			[this](SOCKET socket)
			{
				Box* otherObj = object::Instantiate<Box>(eLayerType::PhysicalObject);
				otherObj->SetPos(Vector3(-5.f, 20.f, 5.f));
				otherObj->SetScale({ 2.5f, 2.5f, 2.5f });
				otherObj->SetName(L"Box");
				otherObj->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");

				ApplyPacketScript* Other = otherObj->AddComponent<ApplyPacketScript>(eComponentType::Script);
				Other->ServerObjectInit();
				std::shared_ptr<UserIDPacket> otherPacket = std::make_shared<UserIDPacket>();

				int ObjectId = Other->GetObjectID();
				otherPacket->SetObjectID(ObjectId);

				GameNetSerializer Ser;
				otherPacket->SerializePacket(Ser);

				send(socket, Ser.GetDataConstPtr(), Ser.GetWriteOffSet(), 0);
			});
		mNet = &mServer;
		mbIsServer = true;
		mbNetworkOn = true;

		// 서버 키면서 플레이어 생성
		Player* player = object::Instantiate<Player>(eLayerType::Player);
		player->SetPos(Vector3(5.f, 5.f, 5.f));
		player->SetScale({ 5.f, 5.f, 5.f });
		player->SetName(L"Player");
		//GUIMaterial* mat = GETSINGLE(ResourceMgr)->CreateMaterial(L"dirt_color", L"dirt_normal", L"PhongShader", L"mat_dirt");
		//player->GetComponent<GUIMeshRenderer>()->SetMaterial(mat);
		player->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");
		player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");
		player->AddComponent<PlayerScript>(eComponentType::Script);

		Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
		physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Sphere, Vector3(2.5f, 2.5f, 2.5f));
		PxRigidDynamic* dy = physical->GetActor<PxRigidDynamic>();

		PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

		player->AddComponent<PhysXCollider>(eComponentType::Collider);
		player->AddComponent<PhysicalMovement>(eComponentType::Movement);

		PacketSendScript* SendToServerScript = player->AddComponent<PacketSendScript>(eComponentType::Script);
		SendToServerScript->ServerObjectInit();

		NetPacketProcessInit(mNet);
	}

	void ServerMgr::ConnectAsClient()
	{
		mClient.Connect("127.0.0.1", 30000);
		mNet = &mClient;
		mbNetworkOn = true;

		NetPacketProcessInit(mNet);
	}


}