#include "guiContentsBrowser.h"

#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"

#include "guiInspector.h"
#include "guiResource.h"
#include "guiEditor.h"
#include "guiWidgetMgr.h"

extern gui::Editor editor;

namespace gui
{

	ContentsBrowser::ContentsBrowser()
		: mTreeWidget(nullptr)
	{
		SetName("ContentsBrowser");
		UINT width = 1600;
		UINT height = 900;

		Vector2 size(static_cast<float>(width), static_cast<float>(height));

		SetSize(ImVec2((float)size.x / 2 + size.x / 5, size.y / 4));

		mTreeWidget = new TreeWidget();
		mTreeWidget->SetName("Resources");
		AddWidget(mTreeWidget);

		mTreeWidget->SetEvent(this
			, std::bind(&ContentsBrowser::toInspector, this, std::placeholders::_1));

		mTreeWidget->SetDummyRoot(true);
		ResetContent();
	}

	ContentsBrowser::~ContentsBrowser()
	{
		delete mTreeWidget;
		mTreeWidget = nullptr;
	}

	void ContentsBrowser::FixedUpdate()
	{
		Widget::FixedUpdate();

		//리소스가 바뀐다면 리소스목록 초기화

		//Texture* tex;
		//ImVec2 size;
		//ImGui::Image(tex, size);
	}

	void ContentsBrowser::Update()
	{
		Widget::Update();
	}

	void ContentsBrowser::LateUpdate()
	{

	}

	void ContentsBrowser::ResetContent()
	{
		//mTreeWidget->Close();
		mTreeWidget->Clear();

		TreeWidget::Node* pRootNode = mTreeWidget->AddNode(nullptr, "Resources", 0, true);

		//enum class eResourceType
		//{
		//	GUIMesh,
		//	GUITexture,
		//	GUIMaterial,
		//	Sound,
		//	Prefab,
		//	MeshData,
		//	GraphicsShader,
		//	ComputeShader,
		//	End,
		//};
		AddResources<Mesh>(pRootNode, "Mesh");
		AddResources<Texture>(pRootNode, "Texture");
		AddResources<Material>(pRootNode, "Materials");
		AddResources<Shader>(pRootNode, "Shaders");
	}

	void ContentsBrowser::toInspector(void* data)
	{
		Resource* resource = static_cast<Resource*>(data);

		Inspector* inspector = GETSINGLE(WidgetMgr)->GetWidget<Inspector>("Inspector");
		inspector->SetTargetResource(resource);
		inspector->InitializeTargetResource();
	}
}