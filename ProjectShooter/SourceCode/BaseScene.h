#pragma once
#pragma warning( disable:4005 )

#include "Global.h"
#include <vector>

#include "Sprite.h"
#include "DebugText.h"
#include "RawInput.h"

#include "SoundManager.h"

struct SceneNeedPointer
{
	//�E�C���h�E�n���h��.
	HWND hWnd;
	//�f�o�C�X�I�u�W�F�N�g.
	ID3D11Device* pDevice;
	//�f�o�C�X�R���e�L�X�g.
	ID3D11DeviceContext* pContext;

	//�e�|�C���^�[�ƃC���X�^���X���Z�b�g.
	void SetSceneNeedPointer(const HWND Wnd, ID3D11Device* const Device, ID3D11DeviceContext* const Context)
	{
		hWnd = Wnd;
		pDevice = Device;
		pContext = Context;
	}
};

class BaseScene
{
public:
	BaseScene(SceneNeedPointer PointerGroup);
	virtual ~BaseScene();

	/*====/ �ǂ̃V�[���ł������������������ /====*/
	//�쐬.
	void Create(const enSwitchToNextScene enNextScene);

	//�X�V.
	void Update(enSwitchToNextScene &enNextScene);

	//3D���f���̕`��.
	void RenderModel();

	//2D�X�v���C�g�̕`��.
	void RenderSprite();

protected:
	SceneNeedPointer m_SceneNeedPointer;

	//�X�v���C�g.
	std::vector<Sprite*> m_vpSprite;

	/*====/ �e�V�[�����ƂɈႤ������������� /====*/
	//�e�V�[���̍쐬.
	virtual void CreateProduct(const enSwitchToNextScene enNextScene) = 0;

	//�e�V�[���̍X�V.
	virtual void UpdateProduct(enSwitchToNextScene &enNextScene) = 0;

	//�e�V�[����3D���f���̕`��.
	virtual void RenderModelProduct() = 0;

	//�e�V�[����2D�X�v���C�g�̕`��.
	virtual void RenderSpriteProduct() = 0;

	//�X�v���C�g�̍쐬.
	virtual void CreateSprite() = 0;

	//�X�v���C�g�̃t���[�����Ƃ̍X�V.
	virtual void UpdateSprite() = 0;

	//�X�v���C�g�̈ʒu.
	virtual void UpdateSpritePositio(int iSpriteNo) = 0;

	//�X�v���C�g�̃A�j���[�V����.
	virtual void UpdateSpriteAnimation(int iSpriteNo) = 0;

	//�X�v���C�g���m�̐ڐG.
	bool IsHittingOfSprite(int iSourceSpriteNo, int iTargetSpriteNo);

	/*====/ �f�o�b�O�p /====*/
#if _DEBUG

	void InputUpdate(enSwitchToNextScene &enNextScene);

	DebugText* m_pDebugText;

	//�f�o�b�O�e�L�X�g�̕\��.
	virtual void RenderDebugText() = 0;

#endif //#if _DEBUG.
};