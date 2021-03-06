#pragma once
#pragma warning( disable:4005 )

#include "Character.h"

#include "CEffects.h"

//Hpの最大値.
const int g_iEnemyHpMax = 5;

class Enemy : public Character
{
public:
	Enemy(clsD3DXSKINMESH* const pModel);
	virtual ~Enemy();

	//指定した場所の方向にモデルを向かせる.
	void DecideTargetDirection(const D3DXVECTOR3 &vPos)
	{
		m_vPlayerPosition = vPos;
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
		return g_iEnemyHpMax;
	}
	int GetHp() const
	{
		return m_State.iHp;
	}

	//レイとメッシュの衝突時.
	void RayHitToMesh(clsDX9Mesh* const pTarget);

	//Sphereとの衝突時.
	void HitToSphere();

private:
	//モデルに向かせたい方向.
	float m_fTargetDirection;

	//プレイヤーの位置.
	D3DXVECTOR3 m_vPlayerPosition;

	float m_fAttackDistance;

	//エフェクト関連.
	clsEffects* m_pEffect;
	//ハンドル.
	::Effekseer::Handle m_HitHandle;
	::Effekseer::Handle m_DeadHandle;

	void UpdateProduct();

	void Wait();

	void Move();

	void Attack();

	void Hit();

	void Dead();

	//アクションの切り替え.
	void SetAction(const enAction Action);
};