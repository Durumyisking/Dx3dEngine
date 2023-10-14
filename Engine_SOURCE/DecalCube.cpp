#include "DecalCube.h"
#include "MeshRenderer.h"
#include "ResourceMgr.h"


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
