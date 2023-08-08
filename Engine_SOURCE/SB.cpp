#include "SB.h"
#include "ResourceMgr.h"
#include "MeshRenderer.h"

const int SkyboxSize = 512;// 2048;
const int IrradianceSize = 512;//
const int PreFilterSize = 256;
const int BrdfLookupSize = 512;


SB::SB()
{
}

SB::~SB()
{
}

void SB::Initialize()
{
	ID3D11Device* device = GetDevice()->GetID3D11Device();
	ID3D11DeviceContext* deviceContext = GetDevice()->GetDeviceContext().Get();

	std::vector<Texture*> cubeFaces;

	Texture* envImg = GETSINGLE(ResourceMgr)->Find<Texture>(L"environment");

	// Create faces
	for (int i = 0; i < 6; ++i)
	{
		Texture* renderTexture = new Texture;
		renderTexture->Create(SkyboxSize, SkyboxSize, DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
		cubeFaces.push_back(renderTexture);
	}

	Material* mat = new Material();
	mat->SetTexture(envImg);
	mat->SetShaderByKey(L"RectToCubemapShader");
	GETSINGLE(ResourceMgr)->Insert<Material>(L"SkyboxMat", mat);

	MeshRenderer* mr = AddComponent<MeshRenderer>(eComponentType::MeshRenderer);
	mr->SetMeshByKey(L"Cubemesh");
	mr->SetMaterial(mat);

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

