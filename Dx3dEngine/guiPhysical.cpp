#include "guiPhysical.h"
#include "Transform.h"
#include "ImGuizmo.h"
#include "guiGizmo.h"

#include "guiWidgetMgr.h"
#include "Physical.h"

namespace gui
{
    GUIPhysical::GUIPhysical()
        : GUIComponent(eComponentType::Physical)
        , mActorType(eActorType::End)
        , mGeometryType(eGeometryType::End)
    {
        SetName("Physical");
        SetSize(ImVec2(250.0f, 200.0f));
    }

    GUIPhysical::~GUIPhysical()
    {

    }

    void GUIPhysical::FixedUpdate()
    {
        GUIComponent::FixedUpdate();


        if (GetTarget() == nullptr)
            return;

        Physical* physical = GetTarget()->GetComponent<Physical>();

        if (physical == nullptr)
            return;

        mActorType = physical->GetActorType();
        mGeometryType = physical->GetGeometryType();
    }

    void GUIPhysical::Update()
    {
        GUIComponent::Update();

        if (GetTarget() == nullptr)
            return;

        Physical* physical = GetTarget()->GetComponent<Physical>();

        if (physical == nullptr)
            return;

        //ActorType Edit
        ImGui::Text("ActorType:"); 
        ImGui::SameLine();

        std::string actorName;

        switch (mActorType)
        {
        case enums::eActorType::Static:
            actorName = "Static";
            break;
        case enums::eActorType::Dynamic:
            actorName = "Dynamic";
            break;
        case enums::eActorType::Kinematic:
            actorName = "Kinematic";
            break;
        case enums::eActorType::Character:
            actorName = "Character";
            break;
        case enums::eActorType::Monster:
            actorName = "Monster";
            break;
        default:
            actorName = "None";
            break;
        }


        ImGui::PushID(0);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
        ImGui::Button(actorName.c_str());
        ImGui::PopStyleColor(1);
        ImGui::PopID();

        //GeometryType Edit
        ImGui::Text("GeomType:");
        ImGui::SameLine();

        std::string geomName;

        switch (mGeometryType)
        {
        case enums::eGeometryType::Box:
            geomName = "Box";
            break;
        case enums::eGeometryType::Capsule:
            geomName = "Capsule";
            break;
        case enums::eGeometryType::Sphere:
            geomName = "Sphere";
            break;
        case enums::eGeometryType::Plane:
            geomName = "Plane";
            break;
        case enums::eGeometryType::ConvexMesh:
            geomName = "ConvexMesh";
            break;
        case enums::eGeometryType::TriangleMesh:
            geomName = "TriangleMesh";
            break;
        default:
            geomName = "None";
            break;
        }

        ImGui::PushID(0);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
        ImGui::Button(geomName.c_str());
        ImGui::PopStyleColor(1);
        ImGui::PopID();
    }

    void GUIPhysical::LateUpdate()
    {
        GUIComponent::LateUpdate();

    }

    void GUIPhysical::EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition)
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
    }
}
