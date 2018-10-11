#ifndef _DX9MESH_H_
#define _DX9MESH_H_
//�x���ɂ��ẴR�[�h���͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning( disable:4005 )

#include "ShaderGathering.h"

//���b�V���f�[�^���t�@�C��������o���ׂ����ɁADirectX9���g�p����.
//�������_�����O(�\��)�́ADirectX11�ōs��.
class clsDX9Mesh
{
public:
	HWND				m_hWnd;		//�E�B���h�E�n���h��.

	//Dx9.
	LPDIRECT3D9			m_pD3d;		//Dx9�I�u�W�F�N�g.
	LPDIRECT3DDEVICE9	m_pDevice9;	//Dx9�f�o�C�X�I�u�W�F�N�g.

	DWORD		m_dwNumMaterials;	//�}�e���A����.
	LPD3DXMESH	m_pMesh;			//���b�V���I�u�W�F�N�g.

	//Dx11.
	ID3D11Device*			m_pDevice11;		//�f�o�C�X�I�u�W�F�N�g.
	ID3D11DeviceContext*	m_pDeviceContext11;	//�f�o�C�X�R���e�L�X�g.

	ShaderPointer			m_Shader;

	ID3D11Buffer*			m_pCBuffPerMesh;	//�R���X�^���g�o�b�t�@(���b�V��).
	ID3D11Buffer*			m_pCBuffPerMaterial;//�R���X�^���g�o�b�t�@(�}�e���A��).
	ID3D11Buffer*			m_pCBuffPerFrame;	//�R���X�^���g�o�b�t�@(�t���[��).

	ID3D11Buffer*			m_pVertexBuffer;//���_(�o�[�e�b�N�X)�o�b�t�@.
	ID3D11Buffer**			m_ppIndexBuffer;//�C���f�b�N�X�o�b�t�@.
	ID3D11SamplerState*		m_pSampleLinear;//�e�N�X�`���̃T���v��-:�e�N�X�`���[�Ɋe��t�B���^��������.

	MY_MATERIAL*	m_pMaterials;	//�}�e���A���\����.
	DWORD			m_NumAttr;		//������.
	DWORD			m_AttrID[300];	//����ID ��300�����܂�.

	bool			m_bTexture;	//�e�N�X�`���̗L��.

	float			m_fScale;	//�g�k.
	float			m_fYaw;		//��](Y��).
	float			m_fPitch;	//��](X��).
	float			m_fRoll;	//��](Z��).
	D3DXVECTOR3		m_vPos;		//�ʒu(X,Y,Z).

	D3DXVECTOR3		m_vRay;			//���C�̈ʒu.
	D3DXVECTOR3		m_vAxis;		//��]��.
	LPD3DXMESH		m_pMeshForRay;	//���C�ƃ��b�V���p.

	ID3D11BlendState*	m_pBlendState;	//�u�����h�X�e�[�g.

	clsDX9Mesh();
	~clsDX9Mesh();

	HRESULT Init(HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, LPSTR fileName);
	//Dx9������.
	HRESULT InitDx9(HWND hWnd);
	//���b�V���Ǎ�.
	HRESULT LoadXMesh(LPSTR fileName);

	//�V�F�[�_�쐬.
	HRESULT InitShader();
	//�����_�����O�p.
	void Render(const D3DXMATRIX &mView, const D3DXMATRIX &mProj, const LIGHT &Light, const D3DXVECTOR3 &vEye);
	//���b�V���`��.
	void RenderMesh(const D3DXMATRIX &mWorld, const D3DXMATRIX &mView, const D3DXMATRIX &mProj);

	//����(�A���t�@�u�����h)�ݒ�̐؂�ւ�.
	void SetBlend(bool flg);
};


#endif//#ifndef _DX9MESH_H_