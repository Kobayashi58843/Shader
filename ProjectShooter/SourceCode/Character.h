#pragma once
#pragma warning( disable:4005 )

#include "Object.h"
#include "CD3DXSKINMESH.h"

class Character : public Object
{
public:
	Character();
	virtual ~Character();

	//�X�V.
	void Update();

	//3D���f���̕`��.
	void RenderModel(const D3DXMATRIX &mView, const D3DXMATRIX &mProj, const D3DXVECTOR3 &vLight, const D3DXVECTOR3 &vEye);

	//���f����Yaw���g���đO��Ɉړ�.
	void MoveFrontRear(const float fSpeed);

	//���f����Yaw���g���č��E�Ɉړ�.
	void MoveLeftRight(const float fSpeed);

protected:
	//�X�L�����b�V���̃��f���ɕK�v�ȏ��.
	struct SkinMeshState
	{	
		//�Đ����Ă���A�j���[�V�����̔ԍ�.
		int iIndex;

		//�A�j���[�V�����R���g���[��.
		LPD3DXANIMATIONCONTROLLER AnimationController;

		//���f���̃A�j���[�V�����̍Đ����x.
		double dAnimationSpeed;

		//���f���̃A�j���[�V�����̌��݂̎���.
		double dAnimationTime;
	};

	struct State
	{
		int iHp;

		bool bDead;
	};

	enum class enAction : UCHAR
	{
		Wait,
		Move,
		Attack,
		Hit,
		Dead
	};

	State m_State;

	enAction m_Action;

	//�g���������f���̃|�C���^.
	clsD3DXSKINMESH* m_pModel;

	SkinMeshState m_ModelState;

	void AttachModel(clsD3DXSKINMESH* const pModel);
	void DetatchModel();

	//�A�j���[�V�����؂�ւ��֐�.
	void ChangeAnimation(const int iIndex, const double dStartPos = 0.0f);

	//���݂̃A�j���[�V�����̏I�����m�F����֐�.
	bool IsAnimationEnd();

	//���݂̃A�j���[�V�����������I�����������m�F����֐�.
	bool IsAnimationRatioEnd(const float fRatio);

	//�A�j���[�V�����̍Đ����x��ς���.
	void SetAnimationSpeed(const double dAnimationSpeed);

	//���݂̍s���̏����̑O�ɒǉ����鏈��.
	virtual void UpdateProduct() = 0;

	//���C�ƃ��b�V���̏Փˎ�.
	virtual void RayHitToMesh(clsDX9Mesh* const pTarget) = 0;

	virtual void Wait() = 0;

	virtual void Move() = 0;

	virtual void Attack() = 0;

	virtual void Hit() = 0;

	virtual void Dead() = 0;

	//�A�N�V�����̐؂�ւ�.
	virtual void SetAction(const enAction Action) = 0;

private:
	//�A�j���[�V�����̍ő吔���擾.
	int GetAnimationQuantityMax();

	//�ʒu,��],�T�C�Y�����f���ɓK��.
	void UpdateState();
};