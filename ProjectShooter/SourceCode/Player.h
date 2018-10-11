#pragma once
#pragma warning( disable:4005 )

#include "Character.h"

#include "BulletManager.h"
#include "CEffects.h"

//Hp�̍ő�l.
const int g_iPlayerHpMax = 10;

class Player : public Character
{
public:
	Player(clsD3DXSKINMESH* const pModel, BulletManager* const pBulletManager);
	virtual ~Player();

	//�w�肵���ꏊ�̕����Ƀ��f������������.
	void DecideTargetDirection(const D3DXVECTOR3 &vPos)
	{
		m_fTargetDirection = atan2f(m_vPos.x - vPos.x, m_vPos.z - vPos.z);
	}

	SPHERE GetCollisionSphere() const
	{
		return m_Collision;
	}

	bool IsDeadFlg() const
	{
		return m_State.bDead;
	}

	int GetHpMax() const
	{
		return g_iPlayerHpMax;
	}
	int GetHp() const
	{
		return m_State.iHp;
	}

	//���C�ƃ��b�V���̏Փˎ�.
	void RayHitToMesh(clsDX9Mesh* const pTarget);

	//Sphere�Ƃ̏Փˎ�.
	void HitToSphere(SPHERE const TargetSphere);

private:
	bool m_bPushedMoveButtonFlg;

	//���f���Ɍ�������������.
	float m_fTargetDirection;

	//�ړ��O�̈ʒu.
	D3DXVECTOR3 m_vOldPos;

	BulletManager* m_pBulletManager;

	//�G�t�F�N�g�֘A.
	clsEffects* m_pEffect;
	//�n���h��.
	::Effekseer::Handle m_HitHandle;
	::Effekseer::Handle m_DeadHandle;
	::Effekseer::Handle m_WallHitHandle;

	void UpdateProduct();

	void Wait();

	void Move();

	void Attack();

	void Hit();

	void Dead();

	//�A�N�V�����̐؂�ւ�.
	void SetAction(const enAction Action);

	//�ړ��̓��͂��󂯕t���邩�ǂ���.
	void AcceptedMoveButton();

	//�ړ�����.
	void MoveLimit();

	//�U���̓��͂��󂯕t���邩�ǂ���.
	void AcceptedAttackButton();
};