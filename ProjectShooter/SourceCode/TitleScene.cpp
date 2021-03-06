#include "TitleScene.h"

#include "Singleton.h"

const float g_fCursorAnimationWaitTime = 0.2f;

TitleScene::TitleScene(SceneNeedPointer PointerGroup)
	: BaseScene(PointerGroup)
{
	m_iCursorAnimationCount = 0;

	//全サウンドを停止する.
	Singleton<SoundManager>().GetInstance().StopSound();

	//シーン移動時のSE.
	Singleton<SoundManager>().GetInstance().PlaySE(SoundManager::enSE_PushButton);
}

TitleScene::~TitleScene()
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
void TitleScene::CreateProduct(const enSwitchToNextScene enNextScene)
{
	//スプライトの作成.
	CreateSprite();
}

//更新.
void TitleScene::UpdateProduct(enSwitchToNextScene &enNextScene)
{
	//BGMをループで再生.
	Singleton<SoundManager>().GetInstance().PlayBGM(SoundManager::enBGM_Title);

	//スプライト更新.
	UpdateSprite();

	//シーン変更.
	ChangeScene(enNextScene);
}

//3Dモデルの描画.
void TitleScene::RenderModelProduct()
{
}

/*====/ スプライト関連 /====*/
//2Dスプライトの描画.
void TitleScene::RenderSpriteProduct()
{
	for (unsigned int i = 0; i < m_vpSprite.size(); i++)
	{
		m_vpSprite[i]->Render();
	}
}

//スプライトの作成.
void TitleScene::CreateSprite()
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
		case enSprite_StartButton:
			SpriteData = { "Data\\Image\\Push.jpg", { 1.0f, 2.0f } };

			break;
		case enSprite_EndButton:
			SpriteData = { "Data\\Image\\End.jpg", { 1.0f, 2.0f } };

			break;
		case enSprite_StartButtonText:
			SpriteData = { "Data\\Image\\ButtonText.png", { 1.0f, 3.0f } };

			break;
		case enSprite_EndButtonText:
			SpriteData = { "Data\\Image\\ButtonText.png", { 1.0f, 3.0f } };

			break;
		case enSprite_Cursor:
			SpriteData = { "Data\\Image\\Cursor.png", { 8.0f, 1.0f } };

			break;
		default:
			ERR_MSG("Title::CreateSprite()","error");

			break;
		}

		//配列を一つ増やす.
		m_vpSprite.push_back(new Sprite(SpriteData.vDivisionQuantity.x, SpriteData.vDivisionQuantity.y));
		m_vpSprite[i]->Create(m_SceneNeedPointer.pDevice, m_SceneNeedPointer.pContext, SpriteData.sPath);
	}
}

//スプライトのフレームごとの更新.
void TitleScene::UpdateSprite()
{
	for (int i = 0; i < enSprite_Max; i++)
	{
		UpdateSpritePositio(i);
		UpdateSpriteAnimation(i);
	}
}

//スプライトの位置.
void TitleScene::UpdateSpritePositio(int iSpriteNo)
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
	case enSprite_StartButton:
		vPosition.x = fWindowWidthCenter - (fWindowWidthCenter / 2.0f);
		vPosition.y = fWindowHeightCenter + (fWindowHeightCenter / 2.0f);

		break;
	case enSprite_EndButton:
		vPosition.x = fWindowWidthCenter + (fWindowWidthCenter / 2.0f);
		vPosition.y = fWindowHeightCenter + (fWindowHeightCenter / 2.0f);
		
		break;
	case enSprite_StartButtonText:
		vPosition.x = fWindowWidthCenter - (fWindowWidthCenter / 2.0f);
		vPosition.y = fWindowHeightCenter + (fWindowHeightCenter / 2.0f);

		break;
	case enSprite_EndButtonText:
		vPosition.x = fWindowWidthCenter + (fWindowWidthCenter / 2.0f);
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
		ERR_MSG("Title::UpdateSpritePositio()", "error");

		break;
	}

	m_vpSprite[iSpriteNo]->SetPos(vPosition.x, vPosition.y);
}

//スプライトのアニメーション.
void TitleScene::UpdateSpriteAnimation(int iSpriteNo)
{
	switch (iSpriteNo)
	{
	case enSprite_BackGround:

		break;
	case enSprite_Logo:

		break;
	case enSprite_StartButton:
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
	case enSprite_EndButton:
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
	case enSprite_StartButtonText:

		break;
	case enSprite_EndButtonText:
		m_vpSprite[iSpriteNo]->SetPatternNoHeight(1.0f);

		break;
	case enSprite_Cursor:
		if (IsHittingOfSprite(enSprite_Cursor, enSprite_StartButton)
			|| IsHittingOfSprite(enSprite_Cursor, enSprite_EndButton))
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
		ERR_MSG("Title::UpdateSpriteAnimation()", "error");

		break;
	}
}

/*====/ シーン変更関連 /====*/
//シーン変更.
void TitleScene::ChangeScene(enSwitchToNextScene &enNextScene)
{
	//左クリックされた時.
	if (Singleton<RawInput>().GetInstance().IsLButtonDown())
	{
		//カーソルがスタートボタンの上にあるか.
		if (IsHittingOfSprite(enSprite_Cursor, enSprite_StartButton))
		{
			enNextScene = enSwitchToNextScene::Action;
		}

		//カーソルがエンドボタンの上にあるか.
		if (IsHittingOfSprite(enSprite_Cursor, enSprite_EndButton))
		{
			DestroyWindow(m_SceneNeedPointer.hWnd);
		}
	}
}

#if _DEBUG

//デバッグテキストの表示.
void TitleScene::RenderDebugText()
{
	char cStrDbgTxt[512];

	sprintf_s(cStrDbgTxt, "Scene : Title");
	m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 0));

	if (Singleton<RawInput>().GetInstance().IsLButtonDown())
	{
		sprintf_s(cStrDbgTxt, "IsLButtonDown : true");
		m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 1));
	}
	else
	{
		sprintf_s(cStrDbgTxt, "IsLButtonDown : false");
		m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 1));
	}
	
	if (Singleton<RawInput>().GetInstance().IsRButtonDown())
	{
		sprintf_s(cStrDbgTxt, "IsRButtonDown : true");
		m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 2));
	}
	else
	{
		sprintf_s(cStrDbgTxt, "IsRButtonDown : false");
		m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 2));
	}
}

#endif //#if _DEBUG.