#pragma once
#include "Component.h"

namespace dru
{
	using namespace math;

	enum class eProjectionType
	{
		Perspective,
		Orthographic,
		End,
	};


	class Camera : public Component
	{
	public:

		__forceinline static Matrix& GetGpuViewMatrix() { return View; }
		__forceinline static Matrix& GetGpuProjectionMatrix() { return Projection; }
		__forceinline static void SetGpuViewMatrix(Matrix view) { View = view; }
		__forceinline static void SetGpuProjectionMatrix(Matrix projection) { Projection = projection; }

		Camera();
		virtual ~Camera();

		virtual void Initialize() override;
		virtual void update() override;
		virtual void fixedUpdate() override;
		virtual void render() override;

		void CreateViewMatrix();
		void CreateProjectionMatrix();
		
		void RegisterCameraInRenderer();

		void TurnLayerMask(eLayerType _layer, bool _enable = true);
		void EnableLayerMasks() { mLayerMask.set(); } // 전부다 true로 }
		void DisableLayerMasks() { mLayerMask.reset(); }

		void SetProjectionType(eProjectionType _Type) { mType = _Type; }
		eProjectionType GetProjectionType() { return mType; }

		float GetScale() const { return mScale; }

		Matrix& GetViewMatrix() { return mView; }
		Matrix& GetProjectionMatrix() { return mProjection; }


		void SetTarget(GameObj* _Target);
		GameObj* GetTarget() const { return mTargetObj; }

		float GetCamSpeed() const { return mCamSpeed; }

		float GetFarDistFromTarget() const { return mFarDist; }
		Vector3 GetCamDir() const { return mCamDir; }

		void SmoothOn() { mSmooth = true; }
		void SmoothOff() { mSmooth = false; }

	private:
		void sortGameObjects();
		void renderOpaque();
		void renderCutout();
		void renderTransparent();
		
		void pushGameObjectToRenderingModes(GameObj* obj);
		bool renderPassCheck(GameObj* _obj);


	private:
		static Matrix View;
		static Matrix Projection; // 모든 obj들의 해당 행렬은 동일함

		Matrix mView;
		Matrix mProjection;


		eProjectionType mType;
		float mAspectRatio; // 종횡비

		float mNear;
		float mFar;
		float mScale;

		std::bitset<static_cast<UINT>(eLayerType::End)> mLayerMask;
		std::vector<GameObj*> mOpaqueGameObjects;
		std::vector<GameObj*> mCutoutGameObjects;
		std::vector<GameObj*> mTransparentGameObjects;
		std::vector<GameObj*> mPostProcessGameObjects;

		GameObj*	mTargetObj;
		Vector3		mCamDir;

		float		mFarDist;

		float		mCamSpeed;

		float		mTime;

		bool		mSmooth;
	};


}