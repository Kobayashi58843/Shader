#include "OverScene.h"

#include "Singleton.h"

const float g_fCursorAnimationWaitTime = 0.2f;

OverScene::OverScene(SceneNeedPointer PointerGroup)
	: BaseScene(PointerGroup)
{
	m_iCursorAnimationCount = 0;

	//全サウンドを停止する.
	Singleton<SoundManager>().GetInstance().StopSound();
}

OverScene::~OverScene()
{
	for (unsigned int i = 0; i < m_vpSprite.size(); i++)
	{
		SAFE_DELETE(m_vpSprite[i]);
	}
	//使っているサイズを0にする.
	m_vpSprite.clear();
	//キャパシティを現在のサイズにあわせる.
	m_vpSprite.shrink_to_fit();
}

//作成.
void OverScene::CreateProduct(const enSwitchToNextScene enNextScene)
{
	//スプライトの作成.
	CreateSprite();
}

//更新.
void OverScene::UpdateProduct(enSwitchToNextScene &enNextScene)
{
	//BGMをループで再生.
	Singleton<SoundManager>().GetInstance().PlayBGM(SoundManager::enBGM_Over);

	//スプライト更新.
	UpdateSprite();

	//左クリックされた時.
	if (Singleton<RawInput>().GetInstance().IsLButtonDown())
	{
		//カーソルがボタンの上にあるか.
		if (IsHittingOfSprite(enSprite_Cursor, enSprite_ReturnButton))
		{
			enNextScene = enSwitchToNextScene::Title;
		}
	}
}

//3Dモデルの描画.
void OverScene::RenderModelProduct()
{
}

/*====/ スプライト関連 /====*/
//2Dスプライトの描画.
void OverScene::RenderSpriteProduct()
{
	for (unsigned int i = 0; i < m_vpSprite.size(); i++)
	{
		m_vpSprite[i]->Render();
	}
}

//スプライトの作成.
void OverScene::CreateSprite()
{
	//キャパシティを確定させる.
	m_vpSprite.reserve(enSprite_Max);

	SpriteState SpriteData;

	//各スプライトの設定.
	for (int i = 0; i < enSprite_Max; i++)
	{
		switch (i)
		{
		case enSprite_BackGround:
			SpriteData =
			{ "Data\\Image\\BackGround.jpg"	//ファイルまでのパス.
			, { 1.0f, 1.0f } };				//元画像を何分割するか.

			break;
		case enSprite_Logo:
			SpriteData = { "Data\\Image\\LogoText.png", { 1.0f, 3.0f } };

			break;
		case enSprite_ReturnButton:
			SpriteData = { "Data\\Image\\End.jpg", { 1.0f, 2.0f } };

			break;
		case enSprite_ReturnButtonText:
			SpriteData = { "Data\\Image\\ButtonText.png", { 1.0f, 3.0f } };

			break;
		case enSprite_Cursor:
			SpriteData = { "Data\\Image\\Cursor.png", { 8.0f, 1.0f } };

			break;
		default:
			ERR_MSG("Clear::CreateSprite()", "error");

			break;
		}

		//配列を一つ増やす.
		m_vpSprite.push_back(new Sprite(SpriteData.vDivisionQuantity.x, SpriteData.vDivisionQuantity.y));
		m_vpSprite[i]->Create(m_SceneNeedPointer.pDevice, m_SceneNeedPointer.pContext, SpriteData.sPath);
	}
}

//スプライトのフレームごとの更新.
void OverScene::UpdateSprite()
{
	for (int i = 0; i < enSprite_Max; i++)
	{
		UpdateSpritePositio(i);
		UpdateSpriteAnimation(i);
	}
}

//スプライトの位置.
void OverScene::UpdateSpritePositio(int iSpriteNo)
{
	//ウインドウの中心.
	float fWindowWidthCenter = g_fWindowWidth / 2.0f;
	float fWindowHeightCenter = g_fWindowHeight / 2.0f;

	//スプライト位置.
	D3DXVECTOR2 vPosition;

	switch (iSpriteNo)
	{
	case enSprite_BackGround:
		vPosition.x = fWindowWidthCenter;
		vPosition.y = fWindowHeightCenter;

		break;
	case enSprite_Logo:
		vPosition.x = fWindowWidthCenter;
		vPosition.y = fWindowHeightCenter / 2.0f;

		break;
	case enSprite_ReturnButton:
		vPosition.x = fWindowWidthCenter;
		vPosition.y = fWindowHeightCenter + (fWindowHeightCenter / 2.0f);

		break;
	case enSprite_ReturnButtonText:
		vPosition.x = fWindowWidthCenter;
		vPosition.y = fWindowHeightCenter + (fWindowHeightCenter / 2.0f);

		break;
	case enSprite_Cursor:
		//マウスの座標.
		POINT MousePosition;
		GetCursorPos(&MousePosition);

		vPosition.x = (float)MousePosition.x;
		vPosition.y = (float)MousePosition.y;

		break;
	default:
		ERR_MSG("Clear::UpdateSpritePositio()", "error");

		break;
	}

	m_vpSprite[iSpriteNo]->SetPos(vPosition.x, vPosition.y);
}

//スプライトのアニメーション.
void OverScene::UpdateSpriteAnimation(int iSpriteNo)
{
	switch (iSpriteNo)
	{
	case enSprite_BackGround:

		break;
	case enSprite_Logo:
		m_vpSprite[iSpriteNo]->SetPatternNoHeight(2.0f);

		break;
	case enSprite_ReturnButton:
		//カーソルとボタンが接触していた時.
		if (IsHittingOfSprite(enSprite_Cursor, iSpriteNo))
		{
			m_vpSprite[iSpriteNo]->SetPatternNoHeight(1.0f);
		}
		else
		{
			m_vpSprite[iSpriteNo]->SetPatternNoHeight(0.0f);
		}

		break;
	case enSprite_ReturnButtonText:
		m_vpSprite[iSpriteNo]->SetPatternNoHeight(2.0f);

		break;
	case enSprite_Cursor:
		if (IsHittingOfSprite(enSprite_Cursor, enSprite_ReturnButton))
		{
			m_iCursorAnimationCount++;
			if (m_iCursorAnimationCount / g_fFPS >= g_fCursorAnimationWaitTime)
			{
				m_vpSprite[iSpriteNo]->AddPatternNoWidth(1.0f);
				m_iCursorAnimationCount = 0;

				int iPatternNo = (int)m_vpSprite[iSpriteNo]->GetPatternNo().x;
				if (iPatternNo % 2 == 0)
				{
					Singleton<SoundManager>().GetInstance().PlaySE(SoundManager::enSE_Cursor);
				}
			}
		}

		break;
	default:
		ERR_MSG("Clear::UpdateSpriteAnimation()", "error");

		break;
	}
}


#if _DEBUG

//デバッグテキストの表示.
void OverScene::RenderDebugText()
{
	char cStrDbgTxt[512];

	sprintf_s(cStrDbgTxt, "Scene : Over");
	m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 0));
}

#endif //#if _DEBUG.