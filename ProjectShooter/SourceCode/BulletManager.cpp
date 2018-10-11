#include "BulletManager.h"
#include "RawInput.h"

//’e‚Ì“–‚½‚è”»’è‚Ì‘å‚«‚³.
const float g_fBulletRadius = 0.5f;

//’e‚Ì‘å‚«‚³.
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

//XV.
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

//•`‰æ.
void BulletManager::Render(const D3DXMATRIX &mView, const D3DXMATRIX &mProj, const D3DXVECTOR3 &vEye)
{
	for (unsigned int i = 0; i < g_iBulletMax; i++)
	{
		m_pBullet[i]->Render(mView, mProj, vEye);
	}
}

//’e‚Ì“–‚½‚è”»’è.
void BulletManager::CollisionJudgmentBullet(const SPHERE TargetSphere, clsDX9Mesh* const pTargetMesh)
{
	for (unsigned int i = 0; i < g_iBulletMax; i++)
	{
		//”­ŽË‚³‚ê‚Ä‚¢‚È‚¢ê‡‚ÍŽŸ‚Ì”»’è‚Ö.
		if (!m_pBullet[i]->IsShotFlg())
		{
			continue;
		}
		else
		{
			//’e‚Æ‘ÎÛ‚ÌSphere‚ªÕ“Ë‚µ‚Ä‚¢‚é‚©.
			if (m_pBullet[i]->BulletCollision(TargetSphere))
			{
				//Õ“Ë.
				m_pBullet[i]->BulletHitToSphere();
				m_pEnemy->HitToSphere();
			}

			//’e‚ÌƒŒƒC‚ÆƒƒbƒVƒ…‚ªÕ“Ë‚µ‚Ä‚¢‚é‚©.
			if (m_pBullet[i]->IsRayHit(pTargetMesh))
			{
				//Õ“Ë.
				m_pBullet[i]->BulletHitToMesh();
			}
		}
	}
}

//’e‚Ì”­ŽË.
bool BulletManager::IsShot()
{
	if (!RawInput::GetInstance()->IsLButtonDown())
	{
		return false;
	}

	for (unsigned int i = 0; i < g_iBulletMax; i++)
	{
		//”­ŽË‚³‚ê‚Ä‚¢‚éê‡‚ÍŽŸ‚Ö.
		if (m_pBullet[i]->IsShotFlg())
		{
			continue;
		}
		else
		{
			//’e‚Ì”­ŽËˆÊ’u‚ð­‚µã‚É‚ ‚°‚é.
			const float fAdjustmentShotPosY = 1.0f;

			m_pBullet[i]->BulletShot(m_mShotMoveMat, { m_vShotPosition.x, m_vShotPosition.y + fAdjustmentShotPosY, m_vShotPosition.z });
			return true;
		}
	}

	return false;
}