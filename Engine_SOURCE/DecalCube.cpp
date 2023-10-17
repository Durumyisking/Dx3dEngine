#include "DecalCube.h"
#include "MeshRenderer.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "TimeMgr.h"


DecalCube::DecalCube()
	:GameObj()
	, mMateiral(nullptr)
{
	
}

DecalCube::~DecalCube()
{
	if (mMateiral)
	{
		delete mMateiral;
		mMateiral = nullptr;
	}
}

void DecalCube::Initialize()
{
	mMateiral = new Material();
	mMateiral->SetRenderingMode(eRenderingMode::Decal);

	Shader* decalShader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DecalShader");
	if(decalShader)
		mMateiral->SetShader(decalShader);

	Texture* depthMap = GETSINGLE(ResourceMgr)->Find<Texture>(L"DepthMapTexture");
	if(depthMap)
		mMateiral->SetTexture(eTextureSlot::DepthMap, depthMap);

	Mesh* cubeMesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Cubemesh");

	MeshRenderer* meshRenderer = AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	meshRenderer->SetMesh(cubeMesh);
	meshRenderer->SetMaterial(mMateiral);

	SUPER::Initialize();
}

void DecalCube::Render()
{
	SUPER::Render();
}

void DecalCube::Update()
{
	SUPER::Update();

	Transform* tr = GetTransform();
	Vector3 pos = tr->GetPosition();
	Vector3 sacle = tr->GetScale();
	if (KEY_DOWN(UP))
	{
		pos.y += 10.f * DT;
	}
	else if (KEY_DOWN(DOWN))
	{
		pos.y -= 10.f * DT;
	}

	if (KEY_DOWN(LEFT))
	{
		pos.x -= 10.f * DT;
	}
	else if (KEY_DOWN(RIGHT))
	{
		pos.x += 10.f * DT;
	}

	if (KEY_DOWN(SPACE))
	{
		sacle.x += 1.f*DT;
		sacle.y += 1.f * DT;
		sacle.z += 1.f * DT;
	}

	tr->SetPosition(pos);
	tr->SetScale(sacle);
}
