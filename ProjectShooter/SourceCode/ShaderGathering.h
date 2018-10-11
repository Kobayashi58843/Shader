#pragma once
#pragma warning( disable:4005 )

#include "Global.h"

#include "ShaderEnumeration.h"
#include "ShaderStructure.h"

class ShaderGathering
{
public:
	static ShaderGathering* GetInstance()
	{
		static ShaderGathering s_Instance;
		return &s_Instance;
	}
	~ShaderGathering();

	//指定したシェーダのポインタが入った構造体を返す.
	ShaderPointer GetStaticMeshShader(const enStaticModelShader Index) const
	{
		return m_StaticMeshShader[Index];
	}

	//指定したコンスタントバッファのポインタを返す.
	ID3D11Buffer* GetStaticCBuffer(const enStaticModelCBuffer Index) const
	{
		return m_pStaticCBuffers[Index];
	}

	//シェーダの初期設定.
	void InitShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

private:
	ShaderGathering();
	ShaderGathering(const ShaderGathering& rhs);
	ShaderGathering& operator = (const ShaderGathering& rhs);

	//Dx11.
	ID3D11Device*			m_pDevice11;		//デバイスオブジェクト.
	ID3D11DeviceContext*	m_pDeviceContext11;	//デバイスコンテキスト.

	//スタティックメッシュのシェーダ.
	ShaderPointer m_StaticMeshShader[enStaticModelShader_Max];

	//コンスタントバッファ(スタティックメッシュ).
	ID3D11Buffer* m_pStaticCBuffers[enStaticModelCBuffer_Max];

	//スタティックメッシュ用のシェーダを作成.
	void CreateStaticShader();

	//シェーダ作成(スタティックメッシュ).
	void MakeStaticMeshShader(char* ShaderName, ShaderEntryPoint EntryPoint, ShaderPointer& ShaderPointer);

	//コンスタントバッファ作成.
	void MakeConstantBuffer(ID3D11Buffer* &pConstantBuffer, UINT CBufferSize);
};