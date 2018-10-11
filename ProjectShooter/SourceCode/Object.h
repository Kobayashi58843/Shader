#pragma once
#pragma warning( disable:4005 )

#include "Global.h"

#include "DX9Mesh.h"
#include "CD3DXSKINMESH.h"

//�X�t�B�A�\����.
struct SPHERE
{
	//���S.
	D3DXVECTOR3 vCenter;
	//���a.
	float fRadius;
};

class Object
{
public:
	Object();
	virtual ~Object();

	D3DXVECTOR3 GetPos() const
	{
		return m_vPos;
	}

	//���C�ƑΏۂ̃��b�V�����Փ˂��Ă��邩.
	bool IsRayHit(clsDX9Mesh* const pTarget);

	//���f�������X�ɖړI�̕����։�].
	void SpinModel(double dDestinationYaw, const float fTurnSpeed);

	//Sphere��Sphere�̓����蔻��.
	bool SphereCollision(SPHERE const pAttacker, SPHERE const pTarget);

	void AddPitch(const double fDegree)
	{
		m_vRot.x += (float)D3DXToRadian(fDegree);
	}
	void AddYaw(const double fDegree)
	{
		m_vRot.y += (float)D3DXToRadian(fDegree);
	}
	void AddRoll(const double fDegree)
	{
		m_vRot.z += (float)D3DXToRadian(fDegree);
	}

	D3DXVECTOR3 GetRayIntersect() const
	{
		return m_vIntersect;
	}

protected:
	//�����蔻��p.
	SPHERE m_Collision;

	//�ʒu.
	D3DXVECTOR3 m_vPos;

	//���f���̉�].
	D3DXVECTOR3 m_vRot;

	//�T�C�Y.
	float m_fScale;

	//�ʒu,��],�T�C�Y��K��.
	virtual void UpdateState() = 0;

private:
	//���C�̌������W.
	D3DXVECTOR3 m_vIntersect;

	//���C�ƃ��b�V���̓����蔻��.
	bool Intersect(const D3DXVECTOR3 vAxis, clsDX9Mesh* const pTarget, float* pfDistance, D3DXVECTOR3* pvIntersect);

	//�����ʒu�̃|���S���̒��_��������.
	HRESULT FindVecticesOnPoly(const LPD3DXMESH pTarget, const DWORD dwPolyIndex, D3DXVECTOR3* pVecVertices);

	//�ǂ̓����蔻��ԘA.
	void PrtWall(clsDX9Mesh* const pWall, bool* bHit);

	//��]�l����.
	void dirOverGuard(float* fYaw);
};