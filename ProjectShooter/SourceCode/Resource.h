#pragma once
#pragma warning( disable:4005 )

#include "DX9Mesh.h"
#include "CD3DXSKINMESH.h"

/*=-=-=-=-=-=-=-=-=/ ���\�[�X�N���X /=-=-=-=-=-=-=-=-=*/
class Resource
{
public:
	//�X�^�e�B�b�N���f�����.
	enum enStaticModel
	{
		enStaticModel_Ground = 0,
		enStaticModel_Shpere,

		enStaticModel_Max,
	};
	//�X�L�����f�����.
	enum enSkinModel
	{
		enSkinModel_Player = 0,
		enSkinModel_Enemy,
		
		enSkinModel_Max,
	};

	static Resource* GetInstance()
	{
		static Resource s_Instance;
		return &s_Instance;
	}
	~Resource();

	/*====/ �X�^�e�B�b�N���f�� /====*/
	clsDX9Mesh*	GetStaticModels(enStaticModel enModel);
	void		CreateStaticModelResouce(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	/*====/ �X�L�����f�� /====*/
	clsD3DXSKINMESH*	GetSkinModels(enSkinModel enModel);
	void				CreateSkinModelResouce(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

private:
	Resource();
	Resource(const Resource& rhs);
	Resource& operator = (const Resource& rhs);

	HWND					m_hWnd;
	ID3D11Device*			m_pDevice11;
	ID3D11DeviceContext*	m_pContext11;
	CD3DXSKINMESH_INIT		si;

	clsDX9Mesh**			m_ppStaticModels;
	clsD3DXSKINMESH**		m_ppSkinModels;

	/*====/ �X�^�e�B�b�N���f�� /====*/
	HRESULT	InitStaticModel(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT	CreateStaticModel(LPSTR fileName, enStaticModel enModel);
	HRESULT	ReleaseStaticModel(enStaticModel enModel);
	bool	IsRengeStaticModel(enStaticModel enModel);

	/*====/ �X�L�����f�� /====*/
	HRESULT	InitSkinModel(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT	CreateSkinModel(LPSTR fileName, enSkinModel enModel);
	HRESULT	ReleaseSkinModel(enSkinModel enModel);
	bool	IsRengeSkinModel(enSkinModel enModel);
};
