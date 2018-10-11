#include "Character.h"

Character::Character()
{
	m_State.iHp = 0;
	m_State.bDead = false;

	m_Action = enAction::Wait;

	m_pModel = nullptr;

	m_ModelState.iIndex = 0;
	m_ModelState.AnimationController = nullptr;
	m_ModelState.dAnimationSpeed = 0;
	m_ModelState.dAnimationTime = 0;
}

Character::~Character()
{
}

//�X�V.
void Character::Update()
{
	UpdateProduct();

	switch (m_Action)
	{
	case enAction::Wait:
		Wait();

		break;
	case enAction::Move:
		Move();

		break;
	case enAction::Attack:
		Attack();

		break;
	case enAction::Hit:
		Hit();

		break;
	case enAction::Dead:
		Dead();

		break;
	default:
		break;
	}
}

//3D���f���̕`��.
void Character::RenderModel(const D3DXMATRIX &mView, const D3DXMATRIX &mProj, const D3DXVECTOR3 &vLight, const D3DXVECTOR3 &vEye)
{
	//�ʒu,��],�T�C�Y�����f���ɓK��.
	UpdateState();

	m_ModelState.dAnimationTime += m_ModelState.dAnimationSpeed;
	m_ModelState.AnimationController->AdvanceTime(m_ModelState.dAnimationSpeed, nullptr);

	m_pModel->Render(mView, mProj, vLight, vEye, m_ModelState.AnimationController);
}

//���f����Yaw���g���đO��Ɉړ�.
void Character::MoveFrontRear(const float fSpeed)
{
	D3DXVECTOR3 vMoveAxisZ = { 0.0f, 0.0f, 1.0f };

	D3DXMATRIX mMat;
	D3DXMatrixRotationY(&mMat, m_vRot.y);

	D3DXVec3TransformCoord(&vMoveAxisZ, &vMoveAxisZ, &mMat);

	m_vPos += vMoveAxisZ * fSpeed;
}

//���f����Yaw���g���č��E�Ɉړ�.
void Character::MoveLeftRight(const float fSpeed)
{
	D3DXVECTOR3 vMoveAxisX = { 1.0f, 0.0f, 0.0f };

	D3DXMATRIX mMat;
	D3DXMatrixRotationY(&mMat, m_vRot.y);

	D3DXVec3TransformCoord(&vMoveAxisX, &vMoveAxisX, &mMat);

	m_vPos += vMoveAxisX * fSpeed;
}

//���f���f�[�^�̊֘A�t��.
void Character::AttachModel(clsD3DXSKINMESH* const pModel)
{
	assert(pModel != nullptr);

	m_pModel = pModel;

	LPD3DXANIMATIONCONTROLLER pAC = m_pModel->GetAnimController();

	pAC->CloneAnimationController(
		pAC->GetMaxNumAnimationOutputs(),
		pAC->GetMaxNumAnimationSets(),
		pAC->GetMaxNumTracks(),
		pAC->GetMaxNumEvents(),
		&m_ModelState.AnimationController);

	m_pModel->SetAnimSpeed(1);
}

//���f���f�[�^�֘A�t������.
void Character::DetatchModel()
{
	if (m_pModel != nullptr)
	{
		m_pModel = nullptr;

		if (m_ModelState.AnimationController != nullptr)
		{
			m_ModelState.AnimationController->Release();
			m_ModelState.AnimationController = nullptr;
		}
	}
}

//�A�j���[�V�����؂�ւ��֐�.
void Character::ChangeAnimation(const int iIndex, const double dStartPos)
{
	//�A�j���[�V�����͈͓̔����`�F�b�N.
	if (iIndex < 0 || iIndex >= GetAnimationQuantityMax())return;

	m_pModel->ChangeAnimSet_StartPos(iIndex, dStartPos, m_ModelState.AnimationController);

	m_ModelState.iIndex = iIndex;

	m_ModelState.dAnimationTime = dStartPos;
}

//���݂̃A�j���[�V�����̏I�����m�F����֐�.
bool Character::IsAnimationEnd()
{
	if (m_pModel->GetAnimPeriod(m_ModelState.iIndex) - m_ModelState.dAnimationSpeed < m_ModelState.dAnimationTime)
	{
		return true;
	}

	return false;
}

//���݂̃A�j���[�V�����������I�����������m�F����֐�.
bool Character::IsAnimationRatioEnd(const float fRatio)
{
	if ((m_pModel->GetAnimPeriod(m_ModelState.iIndex) - m_ModelState.dAnimationSpeed) * fRatio < m_ModelState.dAnimationTime)
	{
		return true;
	}

	return false;
}

//�A�j���[�V�����̍Đ����x��ς���.
void Character::SetAnimationSpeed(const double dAnimationSpeed)
{
	m_ModelState.dAnimationSpeed = dAnimationSpeed;
	m_pModel->SetAnimSpeed(m_ModelState.dAnimationSpeed);
}

//�A�j���[�V�����ő吔���擾.
int Character::GetAnimationQuantityMax()
{
	if (m_ModelState.AnimationController != nullptr)
	{
		return (int)m_ModelState.AnimationController->GetMaxNumAnimationSets();
	}
	return -1;
}

//�ʒu,��],�T�C�Y�������f���ɓK��.
void Character::UpdateState()
{
	m_pModel->m_Trans.vPos = m_vPos;

	m_pModel->m_Trans.fPitch = m_vRot.x;
	m_pModel->m_Trans.fYaw = m_vRot.y;
	m_pModel->m_Trans.fRoll = m_vRot.z;

	m_pModel->m_Trans.vScale = { m_fScale, m_fScale, m_fScale };

	//�����蔻��pSphere�̈ʒu�����킹��.
	D3DXVECTOR3 vSpherePos = m_vPos;
	//Sphere�̏ꏊ����ɂ��炷.
	const float fAdjustmentPositionY = 1.0f;
	vSpherePos.y += m_Collision.fRadius + fAdjustmentPositionY;
	m_Collision.vCenter = vSpherePos;
}

