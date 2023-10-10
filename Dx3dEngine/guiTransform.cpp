#include "guiTransform.h"
#include "Transform.h"
#include "ImGuizmo.h"
#include "guiWidgetMgr.h"
#include "guiGizmo.h"

namespace gui
{
	GUITransform::GUITransform()
		: GUIComponent(eComponentType::Transform)
	{
		SetName("Transform");
		SetSize(ImVec2(200.0f, 120.0f));
	}

	GUITransform::~GUITransform()
	{

	}

	void GUITransform::FixedUpdate()
	{
		GUIComponent::FixedUpdate();


		if (GetTarget() == nullptr)
			return;

		Transform* tr = GetTarget()->GetComponent<Transform>();

		mPosisition = tr->GetPosition();
		mRotation = tr->GetRotation();
		mScale = tr->GetScale();
	}

	void GUITransform::Update()
	{
		GUIComponent::Update();

		ImGui::Text("Position"); ImGui::SameLine();
		ImGui::InputFloat3("##Position", (float*)&mPosisition);

		ImGui::Text("Rotation"); ImGui::SameLine();
		ImGui::InputFloat3("##Rotation", (float*)&mRotation);

		ImGui::Text("Scale"); ImGui::SameLine();
		ImGui::InputFloat3("##Scale", (float*)&mScale);

		if (GetTarget())
		{
			Transform* tr = GetTarget()->GetComponent<Transform>();

			tr->SetPosition(mPosisition);
			tr->SetRotation(mRotation);
			tr->SetScale(mScale);
		}
	}

	void GUITransform::LateUpdate()
	{
		GUIComponent::LateUpdate();

	}
    
    void GUITransform::EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition)
    {
        static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
        static bool useSnap = false;
        static float snap[3] = { 1.f, 1.f, 1.f };
        static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
        static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
        static bool boundSizing = false;
        static bool boundSizingSnap = false;

        Gizmo* gizmo = GETSINGLE(WidgetMgr)->GetWidget<Gizmo>("Gizmo");
        if (gizmo == nullptr)
            return;

        ImGuizmo::OPERATION GizmoOperation = gizmo->GetGizmoOperation();

        if (editTransformDecomposition)
        {
            if (ImGui::RadioButton("Translate", GizmoOperation == ImGuizmo::TRANSLATE))
                GizmoOperation = ImGuizmo::TRANSLATE;
            ImGui::SameLine();
            if (ImGui::RadioButton("Rotate", GizmoOperation == ImGuizmo::ROTATE))
                GizmoOperation = ImGuizmo::ROTATE;
            ImGui::SameLine();
            if (ImGui::RadioButton("Scale", GizmoOperation == ImGuizmo::SCALE))
                GizmoOperation = ImGuizmo::SCALE;
            if (ImGui::RadioButton("Universal", GizmoOperation == ImGuizmo::UNIVERSAL))
                GizmoOperation = ImGuizmo::UNIVERSAL;
            float matrixTranslation[3], matrixRotation[3], matrixScale[3];
            ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
            ImGui::InputFloat3("Tr", matrixTranslation);
            ImGui::InputFloat3("Rt", matrixRotation);
            ImGui::InputFloat3("Sc", matrixScale);
            ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

            if (GizmoOperation != ImGuizmo::SCALE)
            {
                if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
                    mCurrentGizmoMode = ImGuizmo::LOCAL;
                ImGui::SameLine();
                if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
                    mCurrentGizmoMode = ImGuizmo::WORLD;
            }
            if (ImGui::IsKeyPressed(ImGuiKey_S))
                useSnap = !useSnap;
            ImGui::Checkbox("##UseSnap", &useSnap);
            ImGui::SameLine();

            switch (GizmoOperation)
            {
            case ImGuizmo::TRANSLATE:
                ImGui::InputFloat3("Snap", &snap[0]);
                break;
            case ImGuizmo::ROTATE:
                ImGui::InputFloat("Angle Snap", &snap[0]);
                break;
            case ImGuizmo::SCALE:
                ImGui::InputFloat("Scale Snap", &snap[0]);
                break;
            }
            ImGui::Checkbox("Bound Sizing", &boundSizing);
            if (boundSizing)
            {
                ImGui::PushID(3);
                ImGui::Checkbox("##BoundSizing", &boundSizingSnap);
                ImGui::SameLine();
                ImGui::InputFloat3("Snap", boundsSnap);
                ImGui::PopID();
            }
        }

        /*ImGuiIO& io = ImGui::GetIO();
        float viewManipulateRight = io.DisplaySize.x;
        float viewManipulateTop = 0;
        static ImGuiWindowFlags gizmoWindowFlags = 0;
        if (useWindow)
        {
            ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Appearing);
            ImGui::SetNextWindowPos(ImVec2(400, 20), ImGuiCond_Appearing);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));
            ImGui::Begin("Gizmo", 0, gizmoWindowFlags);
            ImGuizmo::SetDrawlist();
            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
            viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
            viewManipulateTop = ImGui::GetWindowPos().y;
            ImGuiWindow* window = ImGui::GetCurrentWindow();
            gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;
        }
        else
        {
            ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
        }

        ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
        ImGuizmo::DrawCubes(cameraView, cameraProjection, &objectMatrix[0][0], gizmoCount);
        ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

        ImGuizmo::ViewManipulate(cameraView, camDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

        if (useWindow)
        {
            ImGui::End();
            ImGui::PopStyleColor(1);
        }*/
    }
}
