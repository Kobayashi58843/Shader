#pragma once
#pragma warning( disable:4005 )

#include "BaseSprite.h"

struct SpriteState
{
	//�t�@�C�����Ƃ����܂ł̃p�X.
	char sPath[MAX_PATH];
	//���摜�����������邩.
	D3DXVECTOR2 vDivisionQuantity;
};

class Sprite : public BaseSprite
{
public:
	Sprite(const float DivisionQuantityWidth, const float DivisionQuantityHeight);
	virtual ~Sprite();

	void SetPos(const float fX, const float fY)
	{
		m_vPos.x = fX;
		m_vPos.y = fY;
	}
	void AddPos(const float fXQuantity, const float fYQuantity)
	{
		m_vPos.x = fXQuantity;
		m_vPos.y = fYQuantity;
	}
	D3DXVECTOR3 GetPos() const
	{
		return m_vPos;
	}

	void SetRot(const D3DXVECTOR3 vRot)
	{
		m_vRot = vRot;
	}
	void AddPitch(const float fQuantity)
	{
		m_vRot.x += fQuantity;
		ClampRot(m_vRot.x);
	}
	void AddYaw(const float fQuantity)
	{
		m_vRot.y += fQuantity;
		ClampRot(m_vRot.y);
	}
	void AddRoll(const float fQuantity)
	{
		m_vRot.z += fQuantity;
		ClampRot(m_vRot.z);
	}
	D3DXVECTOR3 GetRot() const
	{
		return m_vRot;
	}

	void SetScale(const float fScale)
	{
		m_fScale = fScale;
	}
	void AddScale(const float fQuantity)
	{
		if (0.0f <= m_fScale + fQuantity)
		{
			m_fScale += fQuantity;
		}
	}
	float GetScale() const
	{
		return m_fScale;
	}
	D3DXVECTOR2 GetSize()
	{
		D3DXVECTOR2 vSize;
		vSize.x = (m_BaseSpriteState.Width / m_vDivisionQuantity.x) * m_fScale;
		vSize.y = (m_BaseSpriteState.Height / m_vDivisionQuantity.y) * m_fScale;
		return vSize;
	}

	void SetAlpha(const float fAlpha)
	{
		m_fAlpha = fAlpha;
	}
	void AddAlpha(const float fQuantity)
	{
		m_fAlpha += fQuantity;
		ClampSize(m_fAlpha, 0.0f, 1.0f);
	}
	float GetAlpha() const
	{
		return m_fAlpha;
	}

	void SetDispFlg(const bool bDispFlg)
	{
		m_bDispFlg = bDispFlg;
	}
	bool IsDispFlg() const
	{
		return m_bDispFlg;
	}

	void SetPatternNoWidth(const float fQuantity)
	{
		m_vPatternNo.x = fQuantity;
	}
	void SetPatternNoHeight(const float fQuantity)
	{
		m_vPatternNo.y = fQuantity;
	}
	void AddPatternNoWidth(const float fQuantity)
	{
		m_vPatternNo.x += fQuantity;
		ClampPatternNo(m_vPatternNo.x, 0.0f, m_vDivisionQuantity.x);
	}
	void AddPatternNoHeight(const float fQuantity)
	{
		m_vPatternNo.y += fQuantity;
		ClampPatternNo(m_vPatternNo.y, 0.0f, m_vDivisionQuantity.y);
	}
	D3DXVECTOR2 GetPatternNo() const
	{
		return m_vPatternNo;
	}

	void AddUVWidth(const float fQuantity)
	{
		m_vUV.x += fQuantity;
		ClampSize(m_vUV.x, 0.0f, (float)m_BaseSpriteState.Height);
	}
	void AddUVHeight(const float fQuantity)
	{
		m_vUV.y += fQuantity;
		ClampSize(m_vUV.y, 0.0f, (float)m_BaseSpriteState.Height);
	}

	//�ő�l�ƌ��݂̒l������ƃX�v���C�g���Q�[�W�Ƃ��Ĉ���.
	void SpriteAsGage(const int iValueMax, const int iValue, const float fMoveSpeed = 1.0f);

private:
	//�X�v���C�g�����������邩.
	D3DXVECTOR2 m_vDivisionQuantity;
	//���������X�v���C�g�̂ǂ���\�����邩.
	D3DXVECTOR2 m_vPatternNo;
	//���Z����UV.
	D3DXVECTOR2 m_vUV;

	//�|��(�l�p�`)�̒��_���쐬.
	void CreateVertex();
	//���[���h�ϊ�.
	void ConversionWorld(D3DMATRIX &mWorld);
	//�V�F�[�_�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	void InputConstantBuffer(const D3DMATRIX mWorld);

	//���l���ۂ߂�.
	void ClampSize(float &fOut,float fMin, float fMax)
	{
		if (fOut < fMin)
		{
			fOut = fMin;
		}
		if (fOut > fMax)
		{
			fOut = fMax;
		}
	}
	void ClampRot(float &fRot)
	{
		if (fRot > D3DXToRadian(360))
		{
			fRot -= (float)D3DXToRadian(360);
		}
		else if (fRot < 0.0f)
		{
			fRot += (float)D3DXToRadian(360);
		}
	}
	void ClampPatternNo(float &fPatternNo, float fMin, float fMax)
	{
		if (fPatternNo < fMin)
		{
			fPatternNo = fMax - 1.0f;
		}
		else if (fPatternNo >= fMax)
		{
			fPatternNo = fMin;
		}
	}
};