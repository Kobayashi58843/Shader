#pragma once
#pragma warning( disable:4005 )

#include "BaseScene.h"

#include "Camera.h"
#include "Player.h"
#include "Enemy.h"

#include "BulletManager.h"

class ActionScene : public BaseScene
{
public:
	ActionScene(SceneNeedPointer PointerGroup);
	virtual ~ActionScene();

private:
	enum enSprite
	{
		enSprite_Alignment,
		enSprite_PlayerHp,
		enSprite_PlayerHpFrame,
		enSprite_EnemyHp,
		enSprite_EnemyHpFrame,

		enSprite_Max
	};

	void CreateProduct(const enSwitchToNextScene enNextScene);

	void UpdateProduct(enSwitchToNextScene &enNextScene);

	void RenderModelProduct();

	void RenderSpriteProduct();

	/*====/ �J�����֘A /====*/
	Camera* m_pCamera;

	float m_fOldCameraDistance;

	//�J�����̑���.
	void ControlCameraMove();

	/*====/ ���f���֘A /====*/
	Player* m_pPlayer;

	Enemy* m_pEnemy;

	clsDX9Mesh* m_pGround;

	BulletManager* m_pBulletManager;

	/*====/ �X�v���C�g�֘A /====*/

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

	//�f�o�b�O���݂̂̑���.
	void DebugKeyControl();

#endif //#if _DEBUG.

};