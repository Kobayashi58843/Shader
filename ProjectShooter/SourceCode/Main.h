#pragma once
#pragma warning( disable:4005 )	

#include <Windows.h>

#include "Game.h"

//�萔.
#define WND_TITLE	"MissileBozu"
#define APR_NAME	"MissileBozu"

/*====/ ���C���N���X /====*/
class Main
{
private:
	HWND	m_hWnd;

public:
	Main();
	~Main();

	//�E�B���h�E������.
	HRESULT InitWindow(HINSTANCE hInstance,INT x, INT y, INT Width, INT Height,LPSTR WindowName );

	//�E�B���h�E�֐�(���b�Z�[�W���̏���).
	LRESULT MsgProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam );

	//���b�Z�[�W���[�v.
	void Loop();

private:
	//�Q�[��.
	Game* m_pGame;

public:
};