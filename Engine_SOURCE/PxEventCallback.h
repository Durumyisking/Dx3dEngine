#pragma once
#include "Engine.h"


// scene ������ �ݹ����� client���� �������� �ʴ´�

class PxEventCallback
	: public physx::PxSimulationEventCallback
{
public:
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;
	virtual void onWake(PxActor** actors, PxU32 count) override;
	virtual void onSleep(PxActor** actors, PxU32 count) override;
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
	virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;
};

/*
onContact():
�� �޼���� ��ü ���� ������ �߻��� �� ȣ��˴ϴ�.

�� �޼��带 ����Ͽ� ��ü ���� �浹, ����, ���� � ���� ������ ���� �� �ֽ��ϴ�.
�̸� ���� �߰����� �浹 ����, �浹 ����, ������ ���� ���� ������ �� �ֽ��ϴ�.

onConstraintBreak():
�� �޼���� ���� ������ ������ ��� ȣ��˴ϴ�.

���� ���, ������ ���� ����(��ü ���� ����, ����Ʈ ��)�� �Ҿ��������ų� �ı��� �� �߻��մϴ�.
�� �ݹ��� ����Ͽ� ���� ������ ������ ���� �ǵ���� �ްų� �߰� ������ ������ �� �ֽ��ϴ�.

onWake():
�� �޼���� ��ü�� ��� �� ȣ��˴ϴ�.

���� �ùķ��̼ǿ����� ��ü�� ��Ȱ��ȭ�Ͽ� ��� ���ϸ� ���̴� ��찡 �ֽ��ϴ�.
�� �޼��带 ����Ͽ� ��ü�� �ٽ� Ȱ��ȭ�� �� �˸��� �ް�, �ʿ��� �۾��� ������ �� �ֽ��ϴ�.

onSleep():
�� �޼���� ��ü�� ��� �� ȣ��˴ϴ�.

���� �ùķ��̼ǿ����� ��ü�� ���� �ð� ���� �����Ǹ� ��Ȱ��ȭ�Ͽ� ��� ���ϸ� ���� �� �ֽ��ϴ�.
�� �޼��带 ����Ͽ� ��ü�� ������ �� �˸��� �ް�, �ʿ��� �۾��� ������ �� �ֽ��ϴ�.


onTrigger():
�� �޼���� Ʈ����(�浹 ���⸸ ����ǰ� ���� �浹 ������ �߻����� �ʴ� ����)�� Ȱ��ȭ�ǰų� ��Ȱ��ȭ�� �� ȣ��˴ϴ�. 

Ʈ���� ������ ����Ͽ� Ư�� ������ �����ϴ� ��ü ���� ��ȣ�ۿ��� ó���� �� �ֽ��ϴ�.
�� �޼��带 ����Ͽ� Ʈ���� �̺�Ʈ�� ���� �˸��� �ް�, �ʿ��� �۾��� ������ �� �ֽ��ϴ�.


*/

