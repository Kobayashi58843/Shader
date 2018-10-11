#pragma once
#pragma warning( disable:4005 )

#include "BaseScene.h"

class TitleScene : public BaseScene
{
public:
	TitleScene(SceneNeedPointer PointerGroup);
	virtual ~TitleScene();

private:
	enum enSprite
	{
		enSprite_BackGround = 0,
		enSprite_Logo,
		enSprite_StartButton,
		enSprite_EndButton,
		enSprite_StartButtonText,
		enSprite_EndButtonText,
		enSprite_Cursor,

		enSprite_Max
	};

	void CreateProduct(const enSwitchToNextScene enNextScene);

	void UpdateProduct(enSwitchToNextScene &enNextScene);

	void RenderModelProduct();

	void RenderSpriteProduct();

	/*====/ スプライト関連 /====*/
	//カーソルのアニメーション用のカウント.
	int m_iCursorAnimationCount;

	//スプライトの作成.
	void CreateSprite();

	//スプライトのフレームごとの更新.
	void UpdateSprite();

	//スプライトの位置.
	void UpdateSpritePositio(int iSpriteNo);

	//スプライトのアニメーション.
	void UpdateSpriteAnimation(int iSpriteNo);

	/*====/ シーン変更関連 /====*/
	//シーン変更.
	void ChangeScene(enSwitchToNextScene &enNextScene);

#if _DEBUG

	//デバッグテキストの表示.
	void RenderDebugText();

#endif //#if _DEBUG.

};