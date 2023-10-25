#include "guiWidgetMgr.h"
#include "Application.h"
#include "GraphicDevice.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


#include "guiConsole.h"
#include "guiGame.h"
#include "guiHierarchy.h"
#include "guiInspector.h"
#include "guiProject.h"


#include "guiComponent.h"
#include "guiListWidget.h"
#include "guiMaterial.h"
#include "guiMesh.h"
#include "guiMeshRenderer.h"
#include "guiResource.h"
#include "guiTransform.h"
#include "guiTexture.h"
#include "guiTreeWidget.h"
#include "guiVisualEditor.h"
#include "guiGizmo.h"
#include "guiOutLiner.h"
#include "guiSceneSaveWindow.h"
#include "guiObjectWindow.h"

#include "InputMgr.h"

extern Application application;

namespace gui
{
	WidgetMgr::WidgetMgr()
		: mWidgets{}
		, mVisualEditor(nullptr)
		, mbForceReset(false)
	{

	}

	WidgetMgr::~WidgetMgr()
	{
	}

	void WidgetMgr::Initialize()
	{
		ImGui_Initialize();

		// Initialize Widget 
		mVisualEditor = new VisualEditor();

		//Game* game = new Game();
		//mWidgets.insert(std::make_pair("Game", game));

		Hierarchy* mHierarchy = new Hierarchy();
		mWidgets.insert(std::make_pair("Hierarchy", mHierarchy));

		Gizmo* gizmo = new Gizmo();
		mWidgets.insert(std::make_pair("Gizmo", gizmo));

		OutLiner* outLiner = new OutLiner();
		mWidgets.insert(std::make_pair("OutLiner", outLiner));

		Inspector* inspector = new Inspector();
		mWidgets.insert(std::make_pair("Inspector", inspector));

		Project* project = new Project();
		mWidgets.insert(std::make_pair("Project", project));
		
		Console* console = new Console();
		mWidgets.insert(std::make_pair("Console", console));
		console->Initialize();

		ListWidget* list = new ListWidget();
		mWidgets.insert(std::make_pair("ListWidget", list));

		SceneSaveWindow* saveWindow = new SceneSaveWindow();
		mWidgets.insert(std::make_pair("SceneSaveWindow", saveWindow));
		saveWindow->Initialize();

		ObjectWindow* objectWindow = new ObjectWindow();
		mWidgets.insert(std::make_pair("ObjectWindow", objectWindow));
		objectWindow->Initialize();


		//���� �ε� (�⺻ imgui.ini Null �� ������)
		ImGui::LoadIniSettingsFromDisk("../../Custom_ini.ini");
	}

	void WidgetMgr::Release()
	{

		//// ImGui ���� �����ϰ� ������ ���
		//ImGuiIO& io = ImGui::GetIO();
		//io.IniFilename = "../../Custom_ini.ini";
		//ImGui::SaveIniSettingsToDisk(io.IniFilename);

		ImGui_Release();
		
		for (auto iter : mWidgets)
		{
			delete iter.second;
			iter.second = nullptr;
		}

		mWidgets.clear();

		delete mVisualEditor;
		mVisualEditor = nullptr;

	}

	void WidgetMgr::Run()
	{
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

		if (mbForceReset)
		{
			GetWidget<Hierarchy>("Hierarchy")->SetTargetObject(mTargetObj);
			GetWidget<Hierarchy>("Hierarchy")->InitializeScene();
			mbForceReset = false;
		}

		mVisualEditor->Render();
		for (auto iter : mWidgets)
		{
			//std::string debugName = iter.second->GetName();
			iter.second->Render();
		}

		ImGui_Run();

	}

	void WidgetMgr::ImGui_Initialize()
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
		ImGui_ImplDX11_Init(GetDevice()->GetID3D11Device()
			, GetDevice()->GetDeviceContext().Get());


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

	void WidgetMgr::ImGui_Run()
	{
		bool show_demo_window = false;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		ImGuiIO io = ImGui::GetIO();

		//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

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

		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void WidgetMgr::ImGui_Release()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}