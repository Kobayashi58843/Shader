#pragma once
#pragma warning( disable:4005 )

#include "Global.h"

#include "DX9Mesh.h"
#include "CD3DXSKINMESH.h"

//スフィア構造体.
struct SPHERE
{
	//中心.
	D3DXVECTOR3 vCenter;
	//半径.
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

	//レイと対象のメッシュが衝突しているか.
	bool IsRayHit(clsDX9Mesh* const pTarget);

	//モデルを徐々に目的の方向へ回転.
	void SpinModel(double dDestinationYaw, const float fTurnSpeed);

	//Sphere対Sphereの当たり判定.
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
	//当たり判定用.
	SPHERE m_Collision;

	//位置.
	D3DXVECTOR3 m_vPos;

	//モデルの回転.
	D3DXVECTOR3 m_vRot;

	//サイズ.
	float m_fScale;

	//位置,回転,サイズを適応.
	virtual void UpdateState() = 0;

private:
	//レイの交差座標.
	D3DXVECTOR3 m_vIntersect;

	//レイとメッシュの当たり判定.
	bool Intersect(const D3DXVECTOR3 vAxis, clsDX9Mesh* const pTarget, float* pfDistance, D3DXVECTOR3* pvIntersect);

	//交差位置のポリゴンの頂点を見つける.
	HRESULT FindVecticesOnPoly(const LPD3DXMESH pTarget, const DWORD dwPolyIndex, D3DXVECTOR3* pVecVertices);

	//壁の当たり判定間連.
	void PrtWall(clsDX9Mesh* const pWall, bool* bHit);

	//回転値調整.
	void dirOverGuard(float* fYaw);
};