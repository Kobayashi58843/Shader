#include "BaseSprite.h"

//�V�F�[�_�t�@�C����(�f�B���N�g�����܂�)
const char SHADER_NAME[] = "Shader\\Sprite2D.hlsl";

//�R���X�g���N�^.
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

//�f�X�g���N�^.
BaseSprite::~BaseSprite()
{
	m_pDeviceContext11 = nullptr;
	m_pDevice11 = nullptr;
}

//������.
HRESULT BaseSprite::Create(ID3D11Device* const pDevice11, ID3D11DeviceContext* const pContext11, const LPSTR FileName)
{
	m_pDevice11 = pDevice11;
	m_pDeviceContext11 = pContext11;

	//���摜�̏��.
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

//�|���S������ʂɃ����_�����O.
void BaseSprite::Render()
{
	if (!m_bDispFlg)
	{
		return;
	}

	//���[���h�s��.
	D3DXMATRIX mWorldMat;

	//������:�P�ʍs��쐬.
	D3DXMatrixIdentity(&mWorldMat);

	//���[���h�ϊ�(�\���ʒu��ݒ肷��).
	ConversionWorld(mWorldMat);

	//�g�p����V�F�[�_�̓o�^.
	m_pDeviceContext11->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext11->PSSetShader(m_pPixelShader, NULL, 0);

	//�V�F�[�_�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	InputConstantBuffer(mWorldMat);

	//���_�o�b�t�@���Z�b�g.
	UINT stride = sizeof(Sprite2DVertex);
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//���_�C���v�b�g���C�A�E�g���Z�b�g.
	m_pDeviceContext11->IASetInputLayout(m_pVertexLayout);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g.
	m_pDeviceContext11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�e�N�X�`�����V�F�[�_�ɓn��.
	m_pDeviceContext11->PSSetSamplers(0, 1, &m_pSampleLinear);	//�T���v��-���Z�b�g.
	m_pDeviceContext11->PSSetShaderResources(0, 1, &m_pTexture);//�e�N�X�`�����V�F�[�_�ɓn��.

	//�A���t�@�u�����h�p�u�����h�X�e�[�g�쐬���ݒ�.
	SetBlend(true);

	//�v���~�e�B�u�������_�����O.
	m_pDeviceContext11->Draw(4, 0);

	//�A���t�@�u�����h�𖳌��ɂ���.
	SetBlend(false);
}

//�V�F�[�_���쐬����.
HRESULT BaseSprite::CreateShader()
{
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;
	UINT uCompileFlag = 0;

#ifdef _DEBUG
	uCompileFlag
		= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSL����o�[�e�b�N�X�V�F�[�_�̃u���u���쐬.
	if (FAILED(
		D3DX11CompileFromFile(
		SHADER_NAME,	//�V�F�[�_�t�@�C����(HLSL�t�@�C��)
		NULL,			//�}�N����`�̔z��ւ̃|�C���^(���g�p)
		NULL,			//�C���N���[�h�t�@�C���������C���^�[�t�F�C�X�ւ̃|�C���^(���g�p)
		"VS_Main",		//�V�F�[�_�G���g���[�|�C���g�֐��̖��O.
		"vs_5_0",		//�V�F�[�_�̃��f�����w�肷�镶����(�v���t�@�C��)
		uCompileFlag,	//�V�F�[�_�R���p�C���t���O.
		0,				//�G�t�F�N�g�R���p�C���t���O(���g�p)
		NULL,			//�X���b�h�|���v�C���^�[�t�F�C�X�ւ̃|�C���^(���g�p)
		&pCompiledShader,//�u���u���i�[���郁�����ւ̃|�C���^.
		&pErrors,		//�G���[�ƌx���ꗗ���i�[���郁�����ւ̃|�C���^.
		NULL)))		//�߂�l�ւ̃|�C���^(���g�p)
	{
		MessageBox(NULL, "hlsl�Ǎ����s", "�G���[", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬�����u���u����u�o�[�e�b�N�X�V�F�[�_�v���쐬.
	if (FAILED(m_pDevice11->CreateVertexShader(
		pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),
		NULL,&m_pVertexShader)))//(out)�o�[�e�b�N�X�V�F�[�_.
	{
		MessageBox(NULL, "vs�쐬���s", "�G���[", MB_OK);
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���`.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",				//�ʒu.
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,//DXGI�̃t�H�[�}�b�g(32bit float�^*3)
			0,
			0,							//�f�[�^�̊J�n�ʒu.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",		//�e�N�X�`���ʒu.
			0,
			DXGI_FORMAT_R32G32_FLOAT,	//DXGI�̃t�H�[�}�b�g(32bit float�^*2)
			0,
			12,							//�e�N�X�`���f�[�^�̊J�n�ʒu.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};
	//���_�C���v�b�g���C�A�E�g�̔z��v�f�����Z�o.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g�̍쐬.
	if (FAILED(m_pDevice11->CreateInputLayout(layout,numElements,
		pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),
		&m_pVertexLayout)))//(out)���_�C���v�b�g���C�A�E�g.
	{
		MessageBox(NULL, "���_�C���v�b�g���C�A�E�g�쐬���s", "�G���[", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSL����s�N�Z���V�F�[�_�̃u���u���쐬.
	if (FAILED(
		D3DX11CompileFromFile(
		SHADER_NAME,	//�V�F�[�_�t�@�C����(HLSL�t�@�C��)
		NULL, NULL,
		"PS_Main",		//�V�F�[�_�G���g���[�|�C���g�֐��̖��O.
		"ps_5_0",		//�V�F�[�_�̃��f�����w�肷�镶����(�v���t�@�C��)
		uCompileFlag,	//�V�F�[�_�R���p�C���t���O.
		0, NULL,
		&pCompiledShader,//�u���u���i�[���郁�����ւ̃|�C���^.
		&pErrors,
		NULL)))
	{
		MessageBox(NULL, "hlsl(ps)�Ǎ����s", "�G���[", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬�����u���u����u�s�N�Z���V�F�[�_�v���쐬.
	if (FAILED(m_pDevice11->CreatePixelShader(
		pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),
		NULL,&m_pPixelShader)))//(out)�s�N�Z���V�F�[�_.
	{
		MessageBox(NULL, "ps�쐬���s", "�G���[", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);//�u���u���.

	//�R���X�^���g(�萔)�o�b�t�@�쐬 �V�F�[�_�ɓ���̐��l�𑗂�o�b�t�@.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�R���X�^���g�o�b�t�@���w��.
	cb.ByteWidth = sizeof(Sprite2DConstantBuffer);//�R���X�^���g�o�b�t�@�̃T�C�Y.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//�������݂ŃA�N�Z�X.
	cb.MiscFlags = 0;//���̑��̃t���O(���g�p)
	cb.StructureByteStride = 0;//�\���̂̃T�C�Y(���g�p)
	cb.Usage = D3D11_USAGE_DYNAMIC;//�g�p���@:���ڏ�������.

	//�R���X�^���g�o�b�t�@�쐬.
	if (FAILED(m_pDevice11->CreateBuffer(&cb,NULL,&m_pConstantBuffer)))
	{
		MessageBox(NULL, "�R���X�^���g�o�b�t�@�쐬���s", "�G���[", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//���f���쐬.
HRESULT BaseSprite::CreateModel()
{
	//�|��(�l�p�`)�̒��_���쐬.
	CreateVertex();

	//�o�b�t�@�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;						//�g�p��(�f�t�H���g)
	bd.ByteWidth = sizeof(Sprite2DVertex) * m_uVerMax;	//���_�T�C�Y(���_�~4)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			//���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags = 0;		//CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags = 0;			//���̑��̃t���O(���g�p)
	bd.StructureByteStride = 0;	//�\���̂̃T�C�Y(���g�p)

	//�T�u���\�[�X�f�[�^�\����.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = m_Vertices;//�|���̒��_���Z�b�g.

	//���_�o�b�t�@�̍쐬.
	if (FAILED(m_pDevice11->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		MessageBox(NULL, "���_�o�b�t�@�쐬���s", "�G���[", MB_OK);
		return E_FAIL;
	}

	//���_�o�b�t�@���Z�b�g.
	UINT stride = sizeof(Sprite2DVertex);//�f�[�^�Ԋu.
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//�e�N�X�`���p�̃T���v���[�\����.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(SamDesc));
	//���j�A�t�B���^-(���`���)
	//	POINT:���������e��.
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//���b�s���O���[�h.
	//	WRAP  :�J��Ԃ�
	//	MIRROR:���]�J��Ԃ�.
	//	CLAMP :�[�̖͗l�������L�΂�.
	//	BORDER:�ʓr���E�F�����߂�.
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//�T���v���[�쐬.
	if (FAILED(m_pDevice11->CreateSamplerState(&SamDesc, &m_pSampleLinear)))//(out)�T���v���[.
	{
		MessageBox(NULL, "�T���v���쐬���s", "�G���[", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//�e�N�X�`���쐬.
HRESULT BaseSprite::CreateTexture(const LPSTR FileName, ID3D11ShaderResourceView** const pTex)
{
	//�e�N�X�`���쐬.
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(m_pDevice11,FileName,NULL, NULL,pTex,NULL)))
	{
		ERR_MSG("BaseSprite::CreateTexture()", "�G���[");
		return E_FAIL;
	}

	return S_OK;
}

//����(�A���t�@�u�����h)�ݒ�̐؂�ւ�.
void BaseSprite::SetBlend(bool flg)
{
	//�A���t�@�u�����h�p�u�����h�X�e�[�g�\����.
	//png�t�@�C�����ɃA���t�@��񂪂���̂ŁA
	//���߂���悤�Ƀu�����h�X�e�[�g��ݒ肷��.
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));//������.

	blendDesc.IndependentBlendEnable
		= false;//false:RenderTarget[0]�̃����o�[�݂̂��g�p����.
	//true :RenderTarget[0�`7]���g�p�ł���.
	//      (�����_�[�^�[�Q�b�g���ɓƗ������u�����h����)
	blendDesc.AlphaToCoverageEnable
		= false;//true :�A���t�@�g�D�J�o���b�W���g�p����.
	blendDesc.RenderTarget[0].BlendEnable
		= flg;	//true :�A���t�@�u�����h���g�p����.
	blendDesc.RenderTarget[0].SrcBlend	//���f�ނɑ΂���ݒ�.
		= D3D11_BLEND_SRC_ALPHA;		//	�A���t�@�u�����h���w��.
	blendDesc.RenderTarget[0].DestBlend	//�d�˂�f�ނɑ΂���ݒ�.
		= D3D11_BLEND_INV_SRC_ALPHA;	//	�A���t�@�u�����h�̔��]���w��.

	blendDesc.RenderTarget[0].BlendOp	//�u�����h�I�v�V����.
		= D3D11_BLEND_OP_ADD;			//	ADD:���Z����.

	blendDesc.RenderTarget[0].SrcBlendAlpha	//���f�ނ̃A���t�@�ɑ΂���ݒ�.
		= D3D11_BLEND_ONE;					//	���̂܂܎g�p.
	blendDesc.RenderTarget[0].DestBlendAlpha//�d�˂�f�ނ̃A���t�@�ɑ΂���ݒ�.
		= D3D11_BLEND_ZERO;					//	�������Ȃ�.

	blendDesc.RenderTarget[0].BlendOpAlpha	//�A���t�@�̃u�����h�I�v�V����.
		= D3D11_BLEND_OP_ADD;				//	ADD:���Z����.

	blendDesc.RenderTarget[0].RenderTargetWriteMask	//�s�N�Z�����̏������݃}�X�N.
		= D3D11_COLOR_WRITE_ENABLE_ALL;				//	�S�Ă̐���(RGBA)�ւ̃f�[�^�̊i�[��������.

	//�u�����h�X�e�[�g�쐬.
	if (FAILED(m_pDevice11->CreateBlendState(&blendDesc, &m_pBlendState)))
	{
		MessageBox(NULL, "�u�����h�X�e�[�g�쐬���s", "�G���[", MB_OK);
	}

	//�u�����h�X�e�[�g�̐ݒ�.
	UINT mask = 0xffffffff;	//�}�X�N�l.
	m_pDeviceContext11->OMSetBlendState(m_pBlendState, NULL, mask);
}
