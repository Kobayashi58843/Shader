#pragma once
#pragma warning( disable:4005 )

#include "Global.h"

class Direct3D
{
public:
	Direct3D();
	~Direct3D();

	ID3D11Device* GetDevice() const
	{
		return m_pDevice;
	}

	ID3D11DeviceContext* GetDeviceContext() const
	{
		return m_pDeviceContext;
	}

	//Direct3D�쐬.
	void Create(HWND hWnd);

	//���X�^���C�Y�ݒ�.
	void SetRasterize(D3D11_CULL_MODE CullMode);

	//�[�x�e�X�g(Z�e�X�g) ON/OFF�ؑ�.
	void SetDepth(bool bFlg);

	//�o�b�N�o�b�t�@�̃N���A.
	void ClearBackBuffer(D3DXVECTOR4 vClearColor = { 0.0f, 0.0f, 1.0f, 1.0f });

	//�����_�����O���ꂽ�C���[�W��\��.
	void RenderSwapChain();

private:
	//�f�o�C�X�I�u�W�F�N�g.
	ID3D11Device*				m_pDevice;
	//�f�o�C�X�R���e�L�X�g.
	ID3D11DeviceContext*		m_pDeviceContext;
	//�X���b�v�`�F�[��.
	IDXGISwapChain*				m_pSwapChain;
	//�����_�[�^�[�Q�b�g�r���[.
	ID3D11RenderTargetView*		m_pBackBuffer_RTV;
	//�o�b�N�o�b�t�@.
	ID3D11Texture2D*			m_pBackBuffer_Tex;
	//�f�v�X�X�e���V���r���[.
	ID3D11DepthStencilView*		m_pBackBuffer_DSV;
	//�[�x(Z)�e�X�g�ݒ�.
	ID3D11DepthStencilState*	m_pDepthStencilState;

};