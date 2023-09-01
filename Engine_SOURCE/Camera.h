#pragma once
#include "Component.h"



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
	__forceinline static Matrix& GetGpuInverseViewMatrix() { return View; }
	__forceinline static Matrix& GetGpuProjectionMatrix() { return Projection; }
	__forceinline static Matrix& GetSkySphereFov() { return SkyFov; }
	__forceinline static void SetGpuViewMatrix(Matrix view) { View = view; }
	__forceinline static void SetGpuProjectionMatrix(Matrix projection) { Projection = projection; }

	Camera();
	virtual ~Camera();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;
	
	void CreateViewMatrix();
	void CreateProjectionMatrix();
		
	void RegisterCameraInRenderer();

	void TurnLayerMask(eLayerType layer, bool enable = true);
	void EnableLayerMasks() { mLayerMask.set(); } // �遺 true濡 }
	void DisableLayerMasks() { mLayerMask.reset(); }

	void SetProjectionType(eProjectionType type) { mType = type; }
	eProjectionType GetProjectionType() { return mType; }

	float GetScale() const { return mScale; }

	const Matrix& GetViewMatrix() { return mView; }
	const Matrix& GetProjectionMatrix() { return mProjection; }

	void SetTarget(GameObj* target);
	GameObj* GetTarget() const { return mTargetObj; }

	float GetCamSpeed() const { return mCamSpeed; }

	float GetFarDistFromTarget() const { return mFarDist; }
	Vector3 GetCamDir() const { return mCamDir; }

	void SmoothOn() { mSmooth = true; }
	void SmoothOff() { mSmooth = false; }

	bool Raycast(const Vector3& origin, const Vector3& dir, GameObj* gameObject, float maxDistance = 100.f);


private:
	void sortGameObjects();
	void renderDeferred();
	void renderOpaque();
	void renderCutout();
	void renderTransparent();
	void renderPostProcess();
		
	void pushGameObjectToRenderingModes(GameObj* obj);
	bool renderPassCheck(GameObj* obj);


private:
	static Matrix View;
	static Matrix InverseView;
	static Matrix Projection; // 모든 obj들의 해당 행렬은 동일함
	static Matrix SkyFov;

	Matrix mView;
	Matrix mProjection;


	eProjectionType mType;
	float mAspectRatio; // 醫〓

	float mNear;
	float mFar;
	float mScale;

	std::bitset<static_cast<UINT>(eLayerType::End)> mLayerMask;
	std::vector<GameObj*> mDeferredOpaqueGameObjects;
	std::vector<GameObj*> mOpaqueGameObjects;
	std::vector<GameObj*> mCutoutGameObjects;
	std::vector<GameObj*> mTransparentGameObjects;
	std::vector<GameObj*> mPostProcessGameObjects;

	PxRaycastHit    mRaycastHit;
	float			mRayMaxDist;
	UINT			mRayMaxHit;

	GameObj*	mTargetObj;
	Vector3		mCamDir;

	float		mFarDist;
	float		mCamSpeed;
	float		mTime;
	bool		mSmooth;
};

