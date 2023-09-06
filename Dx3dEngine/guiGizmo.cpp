#include "guiGizmo.h"
#include "Camera.h"
#include "Renderer.h"
#include "Object.h"

#include "InputMgr.h"
#include "guiEditorObjMgr.h"
#include "guiWidgetMgr.h"

namespace gui
{
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

    Gizmo::Gizmo()
        : Widget()
        , mGizmoCount(0)
        , mbisOrthographic(false)
        , mOrthographicViewWidth(10.f)
        , mCamera(nullptr)
		, mTargetGameObject(nullptr)
	{
	}
	Gizmo::~Gizmo()
	{
	}

    void Gizmo::Initialize()
    {
    }

    void Gizmo::FixedUpdate()
    {
        if (KEY_TAP(Z))
        {
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        }
        if (KEY_TAP(X))
        {
            //mCurrentGizmoOperation = ImGuizmo::ROTATE;
        }
        if (KEY_TAP(C))
        {
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        }
    }

    void Gizmo::Update()
    {
        mTargetGameObject = GETSINGLE(WidgetMgr)->GetHierachyTargetObject();

        if (mTargetGameObject == nullptr)
            return;

        mCamera = renderer::mainCamera;

        if (mCamera == nullptr)
            return;

        //ImGuizmo::SetOrthographic(false);
        //ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());

        Transform* tr = mTargetGameObject->GetComponent<Transform>();
        if (tr == nullptr)
            return;

        constexpr float kOffsetX = 60.f;
        constexpr float kOffsetY = 45.f;
        const float		x = ImGui::GetWindowPos().x - 60;
        const float		y = ImGui::GetWindowPos().y - 45;
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

        ImGuizmo::Manipulate(&view.m[0][0], &projection.m[0][0], mCurrentGizmoOperation, ImGuizmo::WORLD, &worldMatrix.m[0][0]);

        if (ImGuizmo::IsUsing())
        {
            Vector3 position{};
            Quaternion rotation{};
            Vector3 scale{};
            worldMatrix.Decompose(scale, rotation, position);

            if (mCurrentGizmoOperation == ImGuizmo::TRANSLATE)
            {
                tr->SetPosition(position);
            }
            else if (mCurrentGizmoOperation == ImGuizmo::ROTATE)
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
            else if (mCurrentGizmoOperation == ImGuizmo::SCALE)
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

    //void EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition)
    //{
    //    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
    //    static bool useSnap = false;
    //    static float snap[3] = { 1.f, 1.f, 1.f };
    //    static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
    //    static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
    //    static bool boundSizing = false;
    //    static bool boundSizingSnap = false;

    //    if (editTransformDecomposition)
    //    {
    //        if (ImGui::IsKeyPressed(ImGuiKey_T))
    //            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    //        if (ImGui::IsKeyPressed(ImGuiKey_E))
    //            mCurrentGizmoOperation = ImGuizmo::ROTATE;
    //        if (ImGui::IsKeyPressed(ImGuiKey_R)) // r Key
    //            mCurrentGizmoOperation = ImGuizmo::SCALE;
    //        if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
    //            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    //        ImGui::SameLine();
    //        if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
    //            mCurrentGizmoOperation = ImGuizmo::ROTATE;
    //        ImGui::SameLine();
    //        if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
    //            mCurrentGizmoOperation = ImGuizmo::SCALE;
    //        if (ImGui::RadioButton("Universal", mCurrentGizmoOperation == ImGuizmo::UNIVERSAL))
    //            mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;
    //        float matrixTranslation[3], matrixRotation[3], matrixScale[3];
    //        ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
    //        ImGui::InputFloat3("Tr", matrixTranslation);
    //        ImGui::InputFloat3("Rt", matrixRotation);
    //        ImGui::InputFloat3("Sc", matrixScale);
    //        ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

    //        if (mCurrentGizmoOperation != ImGuizmo::SCALE)
    //        {
    //            if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
    //                mCurrentGizmoMode = ImGuizmo::LOCAL;
    //            ImGui::SameLine();
    //            if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
    //                mCurrentGizmoMode = ImGuizmo::WORLD;
    //        }
    //        if (ImGui::IsKeyPressed(ImGuiKey_S))
    //            useSnap = !useSnap;
    //        ImGui::Checkbox("##UseSnap", &useSnap);
    //        ImGui::SameLine();

    //        switch (mCurrentGizmoOperation)
    //        {
    //        case ImGuizmo::TRANSLATE:
    //            ImGui::InputFloat3("Snap", &snap[0]);
    //            break;
    //        case ImGuizmo::ROTATE:
    //            ImGui::InputFloat("Angle Snap", &snap[0]);
    //            break;
    //        case ImGuizmo::SCALE:
    //            ImGui::InputFloat("Scale Snap", &snap[0]);
    //            break;
    //        }
    //        ImGui::Checkbox("Bound Sizing", &boundSizing);
    //        if (boundSizing)
    //        {
    //            ImGui::PushID(3);
    //            ImGui::Checkbox("##BoundSizing", &boundSizingSnap);
    //            ImGui::SameLine();
    //            ImGui::InputFloat3("Snap", boundsSnap);
    //            ImGui::PopID();
    //        }
    //    }

    //    ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
    //    ImGuizmo::DrawCubes(cameraView, cameraProjection, &objectMatrix[0][0], gizmoCount);
    //    ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

    //    ImGuizmo::ViewManipulate(cameraView, camDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

    //    ImGui::End();
    //    ImGui::PopStyleColor(1);
    //}

    void Frustum(float left, float right, float bottom, float top, float znear, float zfar, float* m16)
    {
        float temp, temp2, temp3, temp4;
        temp = 2.0f * znear;
        temp2 = right - left;
        temp3 = top - bottom;
        temp4 = zfar - znear;
        m16[0] = temp / temp2;
        m16[1] = 0.0;
        m16[2] = 0.0;
        m16[3] = 0.0;
        m16[4] = 0.0;
        m16[5] = temp / temp3;
        m16[6] = 0.0;
        m16[7] = 0.0;
        m16[8] = (right + left) / temp2;
        m16[9] = (top + bottom) / temp3;
        m16[10] = (-zfar - znear) / temp4;
        m16[11] = -1.0f;
        m16[12] = 0.0;
        m16[13] = 0.0;
        m16[14] = (-temp * zfar) / temp4;
        m16[15] = 0.0;
    }

    void Perspective(float fovyInDegrees, float aspectRatio, float znear, float zfar, float* m16)
    {
        float ymax, xmax;
        ymax = znear * tanf(fovyInDegrees * 3.141592f / 180.0f);
        xmax = ymax * aspectRatio;
        Frustum(-xmax, xmax, -ymax, ymax, znear, zfar, m16);
    }

    void OrthoGraphic(const float l, float r, float b, const float t, float zn, const float zf, float* m16)
    {
        m16[0] = 2 / (r - l);
        m16[1] = 0.0f;
        m16[2] = 0.0f;
        m16[3] = 0.0f;
        m16[4] = 0.0f;
        m16[5] = 2 / (t - b);
        m16[6] = 0.0f;
        m16[7] = 0.0f;
        m16[8] = 0.0f;
        m16[9] = 0.0f;
        m16[10] = 1.0f / (zf - zn);
        m16[11] = 0.0f;
        m16[12] = (l + r) / (l - r);
        m16[13] = (t + b) / (b - t);
        m16[14] = zn / (zn - zf);
        m16[15] = 1.0f;
    }

}