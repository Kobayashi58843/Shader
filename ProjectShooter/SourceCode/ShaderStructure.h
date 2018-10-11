#pragma once
#pragma warning( disable:4005 )

#include <d3dx9.h>

//========================================
//	���ʂ��ĕK�v�ȍ\����.
//========================================

struct ShaderPointer
{
	ID3D11VertexShader*	pVertexShader;	//���_�V�F�[�_.
	ID3D11PixelShader*	pPixelShader;	//�s�N�Z���V�F�[�_.
	ID3D11InputLayout*	pVertexLayout;	//���_���C�A�E�g.
};

struct ShaderEntryPoint
{
	char* pVertexShaderEntryPointName;
	char* pPixelShaderEntryPointName;
};

//========================================
//	�X�^�e�B�b�N���b�V���ɕK�v�ȍ\����.
//========================================

// _declspec() : DLL����(�֐��E�N���X�E�N���X�̃����o�֐���)�G�N�X�|�[�g����.
// align()     : (�����I��)16�o�C�g�Ŏg�p����.
#define ALIGN16 _declspec(align(16))

//���C�g���.
struct LIGHT
{
	D3DXVECTOR3	vPos;		//�ʒu.
	D3DXVECTOR3	vDir;		//����.
	D3DXMATRIX	mRot;		//��]�s��.
	float		fIntensity;	//���x(���邳).
};

//�R���X�^���g�o�b�t�@�̃A�v�����̒�`
//���V�F�[�_���̃R���X�^���g�o�b�t�@�ƈ�v���Ă���K�v����.
//���b�V���P�ʂœn�����.
struct CBUFF_PER_MESH
{
	D3DXMATRIX mW;		//���[���h(�ʒu)���W�s��.
	D3DXMATRIX mWVP;	//���[���h,�r���[,�ˉe�̍����ϊ��s��.
};

//�}�e���A���P�ʂœn�����.
struct CBUFF_PER_MATERIAL
{
	D3DXVECTOR4	vAmbient;	//�A���r�G���g.
	D3DXVECTOR4	vDiffuse;	//�f�B�t���[�Y.
	D3DXVECTOR4	vSpecular;	//�X�y�L����.
};

//�t���[���P�ʂœn����� : ���C�g���͂����ɓ����.
struct CBUFF_PER_FRAME
{
	D3DXVECTOR4			vEye;		//�J�����ʒu(���_�ʒu).
	D3DXVECTOR4			vLightDir;	//���C�g����.
	ALIGN16 float		fIntensity;	//���C�g�̋��x(���邳)��x�����g��Ȃ�.
};

//���_�̍\����.
struct MeshVertex
{
	D3DXVECTOR3	vPos;	//�ʒu.
	D3DXVECTOR3	vNormal;//�@��(�A�e�v�Z�ɕK�{)
	D3DXVECTOR2	vTex;	//�e�N�X�`�����W.
};

//�}�e���A���\����.
struct MY_MATERIAL
{
	D3DXVECTOR4	Ambient;	//�A���r�G���g.
	D3DXVECTOR4	Diffuse;	//�f�B�t���[�Y.
	D3DXVECTOR4	Specular;	//�X�y�L����.
	CHAR szTextureName[128];//�e�N�X�`���t�@�C����.
	ID3D11ShaderResourceView* pTexture;//�e�N�X�`��.
	DWORD dwNumFace;	//���̃}�e���A���̃|���S����.

	//�R���X�g���N�^.
	MY_MATERIAL()
	{
		ZeroMemory(this, sizeof(MY_MATERIAL));
	}
	//�f�X�g���N�^.
	~MY_MATERIAL()
	{
		SAFE_RELEASE(pTexture);
	}
};

//========================================
//	�X�L�����b�V���ɕK�v�ȍ\����.
//========================================