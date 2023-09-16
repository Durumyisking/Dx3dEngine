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
		Crouch, //웅크리기
		CrouchMove, 

		Air, //공중에 있지만 다른 액션은 하지 않는 상태
		Wall, //벽차기

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
