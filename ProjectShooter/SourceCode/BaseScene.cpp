#include "BaseScene.h"

BaseScene::BaseScene(SceneNeedPointer PointerGroup)
	: m_SceneNeedPointer(PointerGroup)
{
#if _DEBUG

	//デバッグテキストの作成.
	m_pDebugText = new DebugText;
	//文字色.
	D3DXVECTOR4 vColor(1.0f, 1.0f, 1.0f, 1.0f);
	if (FAILED(m_pDebugText->Init(m_SceneNeedPointer.pContext, (DWORD)g_fWindowWidth, (DWORD)g_fWindowHeight, 100.0f, vColor)))
	{
		MessageBox(NULL, "デバッグテキスト作成失敗", "error", MB_OK);
	}
#endif //#if _DEBUG.

	m_vpSprite.resize(0);
}

BaseScene::~BaseScene()
{
#if _DEBUG

	//デバッグテキストの削除.
	SAFE_DELETE(m_pDebugText);

#endif //#if _DEBUG.
}

//作成.
void BaseScene::Create(const enSwitchToNextScene enNextScene)
{
	//各シーンの作成.
	CreateProduct(enNextScene);
}

//更新.
void BaseScene::Update(enSwitchToNextScene &enNextScene)
{
	//各シーンの更新.
	UpdateProduct(enNextScene);

	/*====/ デバッグ用 /====*/
#if _DEBUG

	//入力情報の更新.
	InputUpdate(enNextScene);

#endif //#if _DEBUG.
}

//3Dモデルの描画.
void BaseScene::RenderModel()
{
	//各シーンの3Dモデルの描画.
	RenderModelProduct();
}

//2Dスプライトの描画.
void BaseScene::RenderSprite()
{
	//各シーンの2Dスプライトの描画.
	RenderSpriteProduct();

#if _DEBUG

	//デバッグテキストの表示.
	RenderDebugText();

#endif //#if _DEBUG.
}

//スプライト同士の接触.
bool BaseScene::IsHittingOfSprite(int iSourceSpriteNo, int iTargetSpriteNo)
{
	D3DXVECTOR2 vSourceSpriteSize = m_vpSprite[iSourceSpriteNo]->GetSize();
	D3DXVECTOR3 vSourceSpritePos = m_vpSprite[iSourceSpriteNo]->GetPos();

	D3DXVECTOR2 vTargetSpriteSize = m_vpSprite[iTargetSpriteNo]->GetSize();
	D3DXVECTOR3 vTargetSpritePos = m_vpSprite[iTargetSpriteNo]->GetPos();

	const float fHalf = 2.0f;

	if (vSourceSpritePos.x + (vSourceSpriteSize.x / fHalf) > vTargetSpritePos.x - (vTargetSpriteSize.x / fHalf) &&
		vSourceSpritePos.x - (vSourceSpriteSize.x / fHalf) < vTargetSpritePos.x + (vTargetSpriteSize.x / fHalf))
	{
		if (vSourceSpritePos.y + (vSourceSpriteSize.y / fHalf) > vTargetSpritePos.y - (vTargetSpriteSize.y / fHalf) &&
			vSourceSpritePos.y - (vSourceSpriteSize.y / fHalf) < vTargetSpritePos.y + (vTargetSpriteSize.y / fHalf))
		{
			return true;
		}
	}

	return false;
}

/*====/ デバッグ用 /====*/
#if _DEBUG

//入力情報の更新.
void BaseScene::InputUpdate(enSwitchToNextScene &enNextScene)
{
	//シーン切り替え.
	if(GetAsyncKeyState('1') & 0x1)
	{
		enNextScene = enSwitchToNextScene::Title;
	}
	else if(GetAsyncKeyState('2') & 0x1)
	{
		enNextScene = enSwitchToNextScene::Action;
	}
	else if(GetAsyncKeyState('3') & 0x1)
	{
		enNextScene = enSwitchToNextScene::Clear;
	}
	else if(GetAsyncKeyState('4') & 0x1)
	{
		enNextScene = enSwitchToNextScene::Over;
	}
}

#endif //#if _DEBUG.