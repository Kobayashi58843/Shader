#include "DX9Mesh.h"

#include "Singleton.h"

//================================================
//	コンストラクタ.
//================================================
clsDX9Mesh::clsDX9Mesh()
{
	ZeroMemory(this, sizeof(clsDX9Mesh));
	m_fScale = 1.0f;
}

//================================================
//	デストラクタ.
//================================================
clsDX9Mesh::~clsDX9Mesh()
{
	SAFE_DELETE_ARRAY(m_ppIndexBuffer);
	SAFE_DELETE_ARRAY(m_pMaterials);

	m_pCBuffPerMesh = nullptr;
	m_pCBuffPerMaterial = nullptr;
	m_pCBuffPerFrame = nullptr;

	//SAFE_DELETE(m_pMesh);

	//オブジェクトのリリース.
	SAFE_RELEASE(m_pDevice9);
	SAFE_RELEASE(m_pD3d);

	m_pDeviceContext11 = nullptr;
	m_pDevice11 = nullptr;

	m_hWnd = nullptr;
}

//================================================
//	初期化.
//================================================
HRESULT clsDX9Mesh::Init(HWND hWnd, ID3D11Device* pDevice11,ID3D11DeviceContext* pContext11, LPSTR fileName)
{
	m_hWnd = hWnd;
	m_pDevice11 = pDevice11;
	m_pDeviceContext11 = pContext11;

	if (FAILED(InitDx9(m_hWnd)))
	{
		return E_FAIL;
	}
	if (FAILED(LoadXMesh(fileName)))
	{
		return E_FAIL;
	}
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//================================================
//	Dx9初期化.
//================================================
HRESULT clsDX9Mesh::InitDx9(HWND hWnd)
{
	m_hWnd = hWnd;

	//「Direct3D」オブジェクトの作成.
	m_pD3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3d == NULL)
	{
		MessageBox(NULL, "Dx9オブジェクト作成失敗", "InitDx9", MB_OK);
		return E_FAIL;
	}

	//Direct3Dデバイス オブジェクトの作成.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	//バックバッファのフォーマット(デフォルト)
	d3dpp.BackBufferCount = 1;					//バックバッファの数.
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//スワップエフェクト(デフォルト)
	d3dpp.Windowed = true;						//ウィンドウモード.
	d3dpp.EnableAutoDepthStencil = true;		//ステンシル有効.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	//ステンシルのフォーマット(16ビット)

	//デバイス作成(HALモード:描画と頂点処理をGPUで行う)
	if (FAILED(m_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice9)))
	{
		//デバイス作成(HALモード:描画はGPU、頂点処理はCPUで行う)
		if (FAILED(m_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp, &m_pDevice9)))
		{
			MessageBox(NULL,"HALモードでデバイスを作成できません\nREFモードで再試行します","警告", MB_OK);

			//デバイス作成(REFモード:描画はCPU、頂点処理はGPUで行う)
			if (FAILED(m_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,D3DCREATE_HARDWARE_VERTEXPROCESSING,&d3dpp, &m_pDevice9)))
			{
				//デバイス作成(REFモード:描画と頂点処理をCPUで行う)
				if (FAILED(m_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice9)))
				{
					MessageBox(NULL,"Direct3Dデバイス作成失敗", NULL, MB_OK);

					return E_FAIL;
				}
			}
		}
	}


	return S_OK;
}

//================================================
//Xファイルからメッシュをロードする.
//================================================
HRESULT clsDX9Mesh::LoadXMesh(LPSTR fileName)
{
	//マテリアルバッファ.
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	//レイとメッシュの判定のために追加.
	if (FAILED(D3DXLoadMeshFromXA(
		fileName,			//ファイル名.
		D3DXMESH_SYSTEMMEM,	//システムメモリに読込.
		m_pDevice9, NULL,
		&pD3DXMtrlBuffer,	//(out)マテリアル情報.
		NULL,
		&m_dwNumMaterials,	//(out)マテリアル数.
		&m_pMeshForRay)))	//(out)メッシュオブジェクト.
	{
		MessageBox(NULL, fileName, "Xファイル読込失敗", MB_OK);
		return E_FAIL;
	}

	//Xファイルのロード.
	if (FAILED(D3DXLoadMeshFromXA(
		fileName,			//ファイル名.
		D3DXMESH_SYSTEMMEM	//システムメモリに読込.
		| D3DXMESH_32BIT,
		m_pDevice9, NULL,
		&pD3DXMtrlBuffer,	//(out)マテリアル情報.
		NULL,
		&m_dwNumMaterials,	//(out)マテリアル数.
		&m_pMesh)))			//(out)メッシュオブジェクト.
	{
		MessageBox(NULL, fileName, "Xファイル読込失敗", MB_OK);
		return E_FAIL;
	}

	D3D11_BUFFER_DESC		bd;	//Dx11バッファ構造体.
	D3D11_SUBRESOURCE_DATA	InitData;//初期化データ.

	//読込んだ情報から必要な情報を抜き出す.
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	//マテリアル数分の領域を獲得.
	m_pMaterials = new MY_MATERIAL[m_dwNumMaterials];
	m_ppIndexBuffer = new ID3D11Buffer*[m_dwNumMaterials];
	//マテリアル数分繰り返し.
	for (DWORD i = 0; i<m_dwNumMaterials; i++)
	{
		//マテリアル情報のコピー.
		//アンビエント.
		m_pMaterials[i].Ambient.x = d3dxMaterials[i].MatD3D.Ambient.r;
		m_pMaterials[i].Ambient.y = d3dxMaterials[i].MatD3D.Ambient.g;
		m_pMaterials[i].Ambient.z = d3dxMaterials[i].MatD3D.Ambient.b;
		m_pMaterials[i].Ambient.w = d3dxMaterials[i].MatD3D.Ambient.a;
		//ディフューズ.
		m_pMaterials[i].Diffuse.x = d3dxMaterials[i].MatD3D.Diffuse.r;
		m_pMaterials[i].Diffuse.y = d3dxMaterials[i].MatD3D.Diffuse.g;
		m_pMaterials[i].Diffuse.z = d3dxMaterials[i].MatD3D.Diffuse.b;
		m_pMaterials[i].Diffuse.w = d3dxMaterials[i].MatD3D.Diffuse.a;
		//スペキュラ.
		m_pMaterials[i].Specular.x = d3dxMaterials[i].MatD3D.Specular.r;
		m_pMaterials[i].Specular.y = d3dxMaterials[i].MatD3D.Specular.g;
		m_pMaterials[i].Specular.z = d3dxMaterials[i].MatD3D.Specular.b;
		m_pMaterials[i].Specular.w = d3dxMaterials[i].MatD3D.Specular.a;

		//(その面に)テクスチャが貼られているか？.
		if (d3dxMaterials[i].pTextureFilename != NULL && lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			m_bTexture = true;//テクスチャフラグを立てる.

			char path[64] = "";
			int  path_count = lstrlen(fileName);

			for (int k = path_count; k >= 0; k--)
			{
				if (fileName[k] == '\\'){
					for (int j = 0; j <= k; j++)
					{
						path[j] = fileName[j];
					}
					path[k + 1] = '\0';
					break;
				}
			}
			//パスとテクスチャファイル名を連結.
			strcat_s(path, sizeof(path), d3dxMaterials[i].pTextureFilename);

			//テクスチャファイル名をコピー.
			strcpy_s(m_pMaterials[i].szTextureName, sizeof(m_pMaterials[i].szTextureName), path);

			//テクスチャ作成.
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(
				m_pDevice11,
				m_pMaterials[i].szTextureName,//テクスチャファイル名.
				NULL, NULL,
				&m_pMaterials[i].pTexture,//(out)テクスチャオブジェクト.
				NULL)))
			{
				MessageBox(NULL, m_pMaterials[i].szTextureName,"テクスチャ作成失敗", MB_OK);
				return E_FAIL;
			}
		}
	}

	//--------------------------------------------
	//	インデックスバッファを作成.
	//--------------------------------------------
	//メッシュの属性情報を得る.
	//属性情報でインデックスバッファから細かいマテリアルごとのインデックスバッファを分離できる.
	D3DXATTRIBUTERANGE* pAttrTable = NULL;

	//メッシュの面および頂点の順番変更を制御し、パフォーマンスを最適化する.
	// D3DXMESHOPT_COMPACT :面の順番を変更し、
	//						使用されていない頂点と面を削除する.
	// D3DXMESHOPT_ATTRSORT:パフォーマンスを上げる為、
	//						面の順番を変更して最適化を行う.
	m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, 0, 0, 0, 0);
	//属性テーブルの取得.
	m_pMesh->GetAttributeTable(NULL, &m_NumAttr);
	pAttrTable = new D3DXATTRIBUTERANGE[m_NumAttr];
	if (FAILED(m_pMesh->GetAttributeTable(pAttrTable, &m_NumAttr)))
	{
		MessageBox(NULL, "属性テーブル取得失敗",
			"LoadXMesh", MB_OK);
		return E_FAIL;
	}

	//同じく、ロックしないと取り出せない.
	int* pIndex = NULL;
	m_pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndex);

	//属性ごとのインデックスバッファを作成.
	for (DWORD i = 0; i<m_NumAttr; i++)
	{
		m_AttrID[i] = pAttrTable[i].AttribId;

		//Dx9のインデックスバッファからの情報でDx11のインデックスバッファを作成.
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * pAttrTable[i].FaceCount * 3; //面数×3で頂点数.
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		//大きいインデックスバッファ内のオフセット(×3する)
		InitData.pSysMem = &pIndex[pAttrTable[i].FaceStart * 3];

		if (FAILED(m_pDevice11->CreateBuffer(&bd, &InitData, &m_ppIndexBuffer[i])))
		{
			MessageBox(NULL, "インデックスバッファ作成失敗","LoadXMesh()", MB_OK);
			return E_FAIL;
		}
		//面の数をコピー.
		m_pMaterials[m_AttrID[i]].dwNumFace
			= pAttrTable[i].FaceCount;
	}
	delete[] pAttrTable;//属性テーブルの削除.

	m_pMesh->UnlockIndexBuffer();

	//不要になったマテリアルバッファを解放.
	SAFE_RELEASE(pD3DXMtrlBuffer);

	//-----------------------------------------------
	//	頂点(バーテックス)バッファの作成.
	//-----------------------------------------------
	//Dx9の場合、mapではなくロックでバーテックスバッファからデータを取り出す.
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_pMesh->GetVertexBuffer(&pVB);
	DWORD dwStride = m_pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	MeshVertex* pVertex = NULL;
	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertex = (MeshVertex*)pVertices;
		//Dx9のバーテックスバッファから情報で、Dx11のバーテックスバッファを作成.
		bd.Usage = D3D11_USAGE_DEFAULT;
		//頂点を格納するのに必要なバイト数.
		bd.ByteWidth = m_pMesh->GetNumBytesPerVertex() * m_pMesh->GetNumVertices(); //頂点数.
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = pVertex;

		//テクスチャ座標がマイナス対策.
		if (m_bTexture)
		{
			//※テクスチャがあれば処理する.
			//頂点数分繰り返し.
			for (unsigned int i = 0; i<m_pMesh->GetNumVertices(); i++)
			{
				if (pVertex[i].vTex.x < 0.0f)
				{
					pVertex[i].vTex.x += 1.0f;
				}
				if (pVertex[i].vTex.y < 0.0f)
				{
					pVertex[i].vTex.y += 1.0f;
				}
			}
		}

		if (FAILED(m_pDevice11->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
		{
			MessageBox(NULL, "頂点(バーテックス)バッファ作成失敗","LoadXMesh()", MB_OK);
			return E_FAIL;
		}
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);//バーテックスバッファ解放.


	//-----------------------------------------------
	//	サンプラ-作成.
	//-----------------------------------------------
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

HRESULT clsDX9Mesh::InitShader()
{
	m_Shader = Singleton<ShaderGathering>().GetInstance().GetStaticMeshShader(enStaticModelShader_Simple);

	m_pCBuffPerMesh = Singleton<ShaderGathering>().GetInstance().GetStaticCBuffer(enStaticModelCBuffer_Mesh);
	m_pCBuffPerMaterial = Singleton<ShaderGathering>().GetInstance().GetStaticCBuffer(enStaticModelCBuffer_Material);
	m_pCBuffPerFrame = Singleton<ShaderGathering>().GetInstance().GetStaticCBuffer(enStaticModelCBuffer_Frame);

	return S_OK;
}

//レンダリング用.
void clsDX9Mesh::Render(const D3DXMATRIX &mView, const D3DXMATRIX &mProj, const LIGHT &Light, const D3DXVECTOR3 &vEye)
{
	D3DXMATRIX	mWorld;	//ワールド行列.
	D3DXMATRIX	mScale, mYaw, mPitch, mRoll, mTrans;

	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixIdentity(&mScale);
	D3DXMatrixIdentity(&mYaw);
	D3DXMatrixIdentity(&mPitch);
	D3DXMatrixIdentity(&mRoll);
	D3DXMatrixIdentity(&mTrans);
	
	//ワールド変換(表示位置を設定する)
	//拡縮.
	D3DXMatrixScaling(&mScale, m_fScale, m_fScale, m_fScale);
	//回転.
	D3DXMatrixRotationY(&mYaw, m_fYaw);		//Y軸回転.
	D3DXMatrixRotationX(&mPitch, m_fPitch);	//X軸回転.
	D3DXMatrixRotationZ(&mRoll, m_fRoll);	//Z軸回転.

	//移動.
	D3DXMatrixTranslation(&mTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	//合成(拡縮×回転×移動)
	mWorld = mScale * mYaw * mPitch * mRoll * mTrans;

	//使用するシェーダをセット.
	m_pDeviceContext11->VSSetShader(m_Shader.pVertexShader, NULL, 0);	//頂点シェーダ.
	m_pDeviceContext11->PSSetShader(m_Shader.pPixelShader, NULL, 0);	//ピクセルシェーダ.

	//シェーダのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	//バッファ内のデータの書き換え開始時にMap.
	if (SUCCEEDED(m_pDeviceContext11->Map(m_pCBuffPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		CBUFF_PER_FRAME cb;//コンスタントバッファ.

		//カメラ位置.
		cb.vEye = D3DXVECTOR4(vEye.x, vEye.y, vEye.z, 0.0f);

		/*/========/ ライト情報 /========/*/
		//方向.
		cb.vLightDir = D3DXVECTOR4(Light.vDir.x, Light.vDir.y, Light.vDir.z, 0.0f);
		//強度.
		cb.fIntensity = Light.fIntensity;

		//コピー先のバッファ,コピー先のバッファサイズ,コピー元のバッファ,コピーするサイズ.
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

		//バッファ内のデータの書き換え終了時にUnmap.
		m_pDeviceContext11->Unmap(m_pCBuffPerFrame, 0);
	}

	//コンスタントバッファをどのシェーダで使うか？.
	m_pDeviceContext11->VSSetConstantBuffers(2, 1, &m_pCBuffPerFrame);//頂点シェーダ.
	m_pDeviceContext11->PSSetConstantBuffers(2, 1, &m_pCBuffPerFrame);//ピクセルシェーダ.

	//メッシュのレンダリング.
	RenderMesh(mWorld, mView, mProj);
}

//メッシュ描画.
void clsDX9Mesh::RenderMesh(const D3DXMATRIX &mWorld, const D3DXMATRIX &mView, const D3DXMATRIX &mProj)
{
	//シェーダのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	//バッファ内のデータの書き換え開始時にMap.
	if (SUCCEEDED(m_pDeviceContext11->Map(m_pCBuffPerMesh, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		CBUFF_PER_MESH cb;//コンスタントバッファ.

		//ワールド行列を渡す.
		cb.mW = mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);

		//ワールド,ビュー(カメラ),プロジェクション(射影)行列を渡す.
		D3DXMATRIX m = mWorld * mView * mProj;
		D3DXMatrixTranspose(&m, &m);//行列を転置する.
		//※行列の計算方法がDirectXとGPUで異なるため.

		cb.mWVP = m;

		//コピー先のバッファ,コピー先のバッファサイズ,コピー元のバッファ,コピーするサイズ.
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));

		//バッファ内のデータの書き換え終了時にUnmap.
		m_pDeviceContext11->Unmap(m_pCBuffPerMesh, 0);
	}

	//コンスタントバッファをどのシェーダで使うか？.
	m_pDeviceContext11->VSSetConstantBuffers(0, 1, &m_pCBuffPerMesh);//頂点シェーダ.
	m_pDeviceContext11->PSSetConstantBuffers(0, 1, &m_pCBuffPerMesh);//ピクセルシェーダ.

	//頂点インプットレイアウトをセット.
	m_pDeviceContext11->IASetInputLayout(m_Shader.pVertexLayout);

	//プリミティブ・トポロジーをセット.
	m_pDeviceContext11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//頂点バッファをセット.
	UINT stride = m_pMesh->GetNumBytesPerVertex();
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//アルファブレンド用ブレンドステート作成.
	SetBlend(false);

	//属性の数だけ、それぞれの属性のインデックスバッファを描画.
	for (DWORD i = 0; i<m_NumAttr; i++)
	{
		//使用されていないマテリアル対策.
		if (m_pMaterials[m_AttrID[i]].dwNumFace == 0)
		{
			continue;
		}
		//インデックスバッファをセット.
		m_pDeviceContext11->IASetIndexBuffer(m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
		//マテリアルの各要素をシェーダに渡す.
		D3D11_MAPPED_SUBRESOURCE pData1;
		if (SUCCEEDED(m_pDeviceContext11->Map(m_pCBuffPerMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData1)))
		{
			CBUFF_PER_MATERIAL cb;
			//アンビエント,ディフューズ,スペキュラをシェーダに渡す.
			cb.vAmbient = m_pMaterials[m_AttrID[i]].Ambient;
			//cb.vDiffuse = m_pMaterials[m_AttrID[i]].Diffuse;
			cb.vDiffuse.x = 1.0f;
			cb.vDiffuse.y = 1.0f;
			cb.vDiffuse.z = 1.0f;
			cb.vSpecular = m_pMaterials[m_AttrID[i]].Specular;

			//コピー先のバッファ,コピー先のバッファサイズ,コピー元のバッファ,コピーするサイズ.
			memcpy_s(pData1.pData, pData1.RowPitch, (void*)&cb, sizeof(CBUFF_PER_MATERIAL));

			m_pDeviceContext11->Unmap(m_pCBuffPerMaterial, 0);
		}

		//このコンスタントバッファをどのシェーダで使うか？.
		m_pDeviceContext11->VSSetConstantBuffers(1, 1, &m_pCBuffPerMaterial);
		m_pDeviceContext11->PSSetConstantBuffers(1, 1, &m_pCBuffPerMaterial);

		//テクスチャをシェーダに渡す.
		if (m_pMaterials[m_AttrID[i]].pTexture)
		{
			//テクスチャがあるとき.
			m_pDeviceContext11->PSSetSamplers(0, 1, &m_pSampleLinear);
			m_pDeviceContext11->PSSetShaderResources(0, 1, &m_pMaterials[m_AttrID[i]].pTexture);
		}
		else
		{
			//テクスチャがないとき.
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			m_pDeviceContext11->PSSetShaderResources(0, 1, Nothing);
		}
		//プリミティブ(ポリゴン)をレンダリング.
		m_pDeviceContext11->DrawIndexed(m_pMaterials[m_AttrID[i]].dwNumFace * 3, 0, 0);
	}
}

//透過(アルファブレンド)設定の切り替え.
void clsDX9Mesh::SetBlend(bool flg)
{
	//アルファブレンド用ブレンドステート構造体.
	//pngファイル内にアルファ情報があるので、
	//透過するようにブレンドステートを設定する.
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));//初期化.

	blendDesc.IndependentBlendEnable
		= false;//false:RenderTarget[0]のメンバーのみが使用する.
	//true :RenderTarget[0〜7]が使用できる.
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
