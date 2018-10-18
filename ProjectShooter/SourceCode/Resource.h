#pragma once
#pragma warning( disable:4005 )

#include "DX9Mesh.h"
#include "CD3DXSKINMESH.h"

/*=-=-=-=-=-=-=-=-=/ リソースクラス /=-=-=-=-=-=-=-=-=*/
class Resource
{
public:
	//スタティックモデル種類.
	enum enStaticModel
	{
		enStaticModel_Ground = 0,
		enStaticModel_Shpere,

		enStaticModel_Max,
	};
	//スキンモデル種類.
	enum enSkinModel
	{
		enSkinModel_Player = 0,
		enSkinModel_Enemy,
		
		enSkinModel_Max,
	};

	Resource();
	~Resource();

	void Create(const HWND hWnd, ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext);

	/*====/ スタティックモデル /====*/
	clsDX9Mesh*	GetStaticModels(enStaticModel enModel);

	/*====/ スキンモデル /====*/
	clsD3DXSKINMESH*	GetSkinModels(enSkinModel enModel);

private:
	HWND					m_hWnd;
	ID3D11Device*			m_pDevice11;
	ID3D11DeviceContext*	m_pContext11;
	CD3DXSKINMESH_INIT		si;

	clsDX9Mesh**			m_ppStaticModels;
	clsD3DXSKINMESH**		m_ppSkinModels;

	/*====/ スタティックモデル /====*/
	void	CreateStaticModelResouce(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT	InitStaticModel(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT	CreateStaticModel(LPSTR fileName, enStaticModel enModel);
	HRESULT	ReleaseStaticModel(enStaticModel enModel);
	bool	IsRengeStaticModel(enStaticModel enModel);

	/*====/ スキンモデル /====*/
	void	CreateSkinModelResouce(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT	InitSkinModel(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT	CreateSkinModel(LPSTR fileName, enSkinModel enModel);
	HRESULT	ReleaseSkinModel(enSkinModel enModel);
	bool	IsRengeSkinModel(enSkinModel enModel);
};
