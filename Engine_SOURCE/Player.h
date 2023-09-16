#pragma once
#include "DynamicObject.h"
#include "MarioParts.h"


class Player :
	public DynamicObject
{
public:
	enum class ePlayerState : short
	{
		Idle,

		Move,
		Jump,
		Crouch, //��ũ����
		CrouchMove, 

		Air, //���߿� ������ �ٸ� �׼��� ���� �ʴ� ����
		Wall, //������

		Hit,
		Groggy,

		CapAction,
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

	std::vector<MarioParts*> GetParts() { return mParts; }
	void SetParts(MarioParts* part) { mParts.push_back(part); }


private:
	std::vector<MarioParts*> mParts;

protected:
	virtual void stateInfoInitalize() final;

public:
	ePlayerState GetPlayerState() { return mPlayerState; }
	void SetPlayerState(ePlayerState playerState);
	void PlayerAnimation(std::wstring name);

private:
	ePlayerState mPlayerState;


};
