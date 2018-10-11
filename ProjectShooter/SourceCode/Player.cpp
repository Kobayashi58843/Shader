#include "Player.h"

#include "SoundManager.h"

const float g_fScale = 0.0005f;

//���f���̌����̒���.
const int g_iAdjustModelOrientation = 180;

//���f���̃A�j���[�V�������x.
const double g_dAnimationSpeed = 0.01f;

//���f���̈ړ����x.
const float g_fMoveSpeed = 0.08f;

//�����蔻��̑傫��.
const float g_fRadius = 0.6f;

//�����ʒu.
const D3DXVECTOR3 g_vInitialPosition = { 0.0f, 0.0f, 0.0f };

//�ړ��\�͈�.
const float g_fMoveLimit = 30.0f;

Player::Player(clsD3DXSKINMESH* const pModel, BulletManager* const pBulletManager)
{
	AttachModel(pModel);

	m_bPushedMoveButtonFlg = false;

	m_fTargetDirection = 0.0f;

	AddYaw(g_iAdjustModelOrientation);

	//�����ʒu.
	m_vPos = g_vInitialPosition;

	m_vOldPos = m_vPos;

	//�T�C�Y.
	m_fScale = g_fScale;

	//�A�j���[�V�������x.
	SetAnimationSpeed(g_dAnimationSpeed);

	SetAction(enAction::Wait);

	m_Collision.fRadius = g_fRadius;

	m_pBulletManager = pBulletManager;

	m_State.iHp = g_iPlayerHpMax;
	m_State.bDead = false;

	m_pEffect = clsEffects::GetInstance();

	//�N����1��ڂ̃G�t�F�N�g���Đ�����Ȃ����߂��̑Ή��Ƃ���1��Đ����Ă���.
	m_HitHandle = m_pEffect->Play({0.0f,0.0f,0.0f}, clsEffects::enEfcType_PlayerHit);
	m_pEffect->Stop(m_HitHandle);
}

Player::~Player()
{
	DetatchModel();

	m_pEffect->StopAll(m_HitHandle);
	m_pEffect->StopAll(m_DeadHandle);
	m_pEffect->StopAll(m_WallHitHandle);
}

//���C�ƃ��b�V���̏Փˎ�.
void Player::RayHitToMesh(clsDX9Mesh* const pTarget)
{
	if (IsRayHit(pTarget))
	{
		//��_�̍��W����y���W�����@��y���W�Ƃ��ăZ�b�g.
		m_vPos.y = GetRayIntersect().y;
	}
}

//Sphere�Ƃ̏Փˎ�.
void Player::HitToSphere(SPHERE const TargetSphere)
{
	if (SphereCollision(m_Collision, TargetSphere))
	{
		if (m_Action != enAction::Hit
			&& m_Action != enAction::Dead)
		{
			SetAction(enAction::Hit);
		}
	}
}

void Player::UpdateProduct()
{
}

void Player::Wait()
{
	//�ړ��̓��͂��󂯕t����.
	AcceptedMoveButton();

	//�U���̓��͂��󂯕t����.
	AcceptedAttackButton();
}

void Player::Move()
{
	const float fTurnSpeed = 0.1f;
	m_fTargetDirection += (float)D3DXToRadian(g_iAdjustModelOrientation);
	SpinModel(m_fTargetDirection, fTurnSpeed);

	//�ړ��̓��͂��󂯕t����.
	AcceptedMoveButton();

	//�U���̓��͂��󂯕t����.
	AcceptedAttackButton();

	//�ړ��L�[��������Ă��Ȃ��̂őҋ@��.
	if (!m_bPushedMoveButtonFlg)
	{
		SetAction(enAction::Wait);
	}
}

void Player::Attack()
{
	const float fTurnSpeed = 0.4f;
	m_fTargetDirection += (float)D3DXToRadian(g_iAdjustModelOrientation);
	SpinModel(m_fTargetDirection, fTurnSpeed);

	const float fRatio = 0.5f;
	if (IsAnimationRatioEnd(fRatio))
	{
		SetAction(enAction::Wait);
	}
}

void Player::Hit()
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

void Player::Dead()
{
	const float fRatio = 0.5f;

	if (IsAnimationRatioEnd(fRatio))
	{
		m_State.bDead = true;
	}
}

//�A�N�V�����̐؂�ւ�.
void Player::SetAction(const enAction Action)
{
	if (m_Action == Action)return;

	m_Action = Action;

	//�A�j���[�V�����̑��x�������l�ɖ߂�.
	SetAnimationSpeed(g_dAnimationSpeed);

	//�G�t�F�N�g�̍Đ��ʒu�𒲐�����.
	const float fAdjustPosition = 1.0f;
	D3DXVECTOR3 vEffectPos = m_vPos;
	vEffectPos.y += fAdjustPosition;

	//�G�t�F�N�g�̍Đ����~�߂�.
	m_pEffect->Stop(m_HitHandle);
	m_pEffect->Stop(m_DeadHandle);

	switch (Action)
	{
	case enAction::Wait:
		ChangeAnimation(1);

		break;
	case enAction::Move:
		ChangeAnimation(0);
		m_bPushedMoveButtonFlg = true;

		break;
	case enAction::Attack:
		ChangeAnimation(7);

		//SE���Đ�.
		SoundManager::GetInstance()->PlaySE(SoundManager::enSE_PlayerAttack);
		SoundManager::GetInstance()->PlaySE(SoundManager::enSE_PlayerAttackVoice);

		break;
	case enAction::Hit:
		ChangeAnimation(3);
		//�A�j���[�V������x������.
		{
			const double dDeadAnimationSpeed = 0.006;
			SetAnimationSpeed(dDeadAnimationSpeed);
		}

		m_State.iHp--;

		//SE���Đ�.
		SoundManager::GetInstance()->PlaySE(SoundManager::enSE_PlayerDamage);
		SoundManager::GetInstance()->PlaySE(SoundManager::enSE_PlayerDamageVoice);

		//�G�t�F�N�g���Đ�.
		m_HitHandle = m_pEffect->Play(vEffectPos, clsEffects::enEfcType_PlayerHit);
		{
			const float fHitEffectScale = 0.5f;
			m_pEffect->SetScale(m_HitHandle, { fHitEffectScale, fHitEffectScale, fHitEffectScale });
		}
		break;
	case enAction::Dead:
		ChangeAnimation(5);
		//�A�j���[�V������x������.
		{
			const double dDeadAnimationSpeed = 0.005;
			SetAnimationSpeed(dDeadAnimationSpeed);
		}

		//SE���Đ�.
		SoundManager::GetInstance()->PlaySE(SoundManager::enSE_PlayerDead);
		SoundManager::GetInstance()->PlaySE(SoundManager::enSE_PlayerDeadVoice);
		
		//�G�t�F�N�g���Đ�.
		m_DeadHandle = m_pEffect->Play(vEffectPos, clsEffects::enEfcType_PlayerDead);
		{
			const float fDeadEffectScale = 0.2f;
			m_pEffect->SetScale(m_DeadHandle, { fDeadEffectScale, fDeadEffectScale, fDeadEffectScale });
		}
		break;
	default:
		break;
	}
}

//�ړ��̓��͂��󂯕t���邩�ǂ���.
void Player::AcceptedMoveButton()
{
	bool bNotPushedFrontRear = false;
	bool bNotPushedLeftRight = false;

	if (GetKeyState('W') & 0x8000)
	{
		SetAction(enAction::Move);
		MoveFrontRear(-g_fMoveSpeed);
	}
	else if (GetKeyState('S') & 0x8000)
	{
		SetAction(enAction::Move);
		MoveFrontRear(g_fMoveSpeed);
	}
	else
	{
		bNotPushedFrontRear = true;
	}

	if (GetKeyState('A') & 0x8000)
	{
		SetAction(enAction::Move);
		MoveLeftRight(g_fMoveSpeed);
	}
	else if (GetKeyState('D') & 0x8000)
	{
		SetAction(enAction::Move);
		MoveLeftRight(-g_fMoveSpeed);
	}
	else
	{
		bNotPushedLeftRight = true;
	}

	if (bNotPushedFrontRear && bNotPushedLeftRight)
	{
		m_bPushedMoveButtonFlg = false;
	}

	//�ړ�����.
	MoveLimit();
}

//�ړ�����.
void Player::MoveLimit()
{
	const float fDistance = D3DXVec3Length(&m_vPos);

	if (fabs(fDistance) < g_fMoveLimit)
	{
		m_vOldPos = m_vPos;
	}
	else
	{
		m_vPos = m_vOldPos;

		if (SoundManager::GetInstance()->IsStoppedFirstSE(SoundManager::enSE_PlayerMoveLimit))
		{
			//SE���Đ�.
			SoundManager::GetInstance()->PlayFirstSE(SoundManager::enSE_PlayerMoveLimit);

			//�G�t�F�N�g���Đ�.
			m_WallHitHandle = m_pEffect->Play(m_vPos, clsEffects::enEfcType_PlayerWallHit);

			const float fEffectScale = 0.5f;
			m_pEffect->SetScale(m_WallHitHandle, { fEffectScale, fEffectScale, fEffectScale });

			const float fYaw = m_vRot.y - (float)D3DXToRadian(90);
			m_pEffect->SetRotation(m_WallHitHandle, { 0.0f, fYaw, 0.0f });
		}
	}
}

//�U���̓��͂��󂯕t���邩�ǂ���.
void Player::AcceptedAttackButton()
{
	if (m_pBulletManager->IsShot())
	{
		SetAction(enAction::Attack);
	}
}