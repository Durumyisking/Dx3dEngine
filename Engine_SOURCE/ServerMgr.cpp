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

namespace dru::server
{
	ServerMgr::ServerMgr()
		: mServer{}
		, mClient{}
		, mNet(nullptr)
	{

	}

	ServerMgr::~ServerMgr()
	{

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
		net->mDispatcher.AddHandler<ObjectUpdatePacket>(PacketEnumType::ObjectUpdatePacket,
			[this](std::shared_ptr<ObjectUpdatePacket> packet)
		{
			if (nullptr == mNet)
			{
				MsgBoxAssert("서버 모드도 안켰는데 어케 받음?");
			}

			if (false == GameNetObject::IsNetObject(packet->GetObjectId()))
			{
				/*Sphere* otherObj = object::Instantiate<Sphere>(eLayerType::PhysicalObject);
				otherObj->SetPos(Vector3(-5.f, 20.f, 5.f));
				otherObj->SetScale({ 2.5f, 2.5f, 2.5f });
				otherObj->SetName(L"Sphere");
				otherObj->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");

				int ObjectID = packet->GetObjectId();

				otherObj->ClientObjectInit(packet->GetObjectId());*/
			}

			GameNetObject::PushObjectPacket(packet);
		});


		net->mDispatcher.AddHandler<UserIDPacket>(PacketEnumType::UserIDPacket,
			[](std::shared_ptr<UserIDPacket> packet)
		{
			//PacketEnumType Enum = packet->GetPacketIDToEnum<PacketEnumType>();

			//Player* player = object::Instantiate<Player>(eLayerType::Player);
			//player->SetPos(Vector3(5.f, 5.f, 5.f));
			//player->SetScale({ 5.f, 5.f, 5.f });
			//player->SetName(L"Player");
			//Material* mat = GETSINGLE(ResourceMgr)->CreateMaterial(L"dirt_color", L"dirt_normal", L"PhongShader", L"mat_dirt");
			//player->GetComponent<MeshRenderer>()->SetMaterial(mat);
			////player->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");
			//player->GetComponent<MeshRenderer>()->SetMeshByKey(L"Spheremesh");
			//player->AddComponent<PlayerScript>(eComponentType::Script);

			//Physical* physical = player->AddComponent<Physical>(eComponentType::Physical);
			//physical->InitialDefaultProperties(eActorType::Dynamic, eGeometryType::Sphere, Vector3(2.5f, 2.5f, 2.5f));
			//PxRigidDynamic* dy = physical->GetActor<PxRigidDynamic>();

			//PhysXRigidBody* rigid = player->AddComponent<PhysXRigidBody>(eComponentType::RigidBody);

			//player->AddComponent<PhysXCollider>(eComponentType::Collider);
			//player->AddComponent<PhysicalMovement>(eComponentType::Movement);

			//PlayerScript* Other = player->GetComponent<PlayerScript>();

			//int ObjectId = packet->GetObjectId();

			//Other->ClientObjectInit(packet->GetObjectId());
		}
		);
	}

	void ServerMgr::OpenServer(SOCKET socket)
	{
		// 유저를 Accept했을때 실행할 함수
		mServer.OpenHost(30000, 
			[this](SOCKET socket)
			{
				/*Sphere* hostObj = object::Instantiate<Sphere>(eLayerType::PhysicalObject);
				hostObj->SetPos(Vector3(-5.f, 20.f, 5.f));
				hostObj->SetScale({ 2.5f, 2.5f, 2.5f });
				hostObj->SetName(L"Sphere");
				hostObj->GetComponent<MeshRenderer>()->SetMaterialByKey(L"PhongMaterial");

				hostObj->ServerObjectInit();

				std::shared_ptr<UserIDPacket> hostPacket = std::make_shared<UserIDPacket>();

				int ObjectId = hostObj->GetNetObjectId();

				hostPacket->SetObjectId(hostObj->GetNetObjectId());
				GameNetSerializer Ser;
				hostPacket->SerializePacket(Ser);

				send(socket, Ser.GetDataConstPtr(), Ser.GetWriteOffSet(), 0);*/
			});
		mNet = &mServer;

		// 서버 키면서 플레이어 생성
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

		NetPacketProcessInit(mNet);
	}

	void ServerMgr::ConnectAsClient()
	{
		mClient.Connect("127.0.0.1", 30000);
		mNet = &mClient;
		NetPacketProcessInit(mNet);
	}


}