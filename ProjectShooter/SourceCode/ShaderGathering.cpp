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

//�V�F�[�_�̏����ݒ�.
void ShaderGathering::InitShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	m_pDevice11 = pDevice;
	m_pDeviceContext11 = pDeviceContext;

	//�X�^�e�B�b�N���b�V���p�V�F�[�_�쐬.
	CreateStaticShader();
}

//�X�^�e�B�b�N���b�V���p�V�F�[�_�쐬.
void ShaderGathering::CreateStaticShader()
{
	char* ShaderName = nullptr;
	ShaderEntryPoint EntryPoint;

	//�V�F�[�_�쐬.
	for (int i = 0; i < enStaticModelShader_Max; i++)
	{
		switch (i)
		{
		case enStaticModelShader_NoTex:
			//�e�N�X�`�������̃V�F�[�_.
			ShaderName = "Shader\\MeshStatic.hlsl";
			EntryPoint.pVertexShaderEntryPointName = "VS_NoTex";
			EntryPoint.pPixelShaderEntryPointName = "PS_NoTex";
			
			break;
		case enStaticModelShader_Simple:
			//�G�t�F�N�g�������Ȃ��V�F�[�_.
			ShaderName = "Shader\\MeshStatic.hlsl";
			EntryPoint.pVertexShaderEntryPointName = "VS_Main";
			EntryPoint.pPixelShaderEntryPointName = "PS_Main";

			break;
		default:
			break;
		}

		MakeStaticMeshShader(ShaderName, EntryPoint, m_StaticMeshShader[i]);
	}

	//�R���X�^���g�o�b�t�@�쐬.
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

//�V�F�[�_�쐬(�X�^�e�B�b�N���b�V��).
void ShaderGathering::MakeStaticMeshShader(char* ShaderName, ShaderEntryPoint EntryPoint, ShaderPointer& ShaderPointer)
{
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;
	UINT uCompileFlag = 0;

#ifdef _DEBUG
	uCompileFlag
		= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSL����o�[�e�b�N�X�V�F�[�_�̃u���u���쐬.
	if (FAILED(D3DX11CompileFromFile(ShaderName, NULL, NULL, EntryPoint.pVertexShaderEntryPointName,
		"vs_5_0", uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(NULL, ShaderName, "hlsl�Ǎ����s", MB_OK);
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬�����u���u����u�o�[�e�b�N�X�V�F�[�_�v���쐬.
	if (FAILED(m_pDevice11->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL,
		&ShaderPointer.pVertexShader)))//(out)�o�[�e�b�N�X�V�F�[�_.
	{
		MessageBox(NULL, ShaderName, "vs�쐬���s", MB_OK);
	}

	//���_�C���v�b�g���C�A�E�g���`.
	D3D11_INPUT_ELEMENT_DESC layout[3];
	//���_�C���v�b�g���C�A�E�g�̔z��v�f��.
	UINT numElements = 0;

	D3D11_INPUT_ELEMENT_DESC tmp[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	numElements = sizeof(tmp) / sizeof(tmp[0]);//�v�f���Z�o.
	memcpy_s(layout, sizeof(layout), tmp, sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements);

	//���_�C���v�b�g���C�A�E�g�̍쐬.
	if (FAILED(m_pDevice11->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(),
		&ShaderPointer.pVertexLayout)))//(out)���_�C���v�b�g���C�A�E�g.
	{
		MessageBox(NULL, "���_�C���v�b�g���C�A�E�g�쐬���s", "�G���[", MB_OK);
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSL����s�N�Z���V�F�[�_�̃u���u���쐬.
	if (FAILED(D3DX11CompileFromFile(ShaderName, NULL, NULL, EntryPoint.pPixelShaderEntryPointName,
		"ps_5_0", uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(NULL, ShaderName, "hlsl�Ǎ����s", MB_OK);
	}

	SAFE_RELEASE(pErrors);

	//��L�ō쐬�����u���u����u�s�N�Z���V�F�[�_�v���쐬.
	if (FAILED(m_pDevice11->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL,
		&ShaderPointer.pPixelShader)))//(out)�s�N�Z���V�F�[�_.
	{
		MessageBox(NULL, ShaderName, "ps�쐬���s", MB_OK);
	}
	SAFE_RELEASE(pCompiledShader);//�u���u���.
}

//�R���X�^���g�o�b�t�@�쐬.
void ShaderGathering::MakeConstantBuffer(ID3D11Buffer* &pConstantBuffer, UINT CBufferSize)
{
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//�R���X�^���g�o�b�t�@���w��.
	cb.ByteWidth = CBufferSize;					//�R���X�^���g�o�b�t�@�̃T�C�Y.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//�������݂ŃA�N�Z�X.
	cb.MiscFlags = 0;							//���̑��̃t���O(���g�p)
	cb.StructureByteStride = 0;					//�\���̂̃T�C�Y(���g�p)
	cb.Usage = D3D11_USAGE_DYNAMIC;				//�g�p���@:���ڏ�������.

	//�R���X�^���g�o�b�t�@�쐬.
	if (FAILED(m_pDevice11->CreateBuffer(&cb, NULL, &pConstantBuffer)))
	{
		MessageBox(NULL, "�R���X�^���g�o�b�t�@�쐬���s", "�G���[", MB_OK);
	}
}