#include "SB.h"
#include "ResourceMgr.h"
#include "MeshRenderer.h"

SB::SB()
{
}

SB::~SB()
{
}

void SB::Initialize()
{
	MeshRenderer* mr = AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mr->SetMeshByKey(L"Cubemesh");

	ID3D11Device* device = GetDevice()->GetID3D11Device();
	ID3D11DeviceContext* deviceContext = GetDevice()->GetDeviceContext().Get();

	std::vector<Texture*> cubeFaces;

	Texture* envImg = GETSINGLE(ResourceMgr)->Find<Texture>(L"environment");

	//// Create faces
	//for (int i = 0; i < 6; ++i)
	//{
	//	RenderTexture* renderTexture = new RenderTexture;
	//	renderTexture->Initialise(device, SkyboxSize, SkyboxSize, 1);
	//	cubeFaces.push_back(renderTexture);
	//}



}
void SB::Update()
{

	GameObj::Update();
}

void SB::FixedUpdate()
{

	GameObj::FixedUpdate();
}

void SB::Render()
{

	GameObj::Render();
}

