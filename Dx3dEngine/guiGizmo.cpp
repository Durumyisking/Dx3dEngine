#include "guiGizmo.h"
#include "Camera.h"
#include "Renderer.h"
#include "Object.h"
#include "Physical.h"
#include "InputMgr.h"

#include "guiEditorObjMgr.h"
#include "guiWidgetMgr.h"
#include "guiVisualEditor.h"
#include "guiHierarchy.h"

namespace gui
{
    Gizmo::Gizmo()
        : Widget()
        , mGizmoOperation(ImGuizmo::TRANSLATE)
        , mbisOrthographic(false)
        , mOrthographicViewWidth(10.f)
        , mCamera(nullptr)
		, mTargetGameObject(nullptr)
	{
        SetName("Gizmo");
	}
	Gizmo::~Gizmo()
	{
	}

    void Gizmo::Initialize()
    {
    }

    void Gizmo::FixedUpdate()
    {
        mWindow_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove 
            | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse 
            | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings;

        if (KEY_TAP(Z))
        {
            mGizmoOperation = ImGuizmo::TRANSLATE;
        }
        if (KEY_TAP(X))
        {
            //mGizmoOperation = ImGuizmo::ROTATE;
        }
        if (KEY_TAP(C))
        {
            mGizmoOperation = ImGuizmo::SCALE;
        }

    }

    void Gizmo::Update()
    {
        mTargetGameObject = GETSINGLE(WidgetMgr)->GetHierachy()->GetTargetObject();

        if (mTargetGameObject == nullptr)
            return;

        mCamera = renderer::mainCamera;

        if (mCamera == nullptr)
            return;

        ImGuizmo::SetOrthographic(mbisOrthographic);
        ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());

        Transform* tr = mTargetGameObject->GetComponent<Transform>();
        if (tr == nullptr)
            return;

        math::Vector3 objPos;
        bool isPhysical = false;

        if (mTargetGameObject->GetComponent<Physical>() != nullptr)
        {
            objPos = mTargetGameObject->GetComponent<Transform>()->GetPhysicalPosition();
            isPhysical = true;
        }
        else
        {
            objPos = mTargetGameObject->GetComponent<Transform>()->GetPosition();
            isPhysical = false;
        }



        constexpr float offsetX = 60.f;
        constexpr float offsetY = 45.f;
        const float		x = ImGui::GetWindowPos().x - offsetX;
        const float		y = ImGui::GetWindowPos().y - offsetY;
        const float		width = ImGui::GetWindowViewport()->Size.x;
        const float		height = ImGui::GetWindowViewport()->Size.y;


        ImGuizmo::SetRect(x, y, width, height);

        Matrix view = mCamera->GetViewMatrix();
        Matrix projection = mCamera->GetProjectionMatrix();
        Matrix worldMatrix = tr->GetWorldMatrix();

        float matrixTranslation[3], matrixRotation[3], matrixScale[3];
        ImGuizmo::DecomposeMatrixToComponents(&worldMatrix.m[0][0], matrixTranslation, matrixRotation, matrixScale);

        Matrix matTranslation = Matrix::CreateTranslation(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
        Matrix matRotation = Matrix::CreateFromYawPitchRoll(matrixRotation[1], matrixRotation[0], matrixRotation[2]);
        Matrix matScale = Matrix::CreateScale(matrixScale[0], matrixScale[1], matrixScale[2]);

        worldMatrix = matScale * matRotation * matTranslation;

        ImGuizmo::Manipulate(&view.m[0][0], &projection.m[0][0], mGizmoOperation, ImGuizmo::WORLD, &worldMatrix.m[0][0]);

        if (ImGuizmo::IsUsing())
        {
            Vector3 position{};
            Quaternion rotation{};
            Vector3 scale{};
            worldMatrix.Decompose(scale, rotation, position);

            if (mGizmoOperation == ImGuizmo::TRANSLATE)
            {
                if(isPhysical)
                    tr->SetPhysicalPosition(objPos);
                else
                    tr->SetPosition(position);
            }
            else if (mGizmoOperation == ImGuizmo::ROTATE)
            {
                // 회전 작동 오류있음
                // 현재사용 X
                Matrix mat = Matrix::CreateFromQuaternion(rotation);
                float x, y, z;

                if (mat._13 > 0.998f)
                {
                    y = atan2f(mat._21, mat._22);
                    x = XM_PI / 2.0f;
                    z = 0;
                }
                else if (mat._13 < -0.998f)
                {
                    y = atan2f(mat._21, mat._22);
                    x = -XM_PI / 2.0f;
                    z = 0;
                }
                else
                {
                    y = atan2f(-mat._23, mat._33);
                    x = asinf(mat._13);
                    z = atan2f(-mat._12, mat._11);
                }

                Vector3 axisRotation(x, y, z);
                tr->SetRotation(axisRotation);

            }
            else if (mGizmoOperation == ImGuizmo::SCALE)
            {
                tr->SetScale(scale);
            }
            else
            {
                return;
            }
        }
    }

    void Gizmo::LateUpdate()
    {
        Widget::LateUpdate();
    }

    void Gizmo::Render()
    {
        Widget::Render();
    }

    void Gizmo::Close()
    {
    }

	void Gizmo::ClearTarget()
	{
	}

    void Gizmo::InitializeTargetGameObject()
    {
    }
}