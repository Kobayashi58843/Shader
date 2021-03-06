#pragma once
#pragma warning( disable:4005 )

#include "Object.h"
#include "CD3DXSKINMESH.h"

class Character : public Object
{
public:
	Character();
	virtual ~Character();

	//更新.
	void Update();

	//3Dモデルの描画.
	void RenderModel(const D3DXMATRIX &mView, const D3DXMATRIX &mProj, const D3DXVECTOR3 &vLight, const D3DXVECTOR3 &vEye);

	//モデルのYawを使って前後に移動.
	void MoveFrontRear(const float fSpeed);

	//モデルのYawを使って左右に移動.
	void MoveLeftRight(const float fSpeed);

protected:
	//スキンメッシュのモデルに必要な情報.
	struct SkinMeshState
	{	
		//再生しているアニメーションの番号.
		int iIndex;

		//アニメーションコントローラ.
		LPD3DXANIMATIONCONTROLLER AnimationController;

		//モデルのアニメーションの再生速度.
		double dAnimationSpeed;

		//モデルのアニメーションの現在の時間.
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

	//使いたいモデルのポインタ.
	clsD3DXSKINMESH* m_pModel;

	SkinMeshState m_ModelState;

	void AttachModel(clsD3DXSKINMESH* const pModel);
	void DetatchModel();

	//アニメーション切り替え関数.
	void ChangeAnimation(const int iIndex, const double dStartPos = 0.0f);

	//現在のアニメーションの終了を確認する関数.
	bool IsAnimationEnd();

	//現在のアニメーションが何割終了したかを確認する関数.
	bool IsAnimationRatioEnd(const float fRatio);

	//アニメーションの再生速度を変える.
	void SetAnimationSpeed(const double dAnimationSpeed);

	//現在の行動の処理の前に追加する処理.
	virtual void UpdateProduct() = 0;

	//レイとメッシュの衝突時.
	virtual void RayHitToMesh(clsDX9Mesh* const pTarget) = 0;

	virtual void Wait() = 0;

	virtual void Move() = 0;

	virtual void Attack() = 0;

	virtual void Hit() = 0;

	virtual void Dead() = 0;

	//アクションの切り替え.
	virtual void SetAction(const enAction Action) = 0;

private:
	//アニメーションの最大数を取得.
	int GetAnimationQuantityMax();

	//位置,回転,サイズをモデルに適応.
	void UpdateState();
};