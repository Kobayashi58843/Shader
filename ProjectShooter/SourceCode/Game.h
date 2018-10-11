#pragma once
#pragma warning( disable:4005 )

#include "Global.h"

#include "Direct3D.h"

#include "SceneFactory.h"

#include "BaseScene.h"

class Game
{
public:
	Game(const HWND hWnd);
	~Game();

	//���b�Z�[�W���̏���.
	void MsgProc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam);

	/*====/ ���t���[���X�V����֐� /====*/
	//�X�V.
	void Update();

private:
	Direct3D* m_pDirect3D;

	//Direct3D�N���X�ō쐬���ꂽ�|�C���^�̊i�[�p.
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pDeviceContext;

	/*====/ �V�[���֘A /====*/
	SceneFactory m_SceneFactory;
	BaseScene* m_pScene;

	//�V�[���ɕK�v�ȃ|�C���^�̍\����.
	SceneNeedPointer m_SceneNeedPointer;

	//�V�[���؂�ւ����̃t�F�[�h�p�摜.
	Sprite* m_pFadeSprite;

	bool m_bBlackOut;

	//�V�[���̕ύX��̈ꎞ�ۑ��p.
	enSwitchToNextScene m_NextSceneTemporary;

	//�t�F�[�h.
	void Fade(const enSwitchToNextScene enNextScene);

	//�V�[���؂�ւ�.
	void SwitchScene(const enSwitchToNextScene enNextScene);

	/*====/ ���t���[���X�V����֐� /====*/
	//�`��.
	void Render();
};