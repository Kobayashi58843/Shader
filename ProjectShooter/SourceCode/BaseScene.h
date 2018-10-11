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
	//ウインドウハンドル.
	HWND hWnd;
	//デバイスオブジェクト.
	ID3D11Device* pDevice;
	//デバイスコンテキスト.
	ID3D11DeviceContext* pContext;

	//各ポインターとインスタンスをセット.
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

	/*====/ どのシーンでも同じ処理をするもの /====*/
	//作成.
	void Create(const enSwitchToNextScene enNextScene);

	//更新.
	void Update(enSwitchToNextScene &enNextScene);

	//3Dモデルの描画.
	void RenderModel();

	//2Dスプライトの描画.
	void RenderSprite();

protected:
	SceneNeedPointer m_SceneNeedPointer;

	//スプライト.
	std::vector<Sprite*> m_vpSprite;

	/*====/ 各シーンごとに違う処理をするもの /====*/
	//各シーンの作成.
	virtual void CreateProduct(const enSwitchToNextScene enNextScene) = 0;

	//各シーンの更新.
	virtual void UpdateProduct(enSwitchToNextScene &enNextScene) = 0;

	//各シーンの3Dモデルの描画.
	virtual void RenderModelProduct() = 0;

	//各シーンの2Dスプライトの描画.
	virtual void RenderSpriteProduct() = 0;

	//スプライトの作成.
	virtual void CreateSprite() = 0;

	//スプライトのフレームごとの更新.
	virtual void UpdateSprite() = 0;

	//スプライトの位置.
	virtual void UpdateSpritePositio(int iSpriteNo) = 0;

	//スプライトのアニメーション.
	virtual void UpdateSpriteAnimation(int iSpriteNo) = 0;

	//スプライト同士の接触.
	bool IsHittingOfSprite(int iSourceSpriteNo, int iTargetSpriteNo);

	/*====/ デバッグ用 /====*/
#if _DEBUG

	void InputUpdate(enSwitchToNextScene &enNextScene);

	DebugText* m_pDebugText;

	//デバッグテキストの表示.
	virtual void RenderDebugText() = 0;

#endif //#if _DEBUG.
};