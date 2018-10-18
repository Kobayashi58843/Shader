#include "Game.h"

#include "SoundManager.h"
#include "CEffects.h"
#include "Resource.h"

#include "Singleton.h"

//ゲーム開始時のシーン.
const enSwitchToNextScene g_StartStateOfScene = enSwitchToNextScene::Title;

Game::Game(const HWND hWnd)
{
	//Direct3D作成.
	m_pDirect3D = new Direct3D;
	m_pDirect3D->Create(hWnd);

	//作成されたDirect3Dからデバイスなどを取ってくる.
	m_pDevice = m_pDirect3D->GetDevice();
	m_pDeviceContext = m_pDirect3D->GetDeviceContext();

	m_pScene = nullptr;

	//シェーダの作成.
	Singleton<ShaderGathering>().GetInstance().InitShader(m_pDevice, m_pDeviceContext);

	//リソースの作成.
	Singleton<Resource>().GetInstance().Create(hWnd, m_pDevice, m_pDeviceContext);

	//BGMとSEを作成.
	Singleton<SoundManager>().GetInstance().LoadSound(hWnd);

	//エフェクトの作成.
	clsEffects::GetInstance()->Create(m_pDevice, m_pDeviceContext);
	clsEffects::GetInstance()->LoadData();

	//RawInput初期化.
	Singleton<RawInput>().GetInstance().InitRawInput(hWnd);

	//シーンに必要なポインタをセット.
	m_SceneNeedPointer.SetSceneNeedPointer(hWnd, m_pDevice, m_pDeviceContext);
	SwitchScene(g_StartStateOfScene);

	/*====/ フェード用スプライト関連 /====*/
	D3DXVECTOR2 vDivisionQuantity = { 1.0f, 1.0f };
	m_pFadeSprite = new Sprite(vDivisionQuantity.x, vDivisionQuantity.y);
	m_pFadeSprite->Create(m_pDevice, m_pDeviceContext, "Data\\Image\\Fade.jpg");

	//位置をウインドウの中心に設定.
	float fWindowWidthCenter = g_fWindowWidth / 2.0f;
	float fWindowHeightCenter = g_fWindowHeight / 2.0f;
	m_pFadeSprite->SetPos(fWindowWidthCenter, fWindowHeightCenter);

	//はじめは非表示に設定しておく.
	m_pFadeSprite->SetDispFlg(false);

	//透過値を0にする.
	m_pFadeSprite->SetAlpha(0.0f);

	m_bBlackOut = false;

	m_NextSceneTemporary = enSwitchToNextScene::Nothing;
}

Game::~Game()
{
	SAFE_DELETE(m_pFadeSprite);

	SAFE_DELETE(m_pScene);

	clsEffects::GetInstance()->Destroy();

	SAFE_DELETE(m_pDirect3D);

	m_pDeviceContext = nullptr;
	m_pDevice = nullptr;
}

//メッセージ毎の処理.
void Game::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		//キーボードが押されたときのキー別の処理.
		switch ((char)wParam)
		{
		case VK_ESCAPE:
			//ウィンドウを破棄する.
			DestroyWindow(hWnd);

			break;
		}
		break;
	}

	Singleton<RawInput>().GetInstance().MsgProc(uMsg, lParam);
}

//更新.
void Game::Update()
{
	enSwitchToNextScene enSwitchNextScene = enSwitchToNextScene::Nothing;

	assert(m_pScene != nullptr);
	m_pScene->Update(enSwitchNextScene);

	//シーンの変更があればフェードさせながらシーンを切り替える.
	Fade(enSwitchNextScene);

	//シーンの描画.
	Render();

	//マウスの入力情報をクリア.
	Singleton<RawInput>().GetInstance().ClearPerFrame();
}

//フェード.
void Game::Fade(const enSwitchToNextScene enNextScene)
{
	//シーンの変更があったとき.
	if (enNextScene != enSwitchToNextScene::Nothing)
	{
		//フェード用の画像が表示可能かどうかでフェード中を判断する.
		//フェード中でない場合.
		if (!m_pFadeSprite->IsDispFlg())
		{
			//フェード用の画像を表示可能にする.
			m_pFadeSprite->SetDispFlg(true);
			m_bBlackOut = true;
			m_NextSceneTemporary = enNextScene;
		}
	}

	if (m_pFadeSprite->IsDispFlg())
	{
		//フェードの速度.
		const float fFadeSpeed = 0.05f;

		if (m_bBlackOut)
		{
			if (1.0f > m_pFadeSprite->GetAlpha())
			{
				m_pFadeSprite->AddAlpha(fFadeSpeed);
			}
			else
			{
				//透過値が最大になった時にシーンを切り替える.
				SwitchScene(m_NextSceneTemporary);
				m_bBlackOut = false;
			}
		}
		else
		{
			if (0.0f < m_pFadeSprite->GetAlpha())
			{
				m_pFadeSprite->AddAlpha(-fFadeSpeed);
			}
			else
			{
				//フェード用の画像を非表示にする.
				m_pFadeSprite->SetDispFlg(false);
				m_pFadeSprite->SetAlpha(0.0f);
			}
		}
	}
}

//シーン切り替え.
void Game::SwitchScene(const enSwitchToNextScene enNextScene)
{
	//前のシーンを消す.
	SAFE_DELETE(m_pScene);

	//次のシーンに切り替える.
	m_pScene = m_SceneFactory.Create(enNextScene, m_SceneNeedPointer);
	m_pScene->Create(enNextScene);
}

//描画.
void Game::Render()
{
	m_pDirect3D->ClearBackBuffer();

	assert(m_pScene != nullptr);

	//深度テストを有効.
	m_pDirect3D->SetDepth(true);

	//3Dモデルの描画.
	m_pScene->RenderModel();

	//深度テストを無効.
	m_pDirect3D->SetDepth(false);

	//2Dスプライトの描画.
	m_pScene->RenderSprite();

	//フェード用画像の描画.
	m_pFadeSprite->Render();

	m_pDirect3D->RenderSwapChain();
}