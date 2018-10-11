#include "BaseSprite.h"

//シェーダファイル名(ディレクトリも含む)
const char SHADER_NAME[] = "Shader\\Sprite2D.hlsl";

//コンストラクタ.
BaseSprite::BaseSprite()
{
	m_pDevice11 = nullptr;
	m_pDeviceContext11 = nullptr;

	m_pVertexShader = nullptr;
	m_pVertexLayout = nullptr;
	m_pPixelShader = nullptr;
	m_pConstantBuffer = nullptr;

	m_pVertexBuffer = nullptr;

	m_pTexture = nullptr;
	m_pSampleLinear = nullptr;
	m_pBlendState = nullptr;

	CrearVECTOR3(m_vPos);
	CrearVECTOR3(m_vRot);
	m_fScale = 1.0f;
	m_fAlpha = 1.0f;
	m_bDispFlg = true;

	m_uVerMax = 0;

	const int iVerticesMax = 4;
	for (unsigned int i = 0; i < iVerticesMax; i++)
	{
		CrearVECTOR3(m_Vertices[i].vPos);
		CrearVECTOR2(m_Vertices[i].vTex);
	}
}

//デストラクタ.
BaseSprite::~BaseSprite()
{
	m_pDeviceContext11 = nullptr;
	m_pDevice11 = nullptr;
}

//初期化.
HRESULT BaseSprite::Create(ID3D11Device* const pDevice11, ID3D11DeviceContext* const pContext11, const LPSTR FileName)
{
	m_pDevice11 = pDevice11;
	m_pDeviceContext11 = pContext11;

	//元画像の情報.
	D3DXGetImageInfoFromFile(FileName, &m_BaseSpriteState);

	if (FAILED(CreateShader()))
	{
		return E_FAIL;
	}
	if (FAILED(CreateModel()))
	{
		return E_FAIL;
	}
	if (FAILED(CreateTexture(FileName, &m_pTexture)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//板ポリゴンを画面にレンダリング.
void BaseSprite::Render()
{
	if (!m_bDispFlg)
	{
		return;
	}

	//ワールド行列.
	D3DXMATRIX mWorldMat;

	//初期化:単位行列作成.
	D3DXMatrixIdentity(&mWorldMat);

	//ワールド変換(表示位置を設定する).
	ConversionWorld(mWorldMat);

	//使用するシェーダの登録.
	m_pDeviceContext11->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext11->PSSetShader(m_pPixelShader, NULL, 0);

	//シェーダのコンスタントバッファに各種データを渡す.
	InputConstantBuffer(mWorldMat);

	//頂点バッファをセット.
	UINT stride = sizeof(Sprite2DVertex);
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//頂点インプットレイアウトをセット.
	m_pDeviceContext11->IASetInputLayout(m_pVertexLayout);

	//プリミティブ・トポロジーをセット.
	m_pDeviceContext11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャをシェーダに渡す.
	m_pDeviceContext11->PSSetSamplers(0, 1, &m_pSampleLinear);	//サンプラ-をセット.
	m_pDeviceContext11->PSSetShaderResources(0, 1, &m_pTexture);//テクスチャをシェーダに渡す.

	//アルファブレンド用ブレンドステート作成＆設定.
	SetBlend(true);

	//プリミティブをレンダリング.
	m_pDeviceContext11->Draw(4, 0);

	//アルファブレンドを無効にする.
	SetBlend(false);
}

//シェーダを作成する.
HRESULT BaseSprite::CreateShader()
{
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;
	UINT uCompileFlag = 0;

#ifdef _DEBUG
	uCompileFlag
		= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSLからバーテックスシェーダのブロブを作成.
	if (FAILED(
		D3DX11CompileFromFile(
		SHADER_NAME,	//シェーダファイル名(HLSLファイル)
		NULL,			//マクロ定義の配列へのポインタ(未使用)
		NULL,			//インクルードファイルを扱うインターフェイスへのポインタ(未使用)
		"VS_Main",		//シェーダエントリーポイント関数の名前.
		"vs_5_0",		//シェーダのモデルを指定する文字列(プロファイル)
		uCompileFlag,	//シェーダコンパイルフラグ.
		0,				//エフェクトコンパイルフラグ(未使用)
		NULL,			//スレッドポンプインターフェイスへのポインタ(未使用)
		&pCompiledShader,//ブロブを格納するメモリへのポインタ.
		&pErrors,		//エラーと警告一覧を格納するメモリへのポインタ.
		NULL)))		//戻り値へのポインタ(未使用)
	{
		MessageBox(NULL, "hlsl読込失敗", "エラー", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したブロブから「バーテックスシェーダ」を作成.
	if (FAILED(m_pDevice11->CreateVertexShader(
		pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),
		NULL,&m_pVertexShader)))//(out)バーテックスシェーダ.
	{
		MessageBox(NULL, "vs作成失敗", "エラー", MB_OK);
		return E_FAIL;
	}

	//頂点インプットレイアウトを定義.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",				//位置.
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,//DXGIのフォーマット(32bit float型*3)
			0,
			0,							//データの開始位置.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",		//テクスチャ位置.
			0,
			DXGI_FORMAT_R32G32_FLOAT,	//DXGIのフォーマット(32bit float型*2)
			0,
			12,							//テクスチャデータの開始位置.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};
	//頂点インプットレイアウトの配列要素数を算出.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点インプットレイアウトの作成.
	if (FAILED(m_pDevice11->CreateInputLayout(layout,numElements,
		pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),
		&m_pVertexLayout)))//(out)頂点インプットレイアウト.
	{
		MessageBox(NULL, "頂点インプットレイアウト作成失敗", "エラー", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSLからピクセルシェーダのブロブを作成.
	if (FAILED(
		D3DX11CompileFromFile(
		SHADER_NAME,	//シェーダファイル名(HLSLファイル)
		NULL, NULL,
		"PS_Main",		//シェーダエントリーポイント関数の名前.
		"ps_5_0",		//シェーダのモデルを指定する文字列(プロファイル)
		uCompileFlag,	//シェーダコンパイルフラグ.
		0, NULL,
		&pCompiledShader,//ブロブを格納するメモリへのポインタ.
		&pErrors,
		NULL)))
	{
		MessageBox(NULL, "hlsl(ps)読込失敗", "エラー", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したブロブから「ピクセルシェーダ」を作成.
	if (FAILED(m_pDevice11->CreatePixelShader(
		pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),
		NULL,&m_pPixelShader)))//(out)ピクセルシェーダ.
	{
		MessageBox(NULL, "ps作成失敗", "エラー", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);//ブロブ解放.

	//コンスタント(定数)バッファ作成 シェーダに特定の数値を送るバッファ.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//コンスタントバッファを指定.
	cb.ByteWidth = sizeof(Sprite2DConstantBuffer);//コンスタントバッファのサイズ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//書き込みでアクセス.
	cb.MiscFlags = 0;//その他のフラグ(未使用)
	cb.StructureByteStride = 0;//構造体のサイズ(未使用)
	cb.Usage = D3D11_USAGE_DYNAMIC;//使用方法:直接書き込み.

	//コンスタントバッファ作成.
	if (FAILED(m_pDevice11->CreateBuffer(&cb,NULL,&m_pConstantBuffer)))
	{
		MessageBox(NULL, "コンスタントバッファ作成失敗", "エラー", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//モデル作成.
HRESULT BaseSprite::CreateModel()
{
	//板ポリ(四角形)の頂点を作成.
	CreateVertex();

	//バッファ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;						//使用方(デフォルト)
	bd.ByteWidth = sizeof(Sprite2DVertex) * m_uVerMax;	//頂点サイズ(頂点×4)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			//頂点バッファとして扱う.
	bd.CPUAccessFlags = 0;		//CPUからはアクセスしない.
	bd.MiscFlags = 0;			//その他のフラグ(未使用)
	bd.StructureByteStride = 0;	//構造体のサイズ(未使用)

	//サブリソースデータ構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = m_Vertices;//板ポリの頂点をセット.

	//頂点バッファの作成.
	if (FAILED(m_pDevice11->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		MessageBox(NULL, "頂点バッファ作成失敗", "エラー", MB_OK);
		return E_FAIL;
	}

	//頂点バッファをセット.
	UINT stride = sizeof(Sprite2DVertex);//データ間隔.
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//テクスチャ用のサンプラー構造体.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(SamDesc));
	//リニアフィルタ-(線形補間)
	//	POINT:高速だが粗い.
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//ラッピングモード.
	//	WRAP  :繰り返し
	//	MIRROR:反転繰り返し.
	//	CLAMP :端の模様を引き伸ばす.
	//	BORDER:別途境界色を決める.
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//サンプラー作成.
	if (FAILED(m_pDevice11->CreateSamplerState(&SamDesc, &m_pSampleLinear)))//(out)サンプラー.
	{
		MessageBox(NULL, "サンプラ作成失敗", "エラー", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//テクスチャ作成.
HRESULT BaseSprite::CreateTexture(const LPSTR FileName, ID3D11ShaderResourceView** const pTex)
{
	//テクスチャ作成.
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(m_pDevice11,FileName,NULL, NULL,pTex,NULL)))
	{
		ERR_MSG("BaseSprite::CreateTexture()", "エラー");
		return E_FAIL;
	}

	return S_OK;
}

//透過(アルファブレンド)設定の切り替え.
void BaseSprite::SetBlend(bool flg)
{
	//アルファブレンド用ブレンドステート構造体.
	//pngファイル内にアルファ情報があるので、
	//透過するようにブレンドステートを設定する.
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));//初期化.

	blendDesc.IndependentBlendEnable
		= false;//false:RenderTarget[0]のメンバーのみが使用する.
	//true :RenderTarget[0～7]が使用できる.
	//      (レンダーターゲット毎に独立したブレンド処理)
	blendDesc.AlphaToCoverageEnable
		= false;//true :アルファトゥカバレッジを使用する.
	blendDesc.RenderTarget[0].BlendEnable
		= flg;	//true :アルファブレンドを使用する.
	blendDesc.RenderTarget[0].SrcBlend	//元素材に対する設定.
		= D3D11_BLEND_SRC_ALPHA;		//	アルファブレンドを指定.
	blendDesc.RenderTarget[0].DestBlend	//重ねる素材に対する設定.
		= D3D11_BLEND_INV_SRC_ALPHA;	//	アルファブレンドの反転を指定.

	blendDesc.RenderTarget[0].BlendOp	//ブレンドオプション.
		= D3D11_BLEND_OP_ADD;			//	ADD:加算合成.

	blendDesc.RenderTarget[0].SrcBlendAlpha	//元素材のアルファに対する設定.
		= D3D11_BLEND_ONE;					//	そのまま使用.
	blendDesc.RenderTarget[0].DestBlendAlpha//重ねる素材のアルファに対する設定.
		= D3D11_BLEND_ZERO;					//	何もしない.

	blendDesc.RenderTarget[0].BlendOpAlpha	//アルファのブレンドオプション.
		= D3D11_BLEND_OP_ADD;				//	ADD:加算合成.

	blendDesc.RenderTarget[0].RenderTargetWriteMask	//ピクセル毎の書き込みマスク.
		= D3D11_COLOR_WRITE_ENABLE_ALL;				//	全ての成分(RGBA)へのデータの格納を許可する.

	//ブレンドステート作成.
	if (FAILED(m_pDevice11->CreateBlendState(&blendDesc, &m_pBlendState)))
	{
		MessageBox(NULL, "ブレンドステート作成失敗", "エラー", MB_OK);
	}

	//ブレンドステートの設定.
	UINT mask = 0xffffffff;	//マスク値.
	m_pDeviceContext11->OMSetBlendState(m_pBlendState, NULL, mask);
}
