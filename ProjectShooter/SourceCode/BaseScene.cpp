#include "BaseScene.h"

BaseScene::BaseScene(SceneNeedPointer PointerGroup)
	: m_SceneNeedPointer(PointerGroup)
{
#if _DEBUG

	//�f�o�b�O�e�L�X�g�̍쐬.
	m_pDebugText = new DebugText;
	//�����F.
	D3DXVECTOR4 vColor(1.0f, 1.0f, 1.0f, 1.0f);
	if (FAILED(m_pDebugText->Init(m_SceneNeedPointer.pContext, (DWORD)g_fWindowWidth, (DWORD)g_fWindowHeight, 100.0f, vColor)))
	{
		MessageBox(NULL, "�f�o�b�O�e�L�X�g�쐬���s", "error", MB_OK);
	}
#endif //#if _DEBUG.

	m_vpSprite.resize(0);
}

BaseScene::~BaseScene()
{
#if _DEBUG

	//�f�o�b�O�e�L�X�g�̍폜.
	SAFE_DELETE(m_pDebugText);

#endif //#if _DEBUG.
}

//�쐬.
void BaseScene::Create(const enSwitchToNextScene enNextScene)
{
	//�e�V�[���̍쐬.
	CreateProduct(enNextScene);
}

//�X�V.
void BaseScene::Update(enSwitchToNextScene &enNextScene)
{
	//�e�V�[���̍X�V.
	UpdateProduct(enNextScene);

	/*====/ �f�o�b�O�p /====*/
#if _DEBUG

	//���͏��̍X�V.
	InputUpdate(enNextScene);

#endif //#if _DEBUG.
}

//3D���f���̕`��.
void BaseScene::RenderModel()
{
	//�e�V�[����3D���f���̕`��.
	RenderModelProduct();
}

//2D�X�v���C�g�̕`��.
void BaseScene::RenderSprite()
{
	//�e�V�[����2D�X�v���C�g�̕`��.
	RenderSpriteProduct();

#if _DEBUG

	//�f�o�b�O�e�L�X�g�̕\��.
	RenderDebugText();

#endif //#if _DEBUG.
}

//�X�v���C�g���m�̐ڐG.
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

/*====/ �f�o�b�O�p /====*/
#if _DEBUG

//���͏��̍X�V.
void BaseScene::InputUpdate(enSwitchToNextScene &enNextScene)
{
	//�V�[���؂�ւ�.
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