#include "guiEditor.h"
#include "Mesh.h"
#include "ResourceMgr.h"
#include "Material.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "GridScript.h"
#include "Object.h"
#include "Application.h"
#include "GraphicDevice.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "guiInspector.h"
#include "guiGame.h"
#include "guiHierarchy.h"
#include "guiProject.h"
#include "guiEditor.h"
#include "guiConsole.h"
#include "guiListWidget.h"

#include "guiVisualEditor.h"

#include "guiDebugObject.h"
#include "guiEditorObject.h"
#include "guiWidget.h"
#include "Graphics.h"


extern dru::Application application;

namespace gui
{
	Editor::Editor()
		: mWidgets{}
		, mEditorObjects{}
		, mDebugObjects{}
		, mVisualEditor(nullptr)
		, mEnable(false)
		, mImguiEnable(true)

	{
	}
	Editor::~Editor()
	{
	}

	void Editor::Initialize()
	{
		mEnable = false;
		mImguiEnable = false;

		if (mEnable == false)
			return;
		// 충돌체의 종류 갯수만큼만 있으면 된다.
		mDebugObjects.resize(static_cast<UINT>(eColliderType::End));

		dru::Mesh* rectMesh = GETSINGLE(dru::ResourceMgr)->Find<dru::Mesh>(L"DebugRectmesh");
		dru::Material* material = GETSINGLE(dru::ResourceMgr)->Find<Material>(L"DebugMaterial");

		mDebugObjects[static_cast<UINT>(eColliderType::Rect)] = new DebugObject();
		dru::MeshRenderer* renderer
			= mDebugObjects[static_cast<UINT>(eColliderType::Rect)]->AddComponent<dru::MeshRenderer>(eComponentType::MeshRenderer);

		renderer->SetMaterial(material);
		renderer->SetMesh(rectMesh);

		dru::Mesh* circleMesh = GETSINGLE(dru::ResourceMgr)->Find<dru::Mesh>(L"Circlemesh");

		mDebugObjects[static_cast<UINT>(eColliderType::Circle)] = new DebugObject();
		renderer
			= mDebugObjects[static_cast<UINT>(eColliderType::Circle)]->AddComponent<dru::MeshRenderer>(eComponentType::MeshRenderer);

		renderer->SetMaterial(material);
		renderer->SetMesh(circleMesh);


		if (mImguiEnable == false)
			return;
		
		ImGui_Initialize();

		mVisualEditor = new VisualEditor();
		//mWidgets.push_back(mVisualEditor);

		// Init Widget 
		Inspector* inspector = new Inspector();
		mWidgets.insert(std::make_pair("Inspector", inspector));

		Game* game = new Game();
		mWidgets.insert(std::make_pair("Game", game));

		Hierarchy* hierarchy = new Hierarchy();
		mWidgets.insert(std::make_pair("Hierarchy", hierarchy));

		Project* project = new Project();
		mWidgets.insert(std::make_pair("Project", project));
		
		Console* console = new Console();
		mWidgets.insert(std::make_pair("Console", console));

		ListWidget* listWidget = new ListWidget();
		mWidgets.insert(std::make_pair("ListWidget", listWidget));

	}

	void Editor::Run()
	{
		if (mEnable == false)
			return;

		Update();
		FixedUpdate();
		Render();

		if (mImguiEnable == false)
			return;

		ImGui_Run();
	}

	void Editor::Update()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->update();
		}
	}

	void Editor::FixedUpdate()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->fixedUpdate();
		}
	}

	void Editor::Render()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->render();
		}

		for ( DebugMesh& mesh : dru::renderer::debugMeshes)
		{
			DebugRender(mesh);
		}
		dru::renderer::debugMeshes.clear();
	}

	void Editor::Release()
	{
		if (mEnable == false)
			return;

		for (auto iter : mWidgets)
		{
			delete iter.second;
			iter.second = nullptr;
		}

		delete mVisualEditor;
		mVisualEditor = nullptr;

		for (auto obj : mEditorObjects)
		{
			delete obj;
			obj = nullptr;
		}

		delete mDebugObjects[static_cast<UINT>(eColliderType::Rect)];
		delete mDebugObjects[static_cast<UINT>(eColliderType::Circle)];

		if (mImguiEnable == false)
			return;
		ImGui_Release();
	}

	void Editor::DebugRender(dru::graphics::DebugMesh& mesh)
	{
		DebugObject* debugObj = mDebugObjects[static_cast<UINT>(mesh.type)];

		dru::Transform* tr = debugObj->GetComponent<dru::Transform>();
		tr->SetPosition(mesh.position);
		tr->SetRotation(mesh.rotation);

		switch (mesh.type)
		{
		case dru::enums::eColliderType::Rect:
			tr->SetScale(mesh.scale);
			break;
		case dru::enums::eColliderType::Circle:
			tr->SetScale(Vector3(mesh.radius));
			break;
		case dru::enums::eColliderType::Line:
			tr->SetScale(Vector3(mesh.scale.x, 1.f, 0.f));
			break;
		case dru::enums::eColliderType::Box:
			break;
		case dru::enums::eColliderType::Sphere:
			break;
		case dru::enums::eColliderType::End:
			break;
		default:
			break;
		}

		dru::BaseRenderer* renderer = debugObj->GetComponent<dru::BaseRenderer>();

		Material* material = renderer->GetMaterial();
		material->SetData(eGPUParam::Int_1, &mesh.state);
		tr->fixedUpdate();

		dru::Camera::SetGpuViewMatrix(dru::renderer::mainCamera->GetViewMatrix());
		dru::Camera::SetGpuProjectionMatrix(dru::renderer::mainCamera->GetProjectionMatrix());

		debugObj->render();

	}

	void Editor::ImGui_Initialize()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;
		//io.ConfigViewportsNoDefaultParent = true;
		//io.ConfigDockingAlwaysTabBar = true;
		//io.ConfigDockingTransparentPayload = true;
		//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
		//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(application.GetHwnd());
		ImGui_ImplDX11_Init(dru::graphics::GetDevice()->GetID3D11Device()
			, dru::graphics::GetDevice()->GetDeviceContext().Get());

		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
		// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
		// - Read 'docs/FONTS.md' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		//io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
		//IM_ASSERT(font != nullptr);

		// Our state

	}

	void Editor::ImGui_Run()
	{
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		ImGuiIO io = ImGui::GetIO();

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		//for (Widget* widget : mWidgets) 
		//{
		//	widget->update();
		//}

		mVisualEditor->Render();

		for (auto iter : mWidgets)
		{
			iter.second->Render();
		}

#pragma region  SAMPLE
		 //2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		//{
		//	static float f = 0.0f;
		//	static int counter = 0;

		//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		//	ImGui::Checkbox("Another Window", &show_another_window);

		//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//		counter++;
		//	ImGui::SameLine();
		//	ImGui::Text("counter = %d", counter);

		//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		//	ImGui::End();
		//}

		//// 3. Show another simple window.
		//if (show_another_window)
		//{
		//	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		//	ImGui::Text("Hello from another window!");
		//	if (ImGui::Button("Close Me"))
		//		show_another_window = false;
		//	ImGui::End();
		//}
#pragma endregion
		// Rendering
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		
		// update and render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void Editor::ImGui_Release()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}
