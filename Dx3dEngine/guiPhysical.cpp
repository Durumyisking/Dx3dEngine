#include "guiPhysical.h"
#include "Transform.h"
#include "ImGuizmo.h"
#include "guiGizmo.h"
#include "guiOutLiner.h"

#include "guiWidgetMgr.h"
#include "Physical.h"
#include "PhysXRigidBody.h"
#include "PhysXCollider.h"
#include "PhysicalMovement.h"

const char* charActorType[(int)eActorType::End] =
{
    "Static", // 정적인 물체 (물리적으로 움직이지 않을 물체)
    "Dynamic", // 동적인 물체 (물리엔진에 직접적인 영향을 받는다)
    "Kinematic", // 프로그래밍 운동제어 물리 시뮬레이션 영향을 받지 않고 스크립트로 움직인다.
    "Character", // 일반적인 액터들과 다른 움직임을 처리하기 위해 사용한다. (컨트롤러를 붙여줘야한다)
    "Monster", // 커스텀
};

const char* charGeometryType[(int)eGeometryType::End] =
{
    "Box",
    "Capsule",
    "Sphere",
    "Plane",
    "ConvexMesh",
    "TriangleMesh",
};

namespace gui
{
    GUIPhysical::GUIPhysical()
        : GUIComponent(eComponentType::Physical)
        , mActorType(eActorType::End)
        , mGeometryType(eGeometryType::End)
        , mScale(Vector3::Zero)
    {
        SetName("Physical");
        SetSize(ImVec2(300.0f, 200.0f));
    }

    GUIPhysical::~GUIPhysical()
    {

    }

    void GUIPhysical::FixedUpdate()
    {
        GUIComponent::FixedUpdate();
    }

    void GUIPhysical::Update()
    {
        GUIComponent::Update();

        if (GetTarget() == nullptr)
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
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.6f));
        ImGui::Button(actorName.c_str());
        ImGui::PopStyleColor(1);
        ImGui::PopID();


        if (mAddingPhysical)
        {
            if (ImGui::TreeNodeEx("Select Actor :", ImGuiTreeNodeFlags_DefaultOpen)) // 메인 트리
            {
                for (int i = 0; i < static_cast<int>(eActorType::End); i++)
                {
                    if (ImGui::TreeNodeEx((void*)(intptr_t)i
                        , (mActorType == static_cast<eActorType>(i) ? ImGuiTreeNodeFlags_Selected : 0)
                        , charActorType[i]))
                    {
                        if (ImGui::IsItemClicked())
                        {
                            mActorType = static_cast<eActorType>(i);
                        }
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
        }

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
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.6f));
        ImGui::Button(geomName.c_str());
        ImGui::PopStyleColor(1);
        ImGui::PopID();


        if (mAddingPhysical)
        {
            if (ImGui::TreeNodeEx("Select Geometry :", ImGuiTreeNodeFlags_DefaultOpen)) // 메인 트리
            {
                for (int i = 0; i < static_cast<int>(eGeometryType::End); i++)
                {
                    if (ImGui::TreeNodeEx((void*)(intptr_t)i
                        , (mGeometryType == static_cast<eGeometryType>(i) ? ImGuiTreeNodeFlags_Selected : 0)
                        , charGeometryType[i]))
                    {
                        if (ImGui::IsItemClicked())
                        {
                            mGeometryType = static_cast<eGeometryType>(i);
                        }
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
        }

        if (mAddingPhysical)
        {
            ImGui::InputFloat3("#ColliderScale", (float*)&mScale);


            if(ImGui::Button("Create", ImVec2(120.f, 60.f)))
            {
                AddPhysical();
            }
        }
    }

    void GUIPhysical::LateUpdate()
    {
        GUIComponent::LateUpdate();

    }

    void GUIPhysical::Initialize()
    {
        if (GetTarget() == nullptr)
            return;

        Physical* physical = GetTarget()->GetComponent<Physical>();

        if (physical == nullptr)
        {
            mActorType = eActorType::End;
            mGeometryType = eGeometryType::End;
        }
        else
        {
            mActorType = physical->GetActorType();
            mGeometryType = physical->GetGeometryType();
        }
    }

    bool GUIPhysical::AddPhysical()
    {
        GameObj* target = GetTarget();
        Physical* physical = target->AddComponent<Physical>(eComponentType::Physical);


        physical->InitialDefaultProperties(mActorType, mGeometryType, mScale);

        target->AddComponent<PhysXRigidBody>(eComponentType::RigidBody)->Initialize();
        target->AddComponent<PhysXCollider>(eComponentType::Collider)->Initialize();

        physical->Initialize();

        if (mActorType != eActorType::Static)
        {
            target->AddComponent<PhysicalMovement>(eComponentType::Movement);
        }


        GETSINGLE(WidgetMgr)->GetWidget<OutLiner>("OutLiner")->InitializeTargetGameObject();

        return true;
    }

    void GUIPhysical::AddingPhysical(bool tf)
    {
        mAddingPhysical = tf; 

        if (mAddingPhysical)
        {
            mScale = Vector3::Zero;
            SetSize(ImVec2(300.0f, 400.0f));
        }
        else
        {
            SetSize(ImVec2(300.0f, 200.0f));
        }
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
