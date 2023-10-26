#include "guiGame.h"
#include "ResourceMgr.h"
#include "Texture.h"
#include "GraphicDevice.h"

namespace gui
{
	Game::Game()
		: mTexture(nullptr)
	{
		SetName("GameScene");

		mTexture = new Texture();
		mTexture->Create(1600, 900, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

		GETSINGLE(ResourceMgr)->Insert<Texture>(L"UIGameSceneTexture", mTexture);
	}

	Game::~Game()
	{
	}

	void Game::FixedUpdate()
	{
		Texture* renderTarget = GETSINGLE(ResourceMgr)->Find<Texture>(L"RenderTargetTexture");
		
		//Texture* gameTex = new Texture();
		//gameTex->Create(1600, 900, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

		//61 번 셰이더 리소스 뷰 null초기화
		ID3D11ShaderResourceView* gameSRV = nullptr;
		GetDevice()->BindShaderResource(eShaderStage::PS, 61, &gameSRV);
		GetDevice()->CopyResource(mTexture->GetTexture().Get()
			, renderTarget->GetTexture().Get());

		mTexture->BindShaderResource(eShaderStage::PS, 61);
	}

	void Game::Update()
	{



		ImGuiIO io = ImGui::GetIO();
		ImVec2 panelSize = ImGui::GetWindowSize();
		ImVec4 tintColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImVec2 scene_size = ImVec2(io.DisplaySize.x, io.DisplaySize.y);

		ImGui::Image((void*)mTexture->GetSRV().Get(), panelSize
				, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), tintColor);
	}

	void Game::LateUpdate()
	{
	}

	void Game::Render()
	{
		Widget::Render();
	}

}
