#pragma once
#include "Component.h"

namespace dru
{
	using namespace math;
	class CCameraScript;

	enum class eProjectionType
	{
		Perspective,
		Orthographic,
		End,
	};


	class CCamera : public CComponent
	{
		friend class CCameraScript;
	public:

		__forceinline static Matrix& GetGpuViewMatrix() { return View; }
		__forceinline static Matrix& GetGpuProjectionMatrix() { return Projection; }
		__forceinline static void SetGpuViewMatrix(Matrix view) { View = view; }
		__forceinline static void SetGpuProjectionMatrix(Matrix projection) { Projection = projection; }

		CCamera();
		virtual ~CCamera();

		virtual void Initialize() final;
		virtual void update() final;
		virtual void fixedUpdate() final;
		virtual void render() final;

		void CreateViewMatrix();
		void CreateProjectionMatrix();
		
		void RegisterCameraInRenderer();

		void TurnLayerMask(eLayerType layer, bool enable = true);
		void EnableLayerMasks() { mLayerMask.set(); } // 전부다 true로 }
		void DisableLayerMasks() { mLayerMask.reset(); }

		void SetProjectionType(eProjectionType type) { mType = type; }
		eProjectionType GetProjectionType() { return mType; }

		float GetScale() const { return mScale; }

		Matrix& GetViewMatrix() { return mView; }
		Matrix& GetProjectionMatrix() { return mProjection; }


<<<<<<< Updated upstream
		void SetTarget(CGameObj* _Target);
		CGameObj* GetTarget() const { return mTargetObj; }
=======
		void SetTarget(GameObj* target);
		GameObj* GetTarget() const { return mTargetObj; }
>>>>>>> Stashed changes

		void SmoothOn() { mSmooth = true; }
		void SmoothOff() { mSmooth = false; }

		CCameraScript* GetCamScript();

	private:
		void sortGameObjects();
		void renderOpaque();
		void renderCutout();
		void renderTransparent();
		
<<<<<<< Updated upstream
		void pushGameObjectToRenderingModes(CGameObj* obj);
		bool renderPassCheck(CGameObj* _obj);
=======
		void pushGameObjectToRenderingModes(GameObj* obj);
		bool renderPassCheck(GameObj* obj);
>>>>>>> Stashed changes


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

		std::bitset<(UINT)eLayerType::End> mLayerMask;
		std::vector<CGameObj*> mOpaqueGameObjects;
		std::vector<CGameObj*> mCutoutGameObjects;
		std::vector<CGameObj*> mTransparentGameObjects;
		std::vector<CGameObj*> mPostProcessGameObjects;

		CGameObj*	mTargetObj;
		Vector3		mCamDir;

		float		mFarDist;

		float		mCamSpeed;

		float		mTime;

		bool		mSmooth;
	};


}