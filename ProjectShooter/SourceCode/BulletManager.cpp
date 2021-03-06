#include "BulletManager.h"
#include "RawInput.h"

#include "Singleton.h"

//弾の当たり判定の大きさ.
const float g_fBulletRadius = 0.5f;

//弾の大きさ.
const float g_fBulletScale = 0.5f;

BulletManager::BulletManager()
{
	for (unsigned int i = 0; i < g_iBulletMax; i++)
	{
		m_pBullet[i] = new Bullet(g_fBulletRadius, g_fBulletScale);
	}

	D3DXMatrixIdentity(&m_mShotMoveMat);
	CrearVECTOR3(m_vShotPosition);

	m_pEnemy = nullptr;
}

BulletManager::~BulletManager()
{
	for (unsigned int i = 0; i < g_iBulletMax; i++)
	{
		SAFE_DELETE_ARRAY(m_pBullet[i]);
	}
}

//更新.
void BulletManager::Update(const D3DXMATRIX mShotMoveMat, const D3DXVECTOR3 vShotPosition, Enemy* const pEnemy)
{
	SetShotMoveMat(mShotMoveMat);
	SetShotPosition(vShotPosition);

	m_pEnemy = pEnemy;

	for (unsigned int i = 0; i < g_iBulletMax; i++)
	{
		m_pBullet[i]->Update(vShotPosition);
	}
}

//描画.
void BulletManager::Render(const D3DXMATRIX &mView, const D3DXMATRIX &mProj, const D3DXVECTOR3 &vEye)
{
	for (unsigned int i = 0; i < g_iBulletMax; i++)
	{
		m_pBullet[i]->Render(mView, mProj, vEye);
	}
}

//弾の当たり判定.
void BulletManager::CollisionJudgmentBullet(const SPHERE TargetSphere, clsDX9Mesh* const pTargetMesh)
{
	for (unsigned int i = 0; i < g_iBulletMax; i++)
	{
		//発射されていない場合は次の判定へ.
		if (!m_pBullet[i]->IsShotFlg())
		{
			continue;
		}
		else
		{
			//弾と対象のSphereが衝突しているか.
			if (m_pBullet[i]->BulletCollision(TargetSphere))
			{
				//衝突.
				m_pBullet[i]->BulletHitToSphere();
				m_pEnemy->HitToSphere();
			}

			//弾のレイとメッシュが衝突しているか.
			if (m_pBullet[i]->IsRayHit(pTargetMesh))
			{
				//衝突.
				m_pBullet[i]->BulletHitToMesh();
			}
		}
	}
}

//弾の発射.
bool BulletManager::IsShot()
{
	if (!Singleton<RawInput>().GetInstance().IsLButtonDown())
	{
		return false;
	}

	for (unsigned int i = 0; i < g_iBulletMax; i++)
	{
		//発射されている場合は次へ.
		if (m_pBullet[i]->IsShotFlg())
		{
			continue;
		}
		else
		{
			//弾の発射位置を少し上にあげる.
			const float fAdjustmentShotPosY = 1.0f;

			m_pBullet[i]->BulletShot(m_mShotMoveMat, { m_vShotPosition.x, m_vShotPosition.y + fAdjustmentShotPosY, m_vShotPosition.z });
			return true;
		}
	}

	return false;
}