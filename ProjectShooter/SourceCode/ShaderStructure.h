#pragma once
#pragma warning( disable:4005 )

#include <d3dx9.h>

//========================================
//	共通して必要な構造体.
//========================================

struct ShaderPointer
{
	ID3D11VertexShader*	pVertexShader;	//頂点シェーダ.
	ID3D11PixelShader*	pPixelShader;	//ピクセルシェーダ.
	ID3D11InputLayout*	pVertexLayout;	//頂点レイアウト.
};

struct ShaderEntryPoint
{
	char* pVertexShaderEntryPointName;
	char* pPixelShaderEntryPointName;
};

//========================================
//	スタティックメッシュに必要な構造体.
//========================================

// _declspec() : DLLから(関数・クラス・クラスのメンバ関数を)エクスポートする.
// align()     : (強制的に)16バイトで使用する.
#define ALIGN16 _declspec(align(16))

//ライト情報.
struct LIGHT
{
	D3DXVECTOR3	vPos;		//位置.
	D3DXVECTOR3	vDir;		//方向.
	D3DXMATRIX	mRot;		//回転行列.
	float		fIntensity;	//強度(明るさ).
};

//コンスタントバッファのアプリ側の定義
//※シェーダ内のコンスタントバッファと一致している必要あり.
//メッシュ単位で渡す情報.
struct CBUFF_PER_MESH
{
	D3DXMATRIX mW;		//ワールド(位置)座標行列.
	D3DXMATRIX mWVP;	//ワールド,ビュー,射影の合成変換行列.
};

//マテリアル単位で渡す情報.
struct CBUFF_PER_MATERIAL
{
	D3DXVECTOR4	vAmbient;	//アンビエント.
	D3DXVECTOR4	vDiffuse;	//ディフューズ.
	D3DXVECTOR4	vSpecular;	//スペキュラ.
};

//フレーム単位で渡す情報 : ライト情報はここに入れる.
struct CBUFF_PER_FRAME
{
	D3DXVECTOR4			vEye;		//カメラ位置(視点位置).
	D3DXVECTOR4			vLightDir;	//ライト方向.
	ALIGN16 float		fIntensity;	//ライトの強度(明るさ)※xしか使わない.
};

//頂点の構造体.
struct MeshVertex
{
	D3DXVECTOR3	vPos;	//位置.
	D3DXVECTOR3	vNormal;//法線(陰影計算に必須)
	D3DXVECTOR2	vTex;	//テクスチャ座標.
};

//マテリアル構造体.
struct MY_MATERIAL
{
	D3DXVECTOR4	Ambient;	//アンビエント.
	D3DXVECTOR4	Diffuse;	//ディフューズ.
	D3DXVECTOR4	Specular;	//スペキュラ.
	CHAR szTextureName[128];//テクスチャファイル名.
	ID3D11ShaderResourceView* pTexture;//テクスチャ.
	DWORD dwNumFace;	//そのマテリアルのポリゴン数.

	//コンストラクタ.
	MY_MATERIAL()
	{
		ZeroMemory(this, sizeof(MY_MATERIAL));
	}
	//デストラクタ.
	~MY_MATERIAL()
	{
		SAFE_RELEASE(pTexture);
	}
};

//========================================
//	スキンメッシュに必要な構造体.
//========================================