#include "Bullet.h"

#include "Singleton.h"

#include "SoundManager.h"
#include "Resource.h"

//�e�̑��x.
const float g_fSpeed = 0.4f;

//�e�̔򋗗�.
const float g_fFlyableDistance = 100.0f;

//�e�̏����ʒu.
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

//�X�V.
void Bullet::Update(const D3DXVECTOR3 vPlayerPos)
{
	//�e�����˒��łȂ���Δ�����.
	if (!m_bShotFlg)
	{
		return;
	}

	m_vPlayerPosition = vPlayerPos;

	//�e�̈ړ�.
	Move();
}

//�`��.
void Bullet::Render(const D3DXMATRIX &mView, const D3DXMATRIX &mProj, const D3DXVECTOR3 &vEye)
{
	//�ʒu,��],�T�C�Y�������f���ɓK��.
	UpdateState();

#if _DEBUG
	m_pModel->Render(mView, mProj, m_Light, vEye);
#endif
}

//�e��Sphere�Ƃ̓����蔻��.
bool Bullet::BulletCollision(SPHERE const TargetSphere)
{
	//�e��Sphere�ƃ^�[�Q�b�g��Sphere���������Ă��邩.
	if (SphereCollision(m_Collision, TargetSphere))
	{
		//�Փ�.
		return true;
	}

	//�Փ˂��Ă��Ȃ�.
	return false;
}

//�e��Sphere�̏Փˎ�.
void Bullet::BulletHitToSphere()
{
	WhenBulletDisappearance();
}

//�e�ƃ��b�V���̏Փˎ�.
void Bullet::BulletHitToMesh()
{
	WhenBulletDisappearance();
}

//�e�̔���.
void Bullet::BulletShot(const D3DXMATRIX mMoveMat, const D3DXVECTOR3 vShotPosition)
{
	m_bShotFlg = true;

	m_mMoveMat = mMoveMat;

	m_vPos = vShotPosition;

	m_fFlyableDistance = 0.0f;

	m_MissileHandle = m_pEffect->Play(m_vPos, clsEffects::enEfcType_Missile);
	//�G�t�F�N�g�̑傫��.
	const float fMissileScale = 0.4f;
	m_pEffect->SetScale(m_MissileHandle, { fMissileScale, fMissileScale, fMissileScale });
}

//�ʒu,��],�T�C�Y�������f���ɓK��.
void Bullet::UpdateState()
{
#if _DEBUG
	m_pModel->m_vPos = m_vPos;

	m_pModel->m_fPitch = m_vRot.x;
	m_pModel->m_fYaw = m_vRot.y;
	m_pModel->m_fRoll = m_vRot.z;

	m_pModel->m_fScale = m_fScale;
#endif
	//�����蔻��pSphere�̈ʒu�����킹��.
	D3DXVECTOR3 vSpherePos = m_vPos;
	//Sphere�̏ꏊ����ɂ��炷.
	vSpherePos.y += m_Collision.fRadius;
	m_Collision.vCenter = vSpherePos;

	//�G�t�F�N�g.
	m_pEffect->SetLocation(m_MissileHandle, m_vPos);

	D3DXVECTOR3 vAxis = { 0.0f, 0.0f, 1.0f };
	D3DXVec3TransformCoord(&vAxis, &vAxis, &m_mMoveMat);
	vAxis += m_vPos;

	m_vRot.y = atan2f(vAxis.x - m_vPos.x, vAxis.z - m_vPos.z);

	m_pEffect->SetRotation(m_MissileHandle, m_vRot);
}

//�e�̈ړ�.
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

//�e�̏Փˎ�.
void Bullet::WhenBulletDisappearance()
{
	//������.
	const int iAttenuationQuantity = 15;
	int iExplosionVolume = 1000;

	D3DXVECTOR3 vPosition = m_vPos - m_vPlayerPosition;
	float fLength = D3DXVec3Length(&vPosition);
	iExplosionVolume -= iAttenuationQuantity * (int)fLength;

	//�ŏ��l��艹���������ꍇ�͍ŏ��l�ɂ���.
	const int iMinExplosionVolume = 50;
	if (iMinExplosionVolume > iExplosionVolume)
	{
		iExplosionVolume = iMinExplosionVolume;
	}

	Singleton<SoundManager>::GetInstance()->PlaySE(SoundManager::enSE_Explosion, iExplosionVolume);

	m_pEffect->Stop(m_MissileHandle);
	m_ExplosionHandle = m_pEffect->Play(m_vPos, clsEffects::enEfcType_Explosion);
	//�G�t�F�N�g�̑傫��.
	const float fExplosionScale = 1.0f;
	m_pEffect->SetScale(m_ExplosionHandle, { fExplosionScale, fExplosionScale, fExplosionScale });

	m_bShotFlg = false;
	m_vPos = g_vInitialPosition;
}