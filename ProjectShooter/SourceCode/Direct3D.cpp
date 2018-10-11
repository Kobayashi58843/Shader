#include "Direct3D.h"

Direct3D::Direct3D() 
{
	m_pDevice = nullptr;			//�f�o�C�X�I�u�W�F�N�g.
	m_pDeviceContext = nullptr;		//�f�o�C�X�R���e�L�X�g.
	m_pSwapChain = nullptr;			//�X���b�v�`�F�[��.
	m_pBackBuffer_RTV = nullptr;	//�����_�[�^�[�Q�b�g�r���[.
	m_pBackBuffer_Tex = nullptr;	//�o�b�N�o�b�t�@.
	m_pBackBuffer_DSV = nullptr;	//�f�v�X�X�e���V���r���[.
	m_pDepthStencilState = nullptr;	//�[�x(Z)�e�X�g�ݒ�.
}

Direct3D::~Direct3D()
{
	SAFE_RELEASE(m_pBackBuffer_DSV);
	SAFE_RELEASE(m_pBackBuffer_Tex);
	SAFE_RELEASE(m_pBackBuffer_RTV);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}

//Direct3D������.
void Direct3D::Create(HWND hWnd)
{
	/*====/ �f�o�C�X�ƃX���b�v�`�F�[���֌W /====*/

	//�X���b�v�`�F�[���\����.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;	//�o�b�N�o�b�t�@�̐�.
	sd.BufferDesc.Width = (UINT)g_fWindowWidth;//�o�b�N�o�b�t�@�̕�.
	sd.BufferDesc.Height = (UINT)g_fWindowHeight;//�o�b�N�o�b�t�@�̍���.
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//�t�H�[�}�b�g(32�r�b�g�J���[)
	sd.BufferDesc.RefreshRate.Numerator = 60;
	//���t���b�V�����[�g(����) ��g_fFPS:60
	sd.BufferDesc.RefreshRate.Denominator = 1;
	//���t���b�V�����[�g(���q)
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//�g����(�\����)
	sd.OutputWindow = hWnd;	//�E�B���h�E�n���h��.
	sd.SampleDesc.Count = 1;	//�}���`�T���v���̐�.
	sd.SampleDesc.Quality = 0;	//�}���`�T���v���̃N�I���e�B.
	sd.Windowed = TRUE;	//�E�B���h�E���[�h(�t���X�N����FALSE)

	//�쐬�����݂�@�\���x���̗D����w��.
	//	(GPU���T�|�[�g����@�\�Z�b�g�̒�`)
	//	D3D_FEATURE_LEVEL�񋓑̂̔z��.
	//	D3D_FEATURE_LEVEL_10_1:Direct3D 10.1��GPU���x��.
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;//�z��̗v�f��.

	//�f�o�C�X�ƃX���b�v�`�F�[���̍쐬.
	//	�n�[�h�E�F�A(GPU)�f�o�C�X�ō쐬.
	if (FAILED(D3D11CreateDeviceAndSwapChain(
		NULL,			//�r�f�I�A�_�v�^�ւ̃|�C���^.
		D3D_DRIVER_TYPE_HARDWARE,//�쐬����f�o�C�X�̎��.
		NULL,			//�\�t�g�E�F�A ���X�^���C�U�[����������DLL�̃n���h��.
		0,				//�L���ɂ��郉���^�C�����C���[.
		&pFeatureLevels,//�쐬�����݂�@�\���x���̏������w�肷��z��ւ̃|�C���^.
		1,				//���̗v�f��.
		D3D11_SDK_VERSION,//SDK�̃o�[�W����.
		&sd,			//�X���b�v�`�F�[���̏������p�����[�^�̃|�C���^.
		&m_pSwapChain,	//(out)�����_�����O�Ɏg�p����X���b�v�`�F�[��.
		&m_pDevice,		//(out)�쐬���ꂽ�f�o�C�X.
		pFeatureLevel,	//�@�\���x���̔z��ɂ���ŏ��̗v�f��\���|�C���^.
		&m_pDeviceContext)))//(out)�f�o�C�X �R���e�L�X�g.
	{
		//WARP�f�o�C�X�̍쐬.
		//	D3D_FEATURE_LEVEL_9_1 �` D3D_FEATURE_LEVEL_10_1
		if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_WARP,NULL, 0, &pFeatureLevels,
			1,D3D11_SDK_VERSION, &sd, &m_pSwapChain,&m_pDevice, pFeatureLevel,&m_pDeviceContext)))
		{
			//���t�@�����X�f�o�C�X�̍쐬.
			//	DirectX SDK���C���X�g�[������Ă��Ȃ��Ǝg���Ȃ�.
			if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_REFERENCE,NULL, 0, &pFeatureLevels,
				1,D3D11_SDK_VERSION, &sd, &m_pSwapChain,&m_pDevice, pFeatureLevel,&m_pDeviceContext)))
			{
				MessageBox(NULL,"�f�o�C�X�ƃX���b�v�`�F�[���̍쐬�Ɏ��s","error(main.cpp)", MB_OK);
			}
		}
	}

	//�e��e�N�X�`��-�ƁA����ɕt�т���e��r���[(���)���쐬.

	//-----------------------------------------------
	//	�o�b�N�o�b�t�@����:�J���[�o�b�t�@�ݒ�.
	//-----------------------------------------------

	//�o�b�N�o�b�t�@�e�N�X�`��-���擾(���ɂ���̂ō쐬�ł͂Ȃ�)
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),	//__uuidof:���Ɋ֘A�t�����ꂽGUID���擾.
		//         Texture2D�̗B��̕��Ƃ��Ĉ���.
		(LPVOID*)&pBackBuffer_Tex);	//(out)�o�b�N�o�b�t�@�e�N�X�`��-.

	//���̃e�N�X�`���ɑ΂������_�[�^�[�Q�b�g�r���[(RTV)���쐬.
	m_pDevice->CreateRenderTargetView(pBackBuffer_Tex,NULL,&m_pBackBuffer_RTV);
	//�o�b�N�o�b�t�@�e�N�X�`��-�����.
	SAFE_RELEASE(pBackBuffer_Tex);

	//-----------------------------------------------
	//	�o�b�N�o�b�t�@����:�f�v�X(�[�x)�X�e���V���֌W.
	//-----------------------------------------------

	//�f�v�X(�[��or�[�x)�X�e���V���r���[�p�̃e�N�X�`��-���쐬.
	D3D11_TEXTURE2D_DESC descDepth;

	descDepth.Width = (UINT)g_fWindowWidth;//��.
	descDepth.Height = (UINT)g_fWindowHeight;//����.
	descDepth.MipLevels = 1;//�~�b�v�}�b�v���x��:1.
	descDepth.ArraySize = 1;//�z��:1.
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;//32�r�b�g�t�H�[�}�b�g.
	descDepth.SampleDesc.Count = 1;//�}���`�T���v���̐�.
	descDepth.SampleDesc.Quality = 0;//�}���`�T���v���̃N�I���e�B.
	descDepth.Usage = D3D11_USAGE_DEFAULT;//�g�p���@:�f�t�H���g.
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;//�[�x(�X�e���V���Ƃ��Ďg�p)
	descDepth.CPUAccessFlags = 0;//CPU����̓A�N�Z�X���Ȃ�.
	descDepth.MiscFlags = 0;//���̑��̐ݒ�Ȃ�.

	//(out)�f�v�X�X�e���V���p�e�N�X�`��.
	m_pDevice->CreateTexture2D(&descDepth,NULL,&m_pBackBuffer_Tex);

	//���̃e�N�X�`���ɑ΂��f�v�X�X�e���V���r���[(DSV)���쐬.
	m_pDevice->CreateDepthStencilView(m_pBackBuffer_Tex,NULL,&m_pBackBuffer_DSV);//(out)DSV.

	//�����_�[�^�[�Q�b�g�r���[�ƃf�v�X�X�e���V���r���[���p�C�v���C���ɃZ�b�g.
	m_pDeviceContext->OMSetRenderTargets(1,&m_pBackBuffer_RTV,m_pBackBuffer_DSV);

	//�[�x�e�X�g(Z�e�X�g)��L���ɂ���.
	D3D11_DEPTH_STENCIL_DESC	depthStencilDesc;
	ZeroMemory(&depthStencilDesc,sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = true;

	if (SUCCEEDED(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState)))
	{
		m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
	}

	//�r���[�|�[�g�̐ݒ�.
	D3D11_VIEWPORT vp;
	vp.Width = g_fWindowWidth;		//��.
	vp.Height = g_fWindowHeight;	//����.
	vp.MinDepth = 0.0f;				//�ŏ��[�x(��O)
	vp.MaxDepth = 1.0f;				//�ő�[�x(��)
	vp.TopLeftX = 0.0f;				//����ʒux.
	vp.TopLeftY = 0.0f;				//����ʒuy.
	m_pDeviceContext->RSSetViewports(1, &vp);

	//���X�^���C�Y(�ʂ̓h��Ԃ���)�ݒ�.
	//D3D11_CULL_NONE :�J�����O��؂�(���w�ʂ�`�悷��)
	//D3D11_CULL_BACK :�w�ʂ�`�悵�Ȃ�.
	//D3D11_CULL_FRONT:���ʂ�`�悵�Ȃ�.
	SetRasterize(D3D11_CULL_NONE);
}

//���X�^���C�Y�ݒ�.
void Direct3D::SetRasterize(D3D11_CULL_MODE CullMode)
{
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.FillMode = D3D11_FILL_SOLID;//�h��Ԃ�(�\���b�h)
	rdc.CullMode = CullMode;
	rdc.FrontCounterClockwise = FALSE;
	//�|���S���̕\�������肷��t���O. 
	//TRUE :�����Ȃ�O����.�E���Ȃ������.
	//FALSE:���̋t�ɂȂ�.
	rdc.DepthClipEnable = FALSE;	//�����ɂ��ẴN���b�s���O�L��.

	ID3D11RasterizerState* pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pDeviceContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);
}

//�[�x�e�X�g(Z�e�X�g) ON/OFF�ؑ�.
void Direct3D::SetDepth(bool bFlg)
{
	D3D11_DEPTH_STENCIL_DESC	depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = bFlg;

	m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
}

//�o�b�N�o�b�t�@�̃N���A.
void Direct3D::ClearBackBuffer(D3DXVECTOR4 vClearColor)
{
	//�J���[�o�b�N�o�b�t�@.
	m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer_RTV, vClearColor);
	//�f�v�X�X�e���V���o�b�N�o�b�t�@.
	m_pDeviceContext->ClearDepthStencilView(m_pBackBuffer_DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

//�����_�����O���ꂽ�C���[�W��\��.
void Direct3D::RenderSwapChain()
{
	m_pSwapChain->Present(0, 0);
}