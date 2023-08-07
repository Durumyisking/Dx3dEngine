#include "SkyBox.h"
#include "MeshRenderer.h"
#include "ResourceMgr.h"


SkyBox::SkyBox()
	: GameObj()
	, mSkyTexture(nullptr)
{
}

SkyBox::SkyBox(const std::wstring& texName, const std::wstring& path)
	: GameObj()
	, mSkyTexture(nullptr)
{
	mSkyTexture = new Texture();
	mSkyTexture->Load(path);
	GETSINGLE(ResourceMgr)->Insert<Texture>(texName, mSkyTexture);
}

SkyBox::~SkyBox()
{
}

void SkyBox::Initialize()
{
	AddComponent<MeshRenderer>(eComponentType::MeshRenderer);

	// 구형 메쉬 세팅
	MeshRenderer* mr = GetComponent<MeshRenderer>();
	Mesh* mesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Spheremesh");
	mr->SetMesh(mesh);

	// 머터리얼 세팅
	Material* mater = GETSINGLE(ResourceMgr)->Find<Material>(L"SkyBoxMaterial");
	mr->SetMaterial(mater);

	if (mSkyTexture != nullptr)
		mater->SetTexture(eTextureSlot::Albedo, mSkyTexture);

	SetScale(Vector3(1000.f, 1000.f, 1000.f));
}


void SkyBox::Update()
{
	GameObj::Update();
}

void SkyBox::FixedUpdate()
{
	if (mTraceObj)
		SetPos(mTraceObj->GetPos());

	GameObj::FixedUpdate();
}

void SkyBox::Render()
{
	BaseRenderer* renderer = GetComponent<BaseRenderer>();
	if (nullptr == renderer)
		return;

	if (nullptr == renderer->GetMaterial()->GetTexture(eTextureSlot::Albedo))
		return;

	GameObj::Render();
}

void SkyBox::TextureLoad(const std::wstring& key, const std::wstring& path)
{
	if (mSkyTexture != nullptr)
		return;

	mSkyTexture = new Texture();
	mSkyTexture->Load(path);
	GETSINGLE(ResourceMgr)->Insert<Texture>(key, mSkyTexture);
}
