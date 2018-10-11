#ifndef _DX9MESH_H_
#define _DX9MESH_H_
//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )

#include "ShaderGathering.h"

//メッシュデータをファイルから取り出す為だけに、DirectX9を使用する.
//※レンダリング(表示)は、DirectX11で行う.
class clsDX9Mesh
{
public:
	HWND				m_hWnd;		//ウィンドウハンドル.

	//Dx9.
	LPDIRECT3D9			m_pD3d;		//Dx9オブジェクト.
	LPDIRECT3DDEVICE9	m_pDevice9;	//Dx9デバイスオブジェクト.

	DWORD		m_dwNumMaterials;	//マテリアル数.
	LPD3DXMESH	m_pMesh;			//メッシュオブジェクト.

	//Dx11.
	ID3D11Device*			m_pDevice11;		//デバイスオブジェクト.
	ID3D11DeviceContext*	m_pDeviceContext11;	//デバイスコンテキスト.

	ShaderPointer			m_Shader;

	ID3D11Buffer*			m_pCBuffPerMesh;	//コンスタントバッファ(メッシュ).
	ID3D11Buffer*			m_pCBuffPerMaterial;//コンスタントバッファ(マテリアル).
	ID3D11Buffer*			m_pCBuffPerFrame;	//コンスタントバッファ(フレーム).

	ID3D11Buffer*			m_pVertexBuffer;//頂点(バーテックス)バッファ.
	ID3D11Buffer**			m_ppIndexBuffer;//インデックスバッファ.
	ID3D11SamplerState*		m_pSampleLinear;//テクスチャのサンプラ-:テクスチャーに各種フィルタをかける.

	MY_MATERIAL*	m_pMaterials;	//マテリアル構造体.
	DWORD			m_NumAttr;		//属性数.
	DWORD			m_AttrID[300];	//属性ID ※300属性まで.

	bool			m_bTexture;	//テクスチャの有無.

	float			m_fScale;	//拡縮.
	float			m_fYaw;		//回転(Y軸).
	float			m_fPitch;	//回転(X軸).
	float			m_fRoll;	//回転(Z軸).
	D3DXVECTOR3		m_vPos;		//位置(X,Y,Z).

	D3DXVECTOR3		m_vRay;			//レイの位置.
	D3DXVECTOR3		m_vAxis;		//回転軸.
	LPD3DXMESH		m_pMeshForRay;	//レイとメッシュ用.

	ID3D11BlendState*	m_pBlendState;	//ブレンドステート.

	clsDX9Mesh();
	~clsDX9Mesh();

	HRESULT Init(HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, LPSTR fileName);
	//Dx9初期化.
	HRESULT InitDx9(HWND hWnd);
	//メッシュ読込.
	HRESULT LoadXMesh(LPSTR fileName);

	//シェーダ作成.
	HRESULT InitShader();
	//レンダリング用.
	void Render(const D3DXMATRIX &mView, const D3DXMATRIX &mProj, const LIGHT &Light, const D3DXVECTOR3 &vEye);
	//メッシュ描画.
	void RenderMesh(const D3DXMATRIX &mWorld, const D3DXMATRIX &mView, const D3DXMATRIX &mProj);

	//透過(アルファブレンド)設定の切り替え.
	void SetBlend(bool flg);
};


#endif//#ifndef _DX9MESH_H_