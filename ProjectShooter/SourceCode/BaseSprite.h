#pragma once
#pragma warning( disable:4005 )

#include "Global.h"

#define ALIGN16 _declspec(align(16))

//�R���X�^���g�o�b�t�@�̃A�v�����̒�`(Sprite2D.hlsl).
struct Sprite2DConstantBuffer
{
	ALIGN16 D3DXMATRIX	mW;				//���[���h�s��.
	ALIGN16 float		fViewPortWidth;	//�r���[�|�[�g��.
	ALIGN16 float		fViewPortHeight;//�r���[�|�[�g����.
	ALIGN16 float		fAlpha;			//�A���t�@�l(���߂Ŏg�p).
	ALIGN16 D3DXVECTOR2	vUV;			//UV���W.
};
//���_�̍\����.
struct Sprite2DVertex
{
	D3DXVECTOR3	vPos;	//�ʒu.
	D3DXVECTOR2	vTex;	//�e�N�X�`�����W.
};

class BaseSprite
{
public:
	BaseSprite();		//�R���X�g���N�^.
	virtual ~BaseSprite();	//�f�X�g���N�^.

	//������.
	HRESULT Create(ID3D11Device* const pDevice11, ID3D11DeviceContext* const pContext11, const LPSTR FileName);

	//�`��(�����_�����O)
	void Render();

protected:
	ID3D11Device*				m_pDevice11;		//�f�o�C�X�I�u�W�F�N�g.
	ID3D11DeviceContext*		m_pDeviceContext11;	//�f�o�C�X�R���e�L�X�g.

	ID3D11VertexShader*			m_pVertexShader;	//���_�V�F�[�_.
	ID3D11InputLayout*			m_pVertexLayout;	//���_���C�A�E�g.
	ID3D11PixelShader*			m_pPixelShader;		//�s�N�Z���V�F�[�_.
	ID3D11Buffer*				m_pConstantBuffer;	//�R���X�^���g�o�b�t�@.

	ID3D11Buffer*				m_pVertexBuffer;	//���_�o�b�t�@.

	ID3D11ShaderResourceView*	m_pTexture;			//�e�N�X�`��.
	ID3D11SamplerState*			m_pSampleLinear;	//�e�N�X�`���̃T���v��-:�e�N�X�`���[�Ɋe��t�B���^��������.

	ID3D11BlendState*			m_pBlendState;		//�u�����h�X�e�[�g.

	D3DXIMAGE_INFO				m_BaseSpriteState;	//���摜�̏��.

	D3DXVECTOR3		m_vPos;			//�ʒu.
	D3DXVECTOR3		m_vRot;			//��].
	float			m_fScale;		//�T�C�Y.
	float			m_fAlpha;		//�A���t�@�l.
	bool			m_bDispFlg;		//�\���t���O.

	UINT			m_uVerMax;		//���_�̍ő吔.
	Sprite2DVertex	m_Vertices[4];

	//�V�F�[�_�쐬.
	HRESULT CreateShader();
	//���f���쐬.
	HRESULT CreateModel();
	//�e�N�X�`���쐬.
	HRESULT CreateTexture(const LPSTR FileName, ID3D11ShaderResourceView** const pTex);

	//����(�A���t�@�u�����h)�ݒ�̐؂�ւ�.
	void SetBlend(bool flg);

	//�|��(�l�p�`)�̒��_���쐬.
	virtual void CreateVertex() = 0;
	//���[���h�ϊ�.
	virtual void ConversionWorld(D3DMATRIX &mWorld) = 0;
	//�V�F�[�_�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	virtual void InputConstantBuffer(const D3DMATRIX mWorld) = 0;
};