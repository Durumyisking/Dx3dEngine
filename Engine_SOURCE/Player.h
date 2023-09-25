#pragma once
#include "DynamicObject.h"
#include "MarioParts.h"
#include "MarioCap.h"

class Player :
	public DynamicObject
{
public:
	enum class ePlayerState : short
	{
		Idle,

		Move,
		Jump,
		Squat, //��ũ����
		SquatMove,

		Air, //���߿� ������ �ٸ� �׼��� ���� �ʴ� ����
		Fall, //�������� ��
		Wall, //������

		Hit,
		Groggy,

		ThrowCap,
		Die,
	};

public:
	Player();
	virtual ~Player();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;
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

private:
	ePlayerState mPlayerState;


};
