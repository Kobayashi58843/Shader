#include "ShaderGathering.h"

ShaderGathering::ShaderGathering()
{
	m_pDevice11 = nullptr;
	m_pDeviceContext11 = nullptr;

	for (unsigned int i = 0; i < enStaticModelShader_Max; i++)
	{
		m_StaticMeshShader[i].pVertexShader = nullptr;
		m_StaticMeshShader[i].pPixelShader = nullptr;
		m_StaticMeshShader[i].pVertexLayout = nullptr;
	}

	for (unsigned int i = 0; i < enStaticModelCBuffer_Max; i++)
	{
		m_pStaticCBuffers[i] = nullptr;
	}
}

ShaderGathering::~ShaderGathering()
{
}

//シェーダの初期設定.
void ShaderGathering::InitShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	m_pDevice11 = pDevice;
	m_pDeviceContext11 = pDeviceContext;

	//スタティックメッシュ用シェーダ作成.
	CreateStaticShader();
}

//スタティックメッシュ用シェーダ作成.
void ShaderGathering::CreateStaticShader()
{
	char* ShaderName = nullptr;
	ShaderEntryPoint EntryPoint;

	//シェーダ作成.
	for (int i = 0; i < enStaticModelShader_Max; i++)
	{
		switch (i)
		{
		case enStaticModelShader_NoTex:
			//テクスチャ無しのシェーダ.
			ShaderName = "Shader\\MeshStatic.hlsl";
			EntryPoint.pVertexShaderEntryPointName = "VS_NoTex";
			EntryPoint.pPixelShaderEntryPointName = "PS_NoTex";
			
			break;
		case enStaticModelShader_Simple:
			//エフェクトをかけないシェーダ.
			ShaderName = "Shader\\MeshStatic.hlsl";
			EntryPoint.pVertexShaderEntryPointName = "VS_Main";
			EntryPoint.pPixelShaderEntryPointName = "PS_Main";

			break;
		default:
			break;
		}

		MakeStaticMeshShader(ShaderName, EntryPoint, m_StaticMeshShader[i]);
	}

	//コンスタントバッファ作成.
	for (int i = 0; i < enStaticModelCBuffer_Max; i++)
	{
		UINT uiBufferSize;

		switch (i)
		{
		case enStaticModelCBuffer_Mesh:
			uiBufferSize = sizeof(CBUFF_PER_MESH);

			break;
		case enStaticModelCBuffer_Material:
			uiBufferSize = sizeof(CBUFF_PER_MATERIAL);

			break;
		case enStaticModelCBuffer_Frame:
			uiBufferSize = sizeof(CBUFF_PER_FRAME);

			break;
		default:
			break;
		}
		
		MakeConstantBuffer(m_pStaticCBuffers[i], uiBufferSize);
	}
}

//シェーダ作成(スタティックメッシュ).
void ShaderGathering::MakeStaticMeshShader(char* ShaderName, ShaderEntryPoint EntryPoint, ShaderPointer& ShaderPointer)
{
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;
	UINT uCompileFlag = 0;

#ifdef _DEBUG
	uCompileFlag
		= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSLからバーテックスシェーダのブロブを作成.
	if (FAILED(D3DX11CompileFromFile(ShaderName, NULL, NULL, EntryPoint.pVertexShaderEntryPointName,
		"vs_5_0", uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(NULL, ShaderName, "hlsl読込失敗", MB_OK);
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したブロブから「バーテックスシェーダ」を作成.
	if (FAILED(m_pDevice11->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL,
		&ShaderPointer.pVertexShader)))//(out)バーテックスシェーダ.
	{
		MessageBox(NULL, ShaderName, "vs作成失敗", MB_OK);
	}

	//頂点インプットレイアウトを定義.
	D3D11_INPUT_ELEMENT_DESC layout[3];
	//頂点インプットレイアウトの配列要素数.
	UINT numElements = 0;

	D3D11_INPUT_ELEMENT_DESC tmp[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	numElements = sizeof(tmp) / sizeof(tmp[0]);//要素数算出.
	memcpy_s(layout, sizeof(layout), tmp, sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements);

	//頂点インプットレイアウトの作成.
	if (FAILED(m_pDevice11->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
		&ShaderPointer.pVertexLayout)))//(out)頂点インプットレイアウト.
	{
		MessageBox(NULL, "頂点インプットレイアウト作成失敗", "エラー", MB_OK);
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSLからピクセルシェーダのブロブを作成.
	if (FAILED(D3DX11CompileFromFile(ShaderName, NULL, NULL, EntryPoint.pPixelShaderEntryPointName,
		"ps_5_0", uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(NULL, ShaderName, "hlsl読込失敗", MB_OK);
	}

	SAFE_RELEASE(pErrors);

	//上記で作成したブロブから「ピクセルシェーダ」を作成.
	if (FAILED(m_pDevice11->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL,
		&ShaderPointer.pPixelShader)))//(out)ピクセルシェーダ.
	{
		MessageBox(NULL, ShaderName, "ps作成失敗", MB_OK);
	}
	SAFE_RELEASE(pCompiledShader);//ブロブ解放.
}

//コンスタントバッファ作成.
void ShaderGathering::MakeConstantBuffer(ID3D11Buffer* &pConstantBuffer, UINT CBufferSize)
{
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//コンスタントバッファを指定.
	cb.ByteWidth = CBufferSize;					//コンスタントバッファのサイズ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//書き込みでアクセス.
	cb.MiscFlags = 0;							//その他のフラグ(未使用)
	cb.StructureByteStride = 0;					//構造体のサイズ(未使用)
	cb.Usage = D3D11_USAGE_DYNAMIC;				//使用方法:直接書き込み.

	//コンスタントバッファ作成.
	if (FAILED(m_pDevice11->CreateBuffer(&cb, NULL, &pConstantBuffer)))
	{
		MessageBox(NULL, "コンスタントバッファ作成失敗", "エラー", MB_OK);
	}
}