#pragma once
#include "DynamicObject.h"
#include "MarioParts.h"
#include "MarioCap.h"

class MeshRenderer;
class PlayerStateScript;
class PhysXRigidBody;

class Player :
	public DynamicObject
{
public:
	enum class ePlayerState : short
	{
		Idle,

		Move,
		Jump,
		Squat, //웅크리기
		SquatMove,

		Air, //공중에 있지만 다른 액션은 하지 않는 상태
		Fall, //떨어지는 중
		Wall, //벽차기

		Hit,
		Groggy,
		
		ThrowCap,
		CatchCap,
		Capture,
		Die,
	};

public:
	Player();
	Player(const Player& Obj);
	virtual ~Player();

	virtual Player* Clone() const;
	virtual void Save(FILE* File) override;
	virtual void Load(FILE* File) override;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;
	virtual void PrevRender() override;
	virtual void FontRender() override;

public:
	virtual void OnCollisionEnter(GameObj* gameObject) override;
	virtual void OnTriggerEnter(GameObj* gameObject) override;
	virtual void OnTriggerExit(GameObj* gameObject) override;
	void KeyCheck();
	virtual void BoneInitialize();
	void boneAnimatorInit(BoneAnimator* animator);

	std::vector<MarioParts*> GetParts() { return mParts; }
	void SetParts(MarioParts* part) { mParts.push_back(part); }
	void SetMarioCap(MarioCap* cap);
	MarioCap* GetMarioCap() const { return mMarioCap; }



private:
	std::vector<MarioParts*> mParts;
	MarioCap* mMarioCap;

protected:
	virtual void stateInfoInitalize() final;

public:
	ePlayerState GetPlayerState() { return mPlayerState; }
	void SetPlayerState(ePlayerState playerState);
	//void PlayerAnimation(std::wstring name);

	bool IsCaptureing();

private:
	ePlayerState mPlayerState;
	MeshRenderer* mMeshRenderer;
	PlayerStateScript* mScript;
	PhysXRigidBody* mRigidBody;
};
