#include "TitleScene.h"

#include "Singleton.h"

const float g_fCursorAnimationWaitTime = 0.2f;

TitleScene::TitleScene(SceneNeedPointer PointerGroup)
	: BaseScene(PointerGroup)
{
	m_iCursorAnimationCount = 0;

	//�S�T�E���h���~����.
	Singleton<SoundManager>().GetInstance().StopSound();

	//�V�[���ړ�����SE.
	Singleton<SoundManager>().GetInstance().PlaySE(SoundManager::enSE_PushButton);
}

TitleScene::~TitleScene()
{
	for (unsigned int i = 0; i < m_vpSprite.size(); i++)
	{
		SAFE_DELETE(m_vpSprite[i]);
	}
	//�g���Ă���T�C�Y��0�ɂ���.
	m_vpSprite.clear();
	//�L���p�V�e�B�����݂̃T�C�Y�ɂ��킹��.
	m_vpSprite.shrink_to_fit();
}

//�쐬.
void TitleScene::CreateProduct(const enSwitchToNextScene enNextScene)
{
	//�X�v���C�g�̍쐬.
	CreateSprite();
}

//�X�V.
void TitleScene::UpdateProduct(enSwitchToNextScene &enNextScene)
{
	//BGM�����[�v�ōĐ�.
	Singleton<SoundManager>().GetInstance().PlayBGM(SoundManager::enBGM_Title);

	//�X�v���C�g�X�V.
	UpdateSprite();

	//�V�[���ύX.
	ChangeScene(enNextScene);
}

//3D���f���̕`��.
void TitleScene::RenderModelProduct()
{
}

/*====/ �X�v���C�g�֘A /====*/
//2D�X�v���C�g�̕`��.
void TitleScene::RenderSpriteProduct()
{
	for (unsigned int i = 0; i < m_vpSprite.size(); i++)
	{
		m_vpSprite[i]->Render();
	}
}

//�X�v���C�g�̍쐬.
void TitleScene::CreateSprite()
{
	//�L���p�V�e�B���m�肳����.
	m_vpSprite.reserve(enSprite_Max);

	SpriteState SpriteData;

	//�e�X�v���C�g�̐ݒ�.
	for (int i = 0; i < enSprite_Max; i++)
	{
		switch (i)
		{
		case enSprite_BackGround:
			SpriteData =
			{ "Data\\Image\\BackGround.jpg"	//�t�@�C���܂ł̃p�X.
			, { 1.0f, 1.0f } };				//���摜�����������邩.

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

		//�z�������₷.
		m_vpSprite.push_back(new Sprite(SpriteData.vDivisionQuantity.x, SpriteData.vDivisionQuantity.y));
		m_vpSprite[i]->Create(m_SceneNeedPointer.pDevice, m_SceneNeedPointer.pContext, SpriteData.sPath);
	}
}

//�X�v���C�g�̃t���[�����Ƃ̍X�V.
void TitleScene::UpdateSprite()
{
	for (int i = 0; i < enSprite_Max; i++)
	{
		UpdateSpritePositio(i);
		UpdateSpriteAnimation(i);
	}
}

//�X�v���C�g�̈ʒu.
void TitleScene::UpdateSpritePositio(int iSpriteNo)
{
	//�E�C���h�E�̒��S.
	float fWindowWidthCenter = g_fWindowWidth / 2.0f;
	float fWindowHeightCenter = g_fWindowHeight / 2.0f;

	//�X�v���C�g�ʒu.
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
		//�}�E�X�̍��W.
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

//�X�v���C�g�̃A�j���[�V����.
void TitleScene::UpdateSpriteAnimation(int iSpriteNo)
{
	switch (iSpriteNo)
	{
	case enSprite_BackGround:

		break;
	case enSprite_Logo:

		break;
	case enSprite_StartButton:
		//�J�[�\���ƃ{�^�����ڐG���Ă�����.
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
		//�J�[�\���ƃ{�^�����ڐG���Ă�����.
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

/*====/ �V�[���ύX�֘A /====*/
//�V�[���ύX.
void TitleScene::ChangeScene(enSwitchToNextScene &enNextScene)
{
	//���N���b�N���ꂽ��.
	if (Singleton<RawInput>().GetInstance().IsLButtonDown())
	{
		//�J�[�\�����X�^�[�g�{�^���̏�ɂ��邩.
		if (IsHittingOfSprite(enSprite_Cursor, enSprite_StartButton))
		{
			enNextScene = enSwitchToNextScene::Action;
		}

		//�J�[�\�����G���h�{�^���̏�ɂ��邩.
		if (IsHittingOfSprite(enSprite_Cursor, enSprite_EndButton))
		{
			DestroyWindow(m_SceneNeedPointer.hWnd);
		}
	}
}

#if _DEBUG

//�f�o�b�O�e�L�X�g�̕\��.
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