#include "Sprite.h"

Sprite::Sprite(const float fDivisionQuantityWidth, const float fDivisionQuantityHeight)
	:m_vDivisionQuantity({fDivisionQuantityWidth, fDivisionQuantityHeight})
{
	CrearVECTOR2(m_vPatternNo);
	CrearVECTOR2(m_vUV);
}

Sprite::~Sprite()
{
}

//最大値と現在の値を入れるとスプライトをゲージとして扱う.
void Sprite::SpriteAsGage(const int iValueMax, const int iValue, const float fMoveSpeed)
{
	float fPartition = (m_BaseSpriteState.Width / m_vDivisionQuantity.x) * m_fScale / iValueMax;
	float fUV_X = (m_BaseSpriteState.Width / m_vDivisionQuantity.x) * m_fScale - (fPartition * iValue);

	if (m_vUV.x != fUV_X)
	{
		//差が小さければ数値を合わせる.
		if (fabs(m_vUV.x - fUV_X) < fMoveSpeed)
		{
			m_vUV.x = fUV_X;
		}

		if (m_vUV.x <= fUV_X)
		{
			AddUVWidth(fMoveSpeed);
		}
		else if (m_vUV.x > fUV_X)
		{
			AddUVWidth(-fMoveSpeed);
		}
	}
}

//板ポリ(四角形)の頂点を作成.
void Sprite::CreateVertex()
{
	assert(m_vDivisionQuantity.x != 0.0f);
	assert(m_vDivisionQuantity.y != 0.0f);

	//表示スプライト幅、高さ.
	float fW = (m_BaseSpriteState.Width / m_vDivisionQuantity.x) * m_fScale;
	float fH = (m_BaseSpriteState.Height / m_vDivisionQuantity.y) * m_fScale;

	float fU = (m_BaseSpriteState.Width / m_vDivisionQuantity.x) / m_BaseSpriteState.Width;
	float fV = (m_BaseSpriteState.Height / m_vDivisionQuantity.y) / m_BaseSpriteState.Height;

	//板ポリ(四角形)の頂点を作成.
	Sprite2DVertex vertices[] =
	{
		D3DXVECTOR3(-fW / 2, fH / 2, 0.0f), D3DXVECTOR2(0.0f, fV),
		D3DXVECTOR3(-fW / 2, -fH / 2, 0.0f), D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR3(fW / 2, fH / 2, 0.0f), D3DXVECTOR2(fU, fV),
		D3DXVECTOR3(fW / 2, -fH / 2, 0.0f), D3DXVECTOR2(fU, 0.0f)
	};
	//最大要素数を算出する.
	m_uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	for (unsigned int i = 0; i < m_uVerMax; i++)
	{
		m_Vertices[i] = vertices[i];
	}
}

//ワールド変換.
void Sprite::ConversionWorld(D3DMATRIX &mWorld)
{
	D3DXMATRIX mTrans;
	D3DXMATRIX mYaw, mPitch, mRoll;
	D3DXMATRIX mScale;

	//サイズ.
	D3DXMatrixScaling(&mScale, m_fScale, m_fScale, m_fScale);

	//回転.
	D3DXMatrixRotationX(&mPitch, m_vRot.x);
	D3DXMatrixRotationY(&mYaw, m_vRot.y);
	D3DXMatrixRotationZ(&mRoll, m_vRot.z);

	//平行移動.
	D3DXMatrixTranslation(&mTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	mWorld = mScale * mPitch * mYaw * mRoll * mTrans;
}

//シェーダのコンスタントバッファに各種データを渡す.
void Sprite::InputConstantBuffer(const D3DMATRIX mWorld)
{
	D3D11_MAPPED_SUBRESOURCE pData;
	Sprite2DConstantBuffer cd;	//コンスタントバッファ.
	//バッファ内のデータの書き方開始時にmap.
	if (SUCCEEDED(m_pDeviceContext11->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド行列を渡す.
		D3DXMATRIX m = mWorld;
		D3DXMatrixTranspose(&m, &m);//行列を転置する.					
		//行列の計算方法がDirectXとGPUで異なる為.
		cd.mW = m;

		//ビューポートの幅、高さを渡す.
		cd.fViewPortWidth = g_fWindowWidth;
		cd.fViewPortHeight = g_fWindowHeight;

		//アルファ値を渡す.
		cd.fAlpha = m_fAlpha;

		//UV座標を渡す.
		cd.vUV.x = (((m_BaseSpriteState.Width / m_vDivisionQuantity.x) / m_BaseSpriteState.Width) * m_vPatternNo.x)
			+ (m_vUV.x / m_BaseSpriteState.Width);

		cd.vUV.y = (((m_BaseSpriteState.Height / m_vDivisionQuantity.y) / m_BaseSpriteState.Height) * m_vPatternNo.y)
			+ (m_vUV.y / m_BaseSpriteState.Height);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cd), sizeof(cd));

		m_pDeviceContext11->Unmap(m_pConstantBuffer, 0);
	}

	//このコンスタントバッファをどのシェーダで使うか？.
	m_pDeviceContext11->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext11->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
}