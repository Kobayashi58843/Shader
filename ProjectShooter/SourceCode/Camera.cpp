#include"Camera.h"

//注視点からカメラまでの距離.
const float g_fFocusingSpaceDistance = 5.0f;
//回転速度.
const float g_fDegree = 1.0f;

//視野の広さ.
const float g_fScope = 4.0f;
//描画する最大の距離.
const float g_fLookDistance = 500.0f;

//上下の限界の角度.
const int g_iMaxAngleDegree = 10;

//どれだけ横にずらすか.
const float g_fDisplaceHorizontally = 0.8f;

//距離の限界.
const float g_fDistanceMax = 8.0f;

//当たり判定の大きさ.
const float g_fRadius = 0.2f;

Camera::Camera(const float fWindowWidth, const float fWindowHeight)
	: m_fWindowWidth(fWindowWidth)
	, m_fWindowHeight(fWindowHeight)
{
	CrearVECTOR3(m_vFocusingSpacePos);
	m_vFocusingSpacePos.z = -g_fFocusingSpaceDistance;

	CrearVECTOR3(m_vWorldPos);

	D3DXMatrixIdentity(&m_mView);
	D3DXMatrixIdentity(&m_mCameraPose);
	D3DXMatrixIdentity(&m_mProjection);

	CrearVECTOR3(m_vLookAt);
	CrearVECTOR3(m_vCameraMoveDirDif);
	CrearVECTOR3(m_vCamRotDif);
	SetRotPerFrame(g_fDegree);
	
	m_fCameraAngleUnit = 0.0f;

	m_fOffsetZ = 0.0f;

	m_fDisplaceHorizontally = 0.0f;

	CrearVECTOR3(m_vPos);
	CrearVECTOR3(m_vRot);
	m_Collision.fRadius = g_fRadius;
	m_Collision.vCenter = m_vPos;

	m_vOldFocusingSpacePos = m_vPos + m_vFocusingSpacePos;
}

Camera::~Camera()
{
}

void Camera::Update(clsDX9Mesh* const pTarget)
{
	//カメラZ軸回転で姿勢仮更新.
	D3DXMATRIX mRotMatV;
	D3DXMatrixRotationYawPitchRoll(&mRotMatV, m_vCamRotDif.y, m_vCamRotDif.x, m_vCamRotDif.z);
	m_mCameraPose = mRotMatV * m_mCameraPose;

	//緯度経度の差分を出す.
	D3DXVECTOR3 vDif;
	D3DXVec3TransformCoord(&vDif, &m_vCameraMoveDirDif, &m_mCameraPose);

	//差分がある時のみ移動をする.
	if (vDif.x != 0.0f || vDif.y != 0.0f || vDif.z != 0.0f)
	{
		//移動.
		Move(vDif);
	}

	//奥行き方向のオフセット更新.
	OffsetZUpdate();

	//移動制限.
	MoveLimit(pTarget);

	//移動後カメラ姿勢更新.
	PostureUpdate();

	//カメラのワールド座標.
	m_vWorldPos = m_vFocusingSpacePos + m_vLookAt;

	//ビュー行列更新.
	memcpy(&m_mView, &m_mCameraPose, sizeof(D3DXMATRIX));
	memcpy(&m_mView.m[3], &m_vWorldPos, sizeof(D3DXVECTOR3));
	m_mView._44 = 1.0f;
	D3DXMatrixInverse(&m_mView, 0, &m_mView);

	//横にずらす.
	m_mView.m[3][0] += g_fDisplaceHorizontally;

	//プロジェクション(射影行列)変換.
	Projection();

	//差分メンバをリセット.
	CrearVECTOR3(m_vCameraMoveDirDif);
	m_fOffsetZ = 0.0f;
	CrearVECTOR3(m_vCamRotDif);
}

//プロジェクション(射影行列)変換.
void Camera::Projection()
{
	D3DXMatrixPerspectiveFovLH(&m_mProjection, (FLOAT)(D3DX_PI / g_fScope),
		m_fWindowWidth / m_fWindowHeight,//アスペクト比(幅÷高さ).
		0.1f, g_fLookDistance);
}

//移動.
void Camera::Move(const D3DXVECTOR3 vDif)
{
	D3DXVECTOR3 vRotAxis;
	D3DXVECTOR3* camZAxis = (D3DXVECTOR3*)m_mCameraPose.m[2];
	D3DXVec3Cross(&vRotAxis, &vDif, camZAxis);

	D3DXQUATERNION qTrans;
	D3DXQuaternionRotationAxis(&qTrans, &vRotAxis, m_fCameraAngleUnit);
	D3DXMATRIX mTransRot;
	D3DXMatrixRotationQuaternion(&mTransRot, &qTrans);
	D3DXVec3TransformCoord(&m_vFocusingSpacePos, &m_vFocusingSpacePos, &mTransRot);

	//移動後の位置をレイ用のものに入れる.
	UpdateState();
}

//移動制限.
void Camera::MoveLimit(clsDX9Mesh* const pTarget)
{
	float fDistance = D3DXVec3Length(&m_vFocusingSpacePos);
	float fCamAngleY = atan(fDistance / m_vFocusingSpacePos.y);

	if (D3DXToRadian(90) - fabs(fCamAngleY) < D3DXToRadian(g_iMaxAngleDegree))
	{
		//注視点空間でのカメラの位置を保存.
		m_vOldFocusingSpacePos = m_vFocusingSpacePos;
	}
	else
	{
		//範囲外の場合は前の位置に戻す.
		m_vFocusingSpacePos = m_vOldFocusingSpacePos;
	}
}

//姿勢更新.
void Camera::PostureUpdate()
{
	D3DXVECTOR3 vCameraPoseX, vCameraPoseY, vCameraPoseZ;

	vCameraPoseZ = -m_vFocusingSpacePos;
	D3DXVec3Normalize(&vCameraPoseZ, &vCameraPoseZ);

	vCameraPoseY = { 0.0f, 1.0f, 0.0f };
	D3DXVec3Cross(&vCameraPoseX, &vCameraPoseY, &vCameraPoseZ);
	D3DXVec3Normalize(&vCameraPoseX, &vCameraPoseX);

	D3DXVec3Cross(&vCameraPoseY, &vCameraPoseZ, &vCameraPoseX);
	D3DXVec3Normalize(&vCameraPoseY, &vCameraPoseY);

	D3DXMatrixIdentity(&m_mCameraPose);
	memcpy(m_mCameraPose.m[0], &vCameraPoseX, sizeof(D3DXVECTOR3));
	memcpy(m_mCameraPose.m[1], &vCameraPoseY, sizeof(D3DXVECTOR3));
	memcpy(m_mCameraPose.m[2], &vCameraPoseZ, sizeof(D3DXVECTOR3));
}

//奥行き方向のオフセット更新.
void Camera::OffsetZUpdate()
{
	D3DXVECTOR3 vCameraPoseZ;
	vCameraPoseZ = -m_vFocusingSpacePos;
	D3DXVec3Normalize(&vCameraPoseZ, &vCameraPoseZ);

	float fDistance = D3DXVec3Length(&m_vFocusingSpacePos);

	if (fDistance - m_fOffsetZ <= g_fDistanceMax &&
		fDistance - m_fOffsetZ > 0.0f)
	{
		m_vFocusingSpacePos += m_fOffsetZ * vCameraPoseZ;
	}
}

//位置,回転,サイズを適応.
void Camera::UpdateState()
{
	m_vPos = m_vFocusingSpacePos + m_vLookAt;
	m_Collision.vCenter = m_vPos;
}