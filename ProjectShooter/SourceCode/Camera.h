#pragma once
#pragma warning( disable:4005 )

#include "Object.h"

class Camera : public Object
{
public:
	Camera(const float fWindowWidth, const float fWindowHeight);
	~Camera();

	//���t���[���X�V�������.
	void Update(clsDX9Mesh* const pTarget);

	//������Ԃł̃J�����ʒu.
	void SetFocusingSpacePos(const D3DXVECTOR3 vPos)
	{
		m_vFocusingSpacePos = vPos;
	}
	D3DXVECTOR3 GetFocusingSpacePos() const
	{
		return m_vFocusingSpacePos;
	}

	//�J�����̃��[���h���W.
	D3DXVECTOR3 GetWorldPos() const
	{
		return m_vWorldPos;
	}

	//�r���[�s��.
	D3DXMATRIX GetView() const
	{
		return m_mView;
	}

	//�J�����p���s��.
	D3DXMATRIX GetCameraPose() const
	{
		return m_mCameraPose;
	}

	//�v���W�F�N�V�����s��.
	D3DXMATRIX GetProjection() const
	{
		return m_mProjection;
	}

	//�����_.
	void SetLookAt(const D3DXVECTOR3 vLook)
	{
		m_vLookAt = vLook;
	}
	D3DXVECTOR3 GetLookAt() const
	{
		return m_vLookAt;
	}

	//�ړ�.
	void MoveLeft()
	{
		m_vCameraMoveDirDif.x = -1.0f;
	}
	void MoveRight()
	{
		m_vCameraMoveDirDif.x = 1.0f;
	}
	void MoveUp()
	{
		m_vCameraMoveDirDif.y = 1.0f;
	}
	void MoveDown()
	{
		m_vCameraMoveDirDif.y = -1.0f;
	}

	//��]�����x�N�g���̍���.
	void SetRotDif(const D3DXVECTOR3 vRotDif)
	{
		m_vCamRotDif = vRotDif;
	}

	//�J������]���ɓ����p�x�̒P��(�x���@�Ŋp�x�����Ă�������).
	void SetRotPerFrame(const float fDegree)
	{
		double dMoveAngle = D3DXToRadian(fDegree);
		m_fCameraAngleUnit = (float)dMoveAngle;
	}

	//�J�����̉��s������������.
	void SetOffsetZ(const float fOffsetZ)
	{
		m_fOffsetZ = fOffsetZ;
	}

	//���ɂ��炷�l.
	void SetDisplaceHorizontally(const float fDisplace)
	{
		m_fDisplaceHorizontally = fDisplace;
	}

	//���C�ƃ��b�V���̏Փˎ�.
	void RayHitToMesh(clsDX9Mesh* const pTarget);

private:
	//�E�B���h�E��.
	float m_fWindowWidth;
	//�E�B���h�E����.
	float m_fWindowHeight;

	//�����_��Ԃł̃J�����̈ʒu.
	D3DXVECTOR3 m_vFocusingSpacePos;
	//�J�����̃��[���h���W.
	D3DXVECTOR3 m_vWorldPos;
	//�r���[�s��.
	D3DXMATRIX	m_mView;
	//�J�����p���s��.
	D3DXMATRIX m_mCameraPose;
	//�v���W�F�N�V�����s��.
	D3DXMATRIX m_mProjection;

	//�����_.
	D3DXVECTOR3	m_vLookAt;
	//�J�����̈ړ������x�N�g���̍���.
	D3DXVECTOR3 m_vCameraMoveDirDif;
	//�J�������ł̉�]�̍���.
	D3DXVECTOR3 m_vCamRotDif;
	//�ړ�����]�P�ʁi���W�A���j.
	float m_fCameraAngleUnit;
	//���s������������.
	float m_fOffsetZ;

	//�O�̒����_��Ԃł̃J�����̈ʒu.
	D3DXVECTOR3 m_vOldFocusingSpacePos;

	//���ɂ��炷.
	float m_fDisplaceHorizontally;

	//�v���W�F�N�V����(�ˉe�s��)�ϊ�.
	void Projection();

	//�ړ�.
	void Move(const D3DXVECTOR3 vDif);

	//�ړ�����.
	void MoveLimit(clsDX9Mesh* const pTarget);

	//�p���X�V.
	void PostureUpdate();

	//���s�������̃I�t�Z�b�g�X�V.
	void OffsetZUpdate();

	//�ʒu,��],�T�C�Y��K��.
	void UpdateState();
};
