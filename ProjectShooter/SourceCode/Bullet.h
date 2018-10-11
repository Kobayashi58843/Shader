#pragma once

#include "Object.h"

#include "CEffects.h"

class Bullet : public Object
{
public:
	Bullet(const float fCollisionRadius, const float fScale);
	~Bullet();

	//�X�V.
	void Update(const D3DXVECTOR3 vPlayerPos);

	//�`��.
	void Render(const D3DXMATRIX &mView, const D3DXMATRIX &mProj, const D3DXVECTOR3 &vEye);

	//�e��Sphere�Ƃ̓����蔻��.
	bool BulletCollision(SPHERE const TargetSphere);

	//�e��Sphere�̏Փˎ�.
	void BulletHitToSphere();

	//�e�ƃ��b�V���̏Փˎ�.
	void BulletHitToMesh();

	//�e�̔���.
	void BulletShot(const D3DXMATRIX mMoveMat, const D3DXVECTOR3 vShotPosition);

	bool IsShotFlg() const
	{
		return m_bShotFlg;
	}

private:
#if _DEBUG
	//�e�̃��f��.
	clsDX9Mesh* m_pModel;
#endif

	//�G�t�F�N�g�֘A.
	clsEffects* m_pEffect;
	//�n���h��.
	::Effekseer::Handle m_MissileHandle;
	::Effekseer::Handle m_ExplosionHandle;
	
	//���˒�.
	bool m_bShotFlg;

	//�e�̔򋗗�.
	float m_fFlyableDistance;

	//���C�g���.
	LIGHT m_Light;

	//�ړ�����.
	D3DXMATRIX m_mMoveMat;

	//���ʒ��߂Ɏg���v���C���[�̈ʒu.
	D3DXVECTOR3 m_vPlayerPosition;

	//�ʒu,��],�T�C�Y�������f���ɓK��.
	void UpdateState();

	//�e�̈ړ�.
	void Move();

	//�e�̏��Ŏ�.
	void WhenBulletDisappearance();
};