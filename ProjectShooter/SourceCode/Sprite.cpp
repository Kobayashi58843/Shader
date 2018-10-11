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

//�ő�l�ƌ��݂̒l������ƃX�v���C�g���Q�[�W�Ƃ��Ĉ���.
void Sprite::SpriteAsGage(const int iValueMax, const int iValue, const float fMoveSpeed)
{
	float fPartition = (m_BaseSpriteState.Width / m_vDivisionQuantity.x) * m_fScale / iValueMax;
	float fUV_X = (m_BaseSpriteState.Width / m_vDivisionQuantity.x) * m_fScale - (fPartition * iValue);

	if (m_vUV.x != fUV_X)
	{
		//������������ΐ��l�����킹��.
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

//�|��(�l�p�`)�̒��_���쐬.
void Sprite::CreateVertex()
{
	assert(m_vDivisionQuantity.x != 0.0f);
	assert(m_vDivisionQuantity.y != 0.0f);

	//�\���X�v���C�g���A����.
	float fW = (m_BaseSpriteState.Width / m_vDivisionQuantity.x) * m_fScale;
	float fH = (m_BaseSpriteState.Height / m_vDivisionQuantity.y) * m_fScale;

	float fU = (m_BaseSpriteState.Width / m_vDivisionQuantity.x) / m_BaseSpriteState.Width;
	float fV = (m_BaseSpriteState.Height / m_vDivisionQuantity.y) / m_BaseSpriteState.Height;

	//�|��(�l�p�`)�̒��_���쐬.
	Sprite2DVertex vertices[] =
	{
		D3DXVECTOR3(-fW / 2, fH / 2, 0.0f), D3DXVECTOR2(0.0f, fV),
		D3DXVECTOR3(-fW / 2, -fH / 2, 0.0f), D3DXVECTOR2(0.0f, 0.0f),
		D3DXVECTOR3(fW / 2, fH / 2, 0.0f), D3DXVECTOR2(fU, fV),
		D3DXVECTOR3(fW / 2, -fH / 2, 0.0f), D3DXVECTOR2(fU, 0.0f)
	};
	//�ő�v�f�����Z�o����.
	m_uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	for (unsigned int i = 0; i < m_uVerMax; i++)
	{
		m_Vertices[i] = vertices[i];
	}
}

//���[���h�ϊ�.
void Sprite::ConversionWorld(D3DMATRIX &mWorld)
{
	D3DXMATRIX mTrans;
	D3DXMATRIX mYaw, mPitch, mRoll;
	D3DXMATRIX mScale;

	//�T�C�Y.
	D3DXMatrixScaling(&mScale, m_fScale, m_fScale, m_fScale);

	//��].
	D3DXMatrixRotationX(&mPitch, m_vRot.x);
	D3DXMatrixRotationY(&mYaw, m_vRot.y);
	D3DXMatrixRotationZ(&mRoll, m_vRot.z);

	//���s�ړ�.
	D3DXMatrixTranslation(&mTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	mWorld = mScale * mPitch * mYaw * mRoll * mTrans;
}

//�V�F�[�_�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
void Sprite::InputConstantBuffer(const D3DMATRIX mWorld)
{
	D3D11_MAPPED_SUBRESOURCE pData;
	Sprite2DConstantBuffer cd;	//�R���X�^���g�o�b�t�@.
	//�o�b�t�@���̃f�[�^�̏������J�n����map.
	if (SUCCEEDED(m_pDeviceContext11->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�s���n��.
		D3DXMATRIX m = mWorld;
		D3DXMatrixTranspose(&m, &m);//�s���]�u����.					
		//�s��̌v�Z���@��DirectX��GPU�ňقȂ��.
		cd.mW = m;

		//�r���[�|�[�g�̕��A������n��.
		cd.fViewPortWidth = g_fWindowWidth;
		cd.fViewPortHeight = g_fWindowHeight;

		//�A���t�@�l��n��.
		cd.fAlpha = m_fAlpha;

		//UV���W��n��.
		cd.vUV.x = (((m_BaseSpriteState.Width / m_vDivisionQuantity.x) / m_BaseSpriteState.Width) * m_vPatternNo.x)
			+ (m_vUV.x / m_BaseSpriteState.Width);

		cd.vUV.y = (((m_BaseSpriteState.Height / m_vDivisionQuantity.y) / m_BaseSpriteState.Height) * m_vPatternNo.y)
			+ (m_vUV.y / m_BaseSpriteState.Height);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cd), sizeof(cd));

		m_pDeviceContext11->Unmap(m_pConstantBuffer, 0);
	}

	//���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�Ŏg�����H.
	m_pDeviceContext11->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext11->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
}