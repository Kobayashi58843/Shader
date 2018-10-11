#include "Enemy.h"

#include "SoundManager.h"

#include "Singleton.h"

const float g_fScale = 0.08f;

//���f���̌����̒���.
const int g_iAdjustModelOrientation = 0;

//���f���̃A�j���[�V�������x.
const double g_dAnimationSpeed = 0.01f;

//���f���̈ړ����x.
const float g_fMoveSpeed = 0.12f;

//�U�����̈ړ����x.
const float g_fAttackMoveSpeed = 0.24f;

//�����蔻��̑傫��.
const float g_fRadius = 1.0f;

//�U���ɂ��鋗��.
const float g_fInAttackRange = 10.0f;

//�����ʒu.
const D3DXVECTOR3 g_vInitialPosition = { 0.0f, 0.0f, 20.0f };

Enemy::Enemy(clsD3DXSKINMESH* const pModel)
{
	AttachModel(pModel);

	m_fTargetDirection = 0.0f;

	CrearVECTOR3(m_vPlayerPosition);

	m_fAttackDistance = 0.0f;

	AddYaw(g_iAdjustModelOrientation);

	//�����ʒu.
	m_vPos = g_vInitialPosition;

	//�T�C�Y.
	m_fScale = g_fScale;

	//�A�j���[�V�������x.
	SetAnimationSpeed(g_dAnimationSpeed);

	ChangeAnimation(5);

	m_Collision.fRadius = g_fRadius;

	m_State.iHp = g_iEnemyHpMax;
	m_State.bDead = false;

	m_pEffect = clsEffects::GetInstance();
}

Enemy::~Enemy()
{
	DetatchModel();

	m_pEffect->StopAll(m_HitHandle);
	m_pEffect->StopAll(m_DeadHandle);
}

//���C�ƃ��b�V���̏Փˎ�.
void Enemy::RayHitToMesh(clsDX9Mesh* const pTarget)
{
	if (IsRayHit(pTarget))
	{
		//��_�̍��W����y���W�����@��y���W�Ƃ��ăZ�b�g.
		m_vPos.y = GetRayIntersect().y;
	}
}

//Sphere�Ƃ̏Փˎ�.
void Enemy::HitToSphere()
{
	if (m_Action != enAction::Attack
		&& m_Action != enAction::Hit
		&& m_Action != enAction::Dead)
	{
		m_State.iHp--;
		SetAction(enAction::Hit);
	}
	else if (m_Action == enAction::Attack || m_Action == enAction::Hit)
	{
		Singleton<SoundManager>::GetInstance()->PlaySE(SoundManager::enSE_EnemyGuard);
	}
}

void Enemy::UpdateProduct()
{
}

void Enemy::Wait()
{
	const float fTurnSpeed = 0.015f;
	m_fTargetDirection += (float)D3DXToRadian(g_iAdjustModelOrientation);
	SpinModel(m_fTargetDirection, fTurnSpeed);

	D3DXVECTOR3 vVec = m_vPlayerPosition - m_vPos;
	float fLength = D3DXVec3Length(&vVec);

	if (fabs(m_vRot.y - m_fTargetDirection) <= D3DXToRadian(2))
	{
		if (fLength <= g_fInAttackRange)
		{
			SetAction(enAction::Attack);
		}
		else
		{
			SetAction(enAction::Move);
		}
	}
}

void Enemy::Move()
{
	const float fTurnSpeed = 0.1f;
	m_fTargetDirection += (float)D3DXToRadian(g_iAdjustModelOrientation);
	SpinModel(m_fTargetDirection, fTurnSpeed);

	MoveFrontRear(-g_fMoveSpeed);

	D3DXVECTOR3 vVec = m_vPlayerPosition - m_vPos;
	float fLength = D3DXVec3Length(&vVec);

	if (fabs(m_vRot.y - m_fTargetDirection) <= D3DXToRadian(2))
	{
		if (fLength <= g_fInAttackRange)
		{
			SetAction(enAction::Attack);
		}
	}
}

void Enemy::Attack()
{
	MoveFrontRear(-g_fAttackMoveSpeed);
	m_fAttackDistance += g_fAttackMoveSpeed;

	const float fAttackMaxDistance = 15.0f;
	if (m_fAttackDistance >= fAttackMaxDistance)
	{
		SetAction(enAction::Wait);
	}
}

void Enemy::Hit()
{
	if (0 >= m_State.iHp)
	{
		SetAction(enAction::Dead);
	}

	const float fRatio = 0.5f;

	if (IsAnimationRatioEnd(fRatio))
	{
		SetAction(enAction::Wait);
	}
}

void Enemy::Dead()
{
	const float fRatio = 0.5f;

	if (IsAnimationRatioEnd(fRatio))
	{
		m_State.bDead = true;
	}
}

//�A�N�V�����̐؂�ւ�.
void Enemy::SetAction(const enAction Action)
{
	if (m_Action == Action)return;

	m_Action = Action;

	//�A�j���[�V�����̑��x�������l�ɖ߂�.
	SetAnimationSpeed(g_dAnimationSpeed);

	//�G�t�F�N�g�̍Đ��ʒu�𒲐�����.
	const float fAdjustPosition = 1.5f;
	D3DXVECTOR3 vEffectPos = m_vPos;
	vEffectPos.y += fAdjustPosition;

	//�G�t�F�N�g�̍Đ����~�߂�.
	m_pEffect->Stop(m_HitHandle);
	m_pEffect->Stop(m_DeadHandle);

	switch (Action)
	{
	case enAction::Wait:
		ChangeAnimation(5);
		m_fAttackDistance = 0.0f;

		break;
	case enAction::Move:
		ChangeAnimation(3);

		break;
	case enAction::Attack:
		ChangeAnimation(7);

		{
			const int iAttackVolume = 400;
			Singleton<SoundManager>::GetInstance()->PlaySE(SoundManager::enSE_EnemyAttack, iAttackVolume);
			Singleton<SoundManager>::GetInstance()->PlaySE(SoundManager::enSE_EnemyAttackVoice);
		}

		break;
	case enAction::Hit:
		ChangeAnimation(2);
		//�A�j���[�V�����𑁂�����.
		{
			const double dHitAnimationSpeed = 0.02;
			SetAnimationSpeed(dHitAnimationSpeed);
		}

		//SE���Đ�.
		Singleton<SoundManager>::GetInstance()->PlaySE(SoundManager::enSE_EnemyDamage);
		Singleton<SoundManager>::GetInstance()->PlaySE(SoundManager::enSE_EnemyDamageVoice);

		//�G�t�F�N�g���Đ�.
		m_HitHandle = m_pEffect->Play(vEffectPos, clsEffects::enEfcType_PlayerHit);
		{
			const float fHitEffectScale = 1.0f;
			m_pEffect->SetScale(m_HitHandle, { fHitEffectScale, fHitEffectScale, fHitEffectScale });
		}
		break;
	case enAction::Dead:
		ChangeAnimation(4);
		//�A�j���[�V������x������.
		{
			const double dDeadAnimationSpeed = 0.005;
			SetAnimationSpeed(dDeadAnimationSpeed);
		}

		//SE���Đ�.
		Singleton<SoundManager>::GetInstance()->PlaySE(SoundManager::enSE_EnemyDead);
		Singleton<SoundManager>::GetInstance()->PlaySE(SoundManager::enSE_EnemyDeadVoice);

		//�G�t�F�N�g���Đ�.
		m_DeadHandle = m_pEffect->Play(vEffectPos, clsEffects::enEfcType_EnemyDead);
		{
			const float fDeadEffectScale = 0.01f;
			m_pEffect->SetScale(m_DeadHandle, { fDeadEffectScale, fDeadEffectScale, fDeadEffectScale });
		}

		break;
	default:
		break;
	}
}