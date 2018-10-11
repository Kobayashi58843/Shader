#pragma once

#include "Bullet.h"

#include "Enemy.h"

//�e�̍ő吔.
const int g_iBulletMax = 64;

class BulletManager
{
public:
	BulletManager();
	~BulletManager();

	//�X�V.
	void Update(const D3DXMATRIX mShotMoveMat, const D3DXVECTOR3 vShotPosition, Enemy* const pEnemy);

	//�`��.
	void Render(const D3DXMATRIX &mView, const D3DXMATRIX &mProj, const D3DXVECTOR3 &vEye);

	//�e�̓����蔻��.
	void CollisionJudgmentBullet(const SPHERE TargetSphere, clsDX9Mesh* const pTargetMesh);

	//�e�̔���.
	bool IsShot();

private:
	Bullet* m_pBullet[g_iBulletMax];

	D3DXMATRIX m_mShotMoveMat;
	D3DXVECTOR3 m_vShotPosition;

	Enemy* m_pEnemy;

	void SetShotMoveMat(const D3DXMATRIX mMoveMat)
	{
		m_mShotMoveMat = mMoveMat;
	}

	void SetShotPosition(const D3DXVECTOR3 vShotPosition)
	{
		m_vShotPosition = vShotPosition;
	}

};