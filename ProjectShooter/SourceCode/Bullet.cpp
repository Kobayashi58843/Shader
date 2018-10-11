#include "Bullet.h"

#include "Singleton.h"

#include "SoundManager.h"
#include "Resource.h"

//弾の速度.
const float g_fSpeed = 0.4f;

//弾の飛距離.
const float g_fFlyableDistance = 100.0f;

//弾の初期位置.
const D3DXVECTOR3 g_vInitialPosition = { 0.0f, -100.0f, 0.0f };

Bullet::Bullet(const float fCollisionRadius, const float fScale)
{
#if _DEBUG
	m_pModel = Singleton<Resource>::GetInstance()->GetStaticModels(Resource::enStaticModel_Shpere);
#endif

	m_bShotFlg = false;

	m_fFlyableDistance = 0.0f;

	CrearVECTOR3(m_Light.vPos);
	D3DXMatrixIsIdentity(&m_Light.mRot);
	CrearVECTOR3(m_Light.vDir);
	m_Light.fIntensity = 0.0f;
	
	m_vPos = g_vInitialPosition;

	CrearVECTOR3(m_vRot);

	m_fScale = fScale;

	D3DXMatrixIdentity(&m_mMoveMat);

	CrearVECTOR3(m_vPlayerPosition);

	m_Collision.fRadius = fCollisionRadius;

	m_pEffect = clsEffects::GetInstance();
}

Bullet::~Bullet()
{
#if _DEBUG
	m_pModel = nullptr;
#endif
	m_pEffect->StopAll(m_MissileHandle);
	m_pEffect->StopAll(m_ExplosionHandle);
}

//更新.
void Bullet::Update(const D3DXVECTOR3 vPlayerPos)
{
	//弾が発射中でなければ抜ける.
	if (!m_bShotFlg)
	{
		return;
	}

	m_vPlayerPosition = vPlayerPos;

	//弾の移動.
	Move();
}

//描画.
void Bullet::Render(const D3DXMATRIX &mView, const D3DXMATRIX &mProj, const D3DXVECTOR3 &vEye)
{
	//位置,回転,サイズ等をモデルに適応.
	UpdateState();

#if _DEBUG
	m_pModel->Render(mView, mProj, m_Light, vEye);
#endif
}

//弾のSphereとの当たり判定.
bool Bullet::BulletCollision(SPHERE const TargetSphere)
{
	//弾のSphereとターゲットのSphereが当たっているか.
	if (SphereCollision(m_Collision, TargetSphere))
	{
		//衝突.
		return true;
	}

	//衝突していない.
	return false;
}

//弾とSphereの衝突時.
void Bullet::BulletHitToSphere()
{
	WhenBulletDisappearance();
}

//弾とメッシュの衝突時.
void Bullet::BulletHitToMesh()
{
	WhenBulletDisappearance();
}

//弾の発射.
void Bullet::BulletShot(const D3DXMATRIX mMoveMat, const D3DXVECTOR3 vShotPosition)
{
	m_bShotFlg = true;

	m_mMoveMat = mMoveMat;

	m_vPos = vShotPosition;

	m_fFlyableDistance = 0.0f;

	m_MissileHandle = m_pEffect->Play(m_vPos, clsEffects::enEfcType_Missile);
	//エフェクトの大きさ.
	const float fMissileScale = 0.4f;
	m_pEffect->SetScale(m_MissileHandle, { fMissileScale, fMissileScale, fMissileScale });
}

//位置,回転,サイズ等をモデルに適応.
void Bullet::UpdateState()
{
#if _DEBUG
	m_pModel->m_vPos = m_vPos;

	m_pModel->m_fPitch = m_vRot.x;
	m_pModel->m_fYaw = m_vRot.y;
	m_pModel->m_fRoll = m_vRot.z;

	m_pModel->m_fScale = m_fScale;
#endif
	//当たり判定用Sphereの位置を合わせる.
	D3DXVECTOR3 vSpherePos = m_vPos;
	//Sphereの場所を上にずらす.
	vSpherePos.y += m_Collision.fRadius;
	m_Collision.vCenter = vSpherePos;

	//エフェクト.
	m_pEffect->SetLocation(m_MissileHandle, m_vPos);

	D3DXVECTOR3 vAxis = { 0.0f, 0.0f, 1.0f };
	D3DXVec3TransformCoord(&vAxis, &vAxis, &m_mMoveMat);
	vAxis += m_vPos;

	m_vRot.y = atan2f(vAxis.x - m_vPos.x, vAxis.z - m_vPos.z);

	m_pEffect->SetRotation(m_MissileHandle, m_vRot);
}

//弾の移動.
void Bullet::Move()
{
	D3DXVECTOR3 vAxisZ = { 0.0f, 0.0f, 1.0f };

	D3DXVec3TransformCoord(&vAxisZ, &vAxisZ, &m_mMoveMat);

	m_vPos += vAxisZ * g_fSpeed;
	
	m_fFlyableDistance += g_fSpeed;
	if (m_fFlyableDistance >= g_fFlyableDistance)
	{
		WhenBulletDisappearance();
	}
}

//弾の衝突時.
void Bullet::WhenBulletDisappearance()
{
	//減衰量.
	const int iAttenuationQuantity = 15;
	int iExplosionVolume = 1000;

	D3DXVECTOR3 vPosition = m_vPos - m_vPlayerPosition;
	float fLength = D3DXVec3Length(&vPosition);
	iExplosionVolume -= iAttenuationQuantity * (int)fLength;

	//最小値より音が小さい場合は最小値にする.
	const int iMinExplosionVolume = 50;
	if (iMinExplosionVolume > iExplosionVolume)
	{
		iExplosionVolume = iMinExplosionVolume;
	}

	Singleton<SoundManager>::GetInstance()->PlaySE(SoundManager::enSE_Explosion, iExplosionVolume);

	m_pEffect->Stop(m_MissileHandle);
	m_ExplosionHandle = m_pEffect->Play(m_vPos, clsEffects::enEfcType_Explosion);
	//エフェクトの大きさ.
	const float fExplosionScale = 1.0f;
	m_pEffect->SetScale(m_ExplosionHandle, { fExplosionScale, fExplosionScale, fExplosionScale });

	m_bShotFlg = false;
	m_vPos = g_vInitialPosition;
}