#pragma once
#pragma warning( disable:4005 )

#include "BaseScene.h"

class ClearScene : public BaseScene
{
public:
	ClearScene(SceneNeedPointer PointerGroup);
	virtual ~ClearScene();

private:
	enum enSprite
	{
		enSprite_BackGround = 0,
		enSprite_Logo,
		enSprite_ReturnButton,
		enSprite_ReturnButtonText,
		enSprite_Cursor,

		enSprite_Max
	};

	void CreateProduct(const enSwitchToNextScene enNextScene);

	void UpdateProduct(enSwitchToNextScene &enNextScene);

	void RenderModelProduct();

	void RenderSpriteProduct();

	/*====/ �X�v���C�g�֘A /====*/
	//�J�[�\���̃A�j���[�V�����p�̃J�E���g.
	int m_iCursorAnimationCount;

	//�X�v���C�g�̍쐬.
	void CreateSprite();

	//�X�v���C�g�̃t���[�����Ƃ̍X�V.
	void UpdateSprite();

	//�X�v���C�g�̈ʒu.
	void UpdateSpritePositio(int iSpriteNo);

	//�X�v���C�g�̃A�j���[�V����.
	void UpdateSpriteAnimation(int iSpriteNo);

#if _DEBUG

	//�f�o�b�O�e�L�X�g�̕\��.
	void RenderDebugText();

#endif //#if _DEBUG.

};