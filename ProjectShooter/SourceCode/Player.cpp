#include "Player.h"

#include "SoundManager.h"

const float g_fScale = 0.0005f;

//モデルの向きの調整.
const int g_iAdjustModelOrientation = 180;

//モデルのアニメーション速度.
const double g_dAnimationSpeed = 0.01f;

//モデルの移動速度.
const float g_fMoveSpeed = 0.08f;

//当たり判定の大きさ.
const float g_fRadius = 0.6f;

//初期位置.
const D3DXVECTOR3 g_vInitialPosition = { 0.0f, 0.0f, 0.0f };

//移動可能範囲.
const float g_fMoveLimit = 30.0f;

Player::Player(clsD3DXSKINMESH* const pModel, BulletManager* const pBulletManager)
{
	AttachModel(pModel);

	m_bPushedMoveButtonFlg = false;

	m_fTargetDirection = 0.0f;

	AddYaw(g_iAdjustModelOrientation);

	//初期位置.
	m_vPos = g_vInitialPosition;

	m_vOldPos = m_vPos;

	//サイズ.
	m_fScale = g_fScale;

	//アニメーション速度.
	SetAnimationSpeed(g_dAnimationSpeed);

	SetAction(enAction::Wait);

	m_Collision.fRadius = g_fRadius;

	m_pBulletManager = pBulletManager;

	m_State.iHp = g_iPlayerHpMax;
	m_State.bDead = false;

	m_pEffect = clsEffects::GetInstance();

	//起動後1回目のエフェクトが再生されないためその対応として1回再生しておく.
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

//レイとメッシュの衝突時.
void Player::RayHitToMesh(clsDX9Mesh* const pTarget)
{
	if (IsRayHit(pTarget))
	{
		//交点の座標からy座標を自機のy座標としてセット.
		m_vPos.y = GetRayIntersect().y;
	}
}

//Sphereとの衝突時.
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
	//移動の入力を受け付ける.
	AcceptedMoveButton();

	//攻撃の入力を受け付ける.
	AcceptedAttackButton();
}

void Player::Move()
{
	const float fTurnSpeed = 0.1f;
	m_fTargetDirection += (float)D3DXToRadian(g_iAdjustModelOrientation);
	SpinModel(m_fTargetDirection, fTurnSpeed);

	//移動の入力を受け付ける.
	AcceptedMoveButton();

	//攻撃の入力を受け付ける.
	AcceptedAttackButton();

	//移動キーが押されていないので待機へ.
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

//アクションの切り替え.
void Player::SetAction(const enAction Action)
{
	if (m_Action == Action)return;

	m_Action = Action;

	//アニメーションの速度を初期値に戻す.
	SetAnimationSpeed(g_dAnimationSpeed);

	//エフェクトの再生位置を調整する.
	const float fAdjustPosition = 1.0f;
	D3DXVECTOR3 vEffectPos = m_vPos;
	vEffectPos.y += fAdjustPosition;

	//エフェクトの再生を止める.
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

		//SEを再生.
		SoundManager::GetInstance()->PlaySE(SoundManager::enSE_PlayerAttack);
		SoundManager::GetInstance()->PlaySE(SoundManager::enSE_PlayerAttackVoice);

		break;
	case enAction::Hit:
		ChangeAnimation(3);
		//アニメーションを遅くする.
		{
			const double dDeadAnimationSpeed = 0.006;
			SetAnimationSpeed(dDeadAnimationSpeed);
		}

		m_State.iHp--;

		//SEを再生.
		SoundManager::GetInstance()->PlaySE(SoundManager::enSE_PlayerDamage);
		SoundManager::GetInstance()->PlaySE(SoundManager::enSE_PlayerDamageVoice);

		//エフェクトを再生.
		m_HitHandle = m_pEffect->Play(vEffectPos, clsEffects::enEfcType_PlayerHit);
		{
			const float fHitEffectScale = 0.5f;
			m_pEffect->SetScale(m_HitHandle, { fHitEffectScale, fHitEffectScale, fHitEffectScale });
		}
		break;
	case enAction::Dead:
		ChangeAnimation(5);
		//アニメーションを遅くする.
		{
			const double dDeadAnimationSpeed = 0.005;
			SetAnimationSpeed(dDeadAnimationSpeed);
		}

		//SEを再生.
		SoundManager::GetInstance()->PlaySE(SoundManager::enSE_PlayerDead);
		SoundManager::GetInstance()->PlaySE(SoundManager::enSE_PlayerDeadVoice);
		
		//エフェクトを再生.
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

//移動の入力を受け付けるかどうか.
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

	//移動制限.
	MoveLimit();
}

//移動制限.
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
			//SEを再生.
			SoundManager::GetInstance()->PlayFirstSE(SoundManager::enSE_PlayerMoveLimit);

			//エフェクトを再生.
			m_WallHitHandle = m_pEffect->Play(m_vPos, clsEffects::enEfcType_PlayerWallHit);

			const float fEffectScale = 0.5f;
			m_pEffect->SetScale(m_WallHitHandle, { fEffectScale, fEffectScale, fEffectScale });

			const float fYaw = m_vRot.y - (float)D3DXToRadian(90);
			m_pEffect->SetRotation(m_WallHitHandle, { 0.0f, fYaw, 0.0f });
		}
	}
}

//攻撃の入力を受け付けるかどうか.
void Player::AcceptedAttackButton()
{
	if (m_pBulletManager->IsShot())
	{
		SetAction(enAction::Attack);
	}
}