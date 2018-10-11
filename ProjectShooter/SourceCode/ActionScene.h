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

	/*====/ カメラ関連 /====*/
	Camera* m_pCamera;

	float m_fOldCameraDistance;

	//カメラの操作.
	void ControlCameraMove();

	/*====/ モデル関連 /====*/
	Player* m_pPlayer;

	Enemy* m_pEnemy;

	clsDX9Mesh* m_pGround;

	BulletManager* m_pBulletManager;

	/*====/ スプライト関連 /====*/

	//スプライトの作成.
	void CreateSprite();

	//スプライトのフレームごとの更新.
	void UpdateSprite();

	//スプライトの位置.
	void UpdateSpritePositio(int iSpriteNo);

	//スプライトのアニメーション.
	void UpdateSpriteAnimation(int iSpriteNo);

#if _DEBUG

	//デバッグテキストの表示.
	void RenderDebugText();

	//デバッグ中のみの操作.
	void DebugKeyControl();

#endif //#if _DEBUG.

};