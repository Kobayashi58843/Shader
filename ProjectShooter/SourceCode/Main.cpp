#include "Main.h"
#include "Global.h"
#include "MyMacro.h"
#include <stdio.h>

#include "Singleton.h"

//�O���[�o���ϐ�.
Main*	g_pClsMain = nullptr;

//���C���֐�.
INT WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR lpCmdLine,INT nCmdShow )
{
	//���������[�N�̌��m.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	g_pClsMain = new Main;//������&���C���N���X�̐錾.

	//�N���X�����݂��Ă邩�`�F�b�N.
	assert(g_pClsMain != nullptr);

#if _DEBUG

	if (SUCCEEDED(g_pClsMain->InitWindow(hInstance, 0, 0, (int)g_fWindowWidth, (int)g_fWindowHeight, WND_TITLE)))
	{
		g_pClsMain->Loop();
	}

#else

	if (SUCCEEDED(g_pClsMain->InitWindow(hInstance, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), WND_TITLE)))
	{
		g_pClsMain->Loop();
	}

#endif //#if _DEBUG.

	/*====/ �I�� /====*/
	//�V���O���g���N���X�̔j��.
	SingletonFinalizer::Finalize();

	//���C���N���X�̔j��.
	delete g_pClsMain;

	//���������[�N�̃_���v�̕\��.
	_CrtDumpMemoryLeaks();
	
	return 0;
}

//�E�B���h�E�v���V�[�W��.
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam )
{
	//�v���V�[�W��.
	return g_pClsMain->MsgProc( hWnd, uMsg, wParam, lParam );
}

/*====/ ���C���N���X /====*/
Main::Main()
{
	ZeroMemory(this, sizeof(Main));

	m_hWnd = nullptr;
}

Main::~Main()
{
	SAFE_DELETE(m_pGame);
}

//�E�B���h�E�������֐�.
HRESULT Main::InitWindow(HINSTANCE hInstance, INT x, INT y, INT Width, INT Height, LPSTR WindowName)
{
	//�E�B���h�E�̒�`.
	WNDCLASSEX wc;
	ZeroMemory( &wc, sizeof( wc ) );//������(0�ݒ�)

	wc.cbSize		= sizeof( wc );
	wc.style		= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	= WndProc;
	wc.hInstance	= hInstance;
	wc.hIcon		= LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor		= LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground= (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName= APR_NAME;
	wc.hIconSm		= LoadIcon( NULL, IDI_APPLICATION );

	//�E�B���h�E�N���X��Windows�ɓo�^.
	if( !RegisterClassEx( &wc ) )
	{
		MessageBox( NULL,"�E�B���h�E�N���X�̓o�^�Ɏ��s", "error", MB_OK );
		return E_FAIL;
	}

	//�E�B���h�E�̍쐬.
#if _DEBUG
	m_hWnd = CreateWindow(
		APR_NAME,			//�A�v����.
		WindowName,			//�E�B���h�E�^�C�g��.
		WS_OVERLAPPEDWINDOW,//�E�B���h�E���(����).
		0, 0,				//�\���ʒu���W.
		Width, Height,		//�E�B���h�E�̕�,����.
		NULL,				//�e�E�B���h�E�n���h��.
		NULL,				//���j���[�ݒ�.
		hInstance,			//�C���X�^���X�ԍ�.
		NULL);				//�E�B���h�E�쐬���ɔ�������C�x���g�ɓn���f�[�^.

#else
	m_hWnd = CreateWindow(APR_NAME,WindowName,WS_VISIBLE | WS_POPUP,0, 0,Width, Height,NULL,NULL,hInstance,NULL);

#endif //#if _DEBUG.

	assert(m_hWnd != NULL);

	//�E�B���h�E�̕\��.
	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

	return S_OK;
}

//�E�B���h�E�֐�(���b�Z�[�W���̏���).
LRESULT Main::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch( uMsg )
	{
	case WM_DESTROY://�E�B���h�E���j�����ꂽ��.

		//�A�v���P�[�V�����̏I����Windows�ɒʒm����.
		PostQuitMessage( 0 );
		break;
	}

	m_pGame->MsgProc(hWnd, uMsg, wParam, lParam);
	
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//���b�Z�[�W���[�v�ƃA�v���P�[�V���������̓����.
void Main::Loop()
{
	m_pGame = new Game(m_hWnd);

	//�t���[�����[�g���ߏ���.
	float fRate		= 0.0f;	//���[�g.
	DWORD sync_old	= timeGetTime();//�ߋ�����.
	DWORD sync_now;					//���ݎ���.

	//���ԏ����ׁ̈A�ŏ��P�ʂ�1�~���b�ɕύX.
	timeBeginPeriod( 1 );
	
	//���b�Z�[�W���[�v.
	MSG msg = { 0 };
	ZeroMemory( &msg, sizeof( msg ) );

	while( msg.message != WM_QUIT )
	{
		Sleep( 1 );
		//���ݎ��Ԃ��擾.
		sync_now = timeGetTime();

		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

#ifndef _DEBUG

		//�f�o�b�O���[�h�ȊO�̎��̓}�E�X�J�[�\���������Ȃ�����.
		ShowCursor(false);

#endif //#ifndef _DEBUG.
		
		//���z���Ԃ��Z�o.
		fRate = 1000.0f / g_fFPS;
		if( sync_now - sync_old >= fRate )
		{
			//���ݎ��Ԃɒu������.
			sync_old = sync_now;

			//�Q�[�����[�v.
			m_pGame->Update();
		}
	}
	//�A�v���P�[�V�����̏I��.
	timeEndPeriod( 1 );//����.
}