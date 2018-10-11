#include "ActionScene.h"
#include "Resource.h"

const float g_fCameraMoveSpeed = 0.2f;

ActionScene::ActionScene(SceneNeedPointer PointerGroup)
	: BaseScene(PointerGroup)
{
	m_pCamera = nullptr;
	m_pPlayer = nullptr;
	m_pEnemy = nullptr;
	m_pGround = nullptr;
	m_pBulletManager = nullptr;

	m_fOldCameraDistance = 0.0f;
}

ActionScene::~ActionScene()
{
	SAFE_DELETE(m_pCamera);

	SAFE_DELETE(m_pPlayer);

	SAFE_DELETE(m_pEnemy);

	m_pGround = nullptr;

	SAFE_DELETE(m_pBulletManager);

	for (unsigned int i = 0; i < m_vpSprite.size(); i++)
	{
		SAFE_DELETE(m_vpSprite[i]);
	}
	//�g���Ă���T�C�Y��0�ɂ���.
	m_vpSprite.clear();
	//�L���p�V�e�B�����݂̃T�C�Y�ɂ��킹��.
	m_vpSprite.shrink_to_fit();

	//�S�T�E���h���~����.
	SoundManager::GetInstance()->StopSound();
}

//�쐬.
void ActionScene::CreateProduct(const enSwitchToNextScene enNextScene)
{
	//�X�v���C�g�̍쐬.
	CreateSprite();

	m_pCamera = new Camera(g_fWindowWidth,g_fWindowHeight);

	m_pBulletManager = new BulletManager;

	m_pPlayer = new Player(Resource::GetInstance()->GetSkinModels(Resource::enSkinModel_Player), m_pBulletManager);

	m_pEnemy = new Enemy(Resource::GetInstance()->GetSkinModels(Resource::enSkinModel_Enemy));

	m_pGround = Resource::GetInstance()->GetStaticModels(Resource::enStaticModel_Ground);
}

//�X�V.
void ActionScene::UpdateProduct(enSwitchToNextScene &enNextScene)
{
#if _DEBUG

	//�f�o�b�O���݂̂̑���.
	DebugKeyControl();

#endif //#if _DEBUG.

	//BGM�����[�v�ōĐ�.
	SoundManager::GetInstance()->PlayBGM(SoundManager::enBGM_Action);

	//�X�v���C�g�X�V.
	UpdateSprite();

	//�L�����N�^�[�X�V.
	m_pPlayer->DecideTargetDirection(m_pCamera->GetWorldPos());
	m_pPlayer->Update();
	m_pPlayer->RayHitToMesh(m_pGround);

	m_pEnemy->DecideTargetDirection(m_pPlayer->GetPos());
	//�����Ƃ���������~�߂Ă܂�.
	//m_pEnemy->Update();
	m_pEnemy->RayHitToMesh(m_pGround);

	m_pPlayer->HitToSphere(m_pEnemy->GetCollisionSphere());

	//�J�����X�V.
	D3DXVECTOR3 vLookAt = m_pPlayer->GetPos();
	vLookAt.y += 1.5f;
	m_pCamera->SetLookAt(vLookAt);
	ControlCameraMove();
	m_pCamera->Update(m_pGround);

	m_pBulletManager->Update(m_pCamera->GetCameraPose(), m_pPlayer->GetPos(), m_pEnemy);
	m_pBulletManager->CollisionJudgmentBullet(m_pEnemy->GetCollisionSphere(), m_pGround);

	//�V�[���ύX.
	if (m_pEnemy->IsDeadFlg())
	{
		enNextScene = enSwitchToNextScene::Clear;
	}
	else if (m_pPlayer->IsDeadFlg())
	{
		enNextScene = enSwitchToNextScene::Over;
	}
}

//3D���f���̕`��.
void ActionScene::RenderModelProduct()
{
	D3DXMATRIX mView = m_pCamera->GetView();
	D3DXMATRIX mProj = m_pCamera->GetProjection();
	D3DXVECTOR3 vCamPos = m_pCamera->GetWorldPos();

	LIGHT Light;
	Light.fIntensity = 0.0f;
	D3DXMatrixIdentity(&Light.mRot);
	CrearVECTOR3(Light.vDir);
	CrearVECTOR3(Light.vPos);

	m_pPlayer->RenderModel(mView, mProj, Light.vPos, vCamPos);

	m_pEnemy->RenderModel(mView, mProj, Light.vPos, vCamPos);

	m_pGround->Render(mView, mProj, Light, vCamPos);

	m_pBulletManager->Render(mView, mProj, vCamPos);

	//�G�t�F�N�g�̕`��.
	clsEffects::GetInstance()->Render(mView, mProj, vCamPos);
}

//�J�����̑���.
void ActionScene::ControlCameraMove()
{
	D3DXVECTOR2 vMouseMovingDistance = RawInput::GetInstance()->GetMouseMovingDistance();

	//�}�E�X�̈ړ����x�Ŏ��_�ړ����鑬����ς���.
	float fCameraMoveSpeed = fabs(g_fCameraMoveSpeed * (vMouseMovingDistance.x + vMouseMovingDistance.y));
	m_pCamera->SetRotPerFrame(fCameraMoveSpeed);

	//�J�����̈ړ�.
	if (0 < vMouseMovingDistance.y)
	{
		m_pCamera->MoveUp();
	}
	else if (0 > vMouseMovingDistance.y)
	{
		m_pCamera->MoveDown();
	}

	if (0 < vMouseMovingDistance.x)
	{
		m_pCamera->MoveLeft();
	}
	else if (0 > vMouseMovingDistance.x)
	{
		m_pCamera->MoveRight();
	}

	/*====/ �J�����̋��� /====*/
	//�E�N���b�N�����݂̒����ʒu����J�����ʒu�܂ł̋������o��.
	const float fDistance = D3DXVec3Length(&m_pCamera->GetFocusingSpacePos());
	if (RawInput::GetInstance()->IsRButtonDown())
	{
		//���݂̋������o���Ă���.
		m_fOldCameraDistance = fDistance;
	}
	else if (RawInput::GetInstance()->IsRButtonUp())
	{
		//���̋����ɖ߂�.
		m_pCamera->SetOffsetZ(-(m_fOldCameraDistance - 1.0f));
	}
	
	//�E�N���b�N�����Ă���Ԃ̓J�����𒍎��ʒu�ɋ߂Â���.
	if (RawInput::GetInstance()->IsRButtonHoldDown())
	{
		if (fDistance > 1.0f)
		{
			m_pCamera->SetOffsetZ(fDistance - 1.0f);
		}
	}
	else
	{
		//�}�E�X�z�C�[����.
		if (RawInput::GetInstance()->IsWheelForward())
		{
			m_pCamera->SetOffsetZ(1.0f);
		}
		else if (RawInput::GetInstance()->IsWheelBackward())
		{
			m_pCamera->SetOffsetZ(-1.0f);
		}
	}
}

/*====/ �X�v���C�g�֘A /====*/
//2D�X�v���C�g�̕`��.
void ActionScene::RenderSpriteProduct()
{
	for (unsigned int i = 0; i < m_vpSprite.size(); i++)
	{
		m_vpSprite[i]->Render();
	}
}

//�X�v���C�g�̍쐬.
void ActionScene::CreateSprite()
{
	//�L���p�V�e�B���m�肳����.
	m_vpSprite.reserve(enSprite_Max);

	SpriteState SpriteData;

	//�e�X�v���C�g�̐ݒ�.
	for (int i = 0; i < enSprite_Max; i++)
	{
		switch (i)
		{
		case enSprite_Alignment:
			SpriteData = { "Data\\Image\\AimingIcon.png", { 1.0f, 1.0f } };

			break;
		case enSprite_PlayerHp:
			SpriteData = { "Data\\Image\\PlayerHpGage.png", { 2.0f, 1.0f } };

			break;
		case enSprite_PlayerHpFrame:
			SpriteData = { "Data\\Image\\HpFrame.png", { 1.0f, 1.0f } };

			break;
		case enSprite_EnemyHp:
			SpriteData = { "Data\\Image\\EnemyHpGage.png", { 2.0f, 1.0f } };

			break;
		case enSprite_EnemyHpFrame:
			SpriteData = { "Data\\Image\\HpFrame.png", { 1.0f, 1.0f } };

			break;

		default:
			ERR_MSG("Action::CreateSprite()", "error");

			break;
		}

		//�z�������₷.
		m_vpSprite.push_back(new Sprite(SpriteData.vDivisionQuantity.x, SpriteData.vDivisionQuantity.y));
		m_vpSprite[i]->Create(m_SceneNeedPointer.pDevice, m_SceneNeedPointer.pContext, SpriteData.sPath);
	}
}

//�X�v���C�g�̃t���[�����Ƃ̍X�V.
void ActionScene::UpdateSprite()
{
	for (int i = 0; i < enSprite_Max; i++)
	{
		UpdateSpritePositio(i);
		UpdateSpriteAnimation(i);
	}
}

//�X�v���C�g�̈ʒu.
void ActionScene::UpdateSpritePositio(int iSpriteNo)
{
	//�E�C���h�E�̒��S.
	float fWindowWidthCenter = g_fWindowWidth / 2.0f;
	float fWindowHeightCenter = g_fWindowHeight / 2.0f;

	//�X�v���C�g�ʒu.
	D3DXVECTOR2 vPosition;

	//Hp�Q�[�W�̑傫��.
	const float fGageScale = 0.2f;

	//�g�ƃQ�[�W�̊Ԋu.
	const float fGageSpacing = 25.0f;

	switch (iSpriteNo)
	{
	case enSprite_Alignment:
		vPosition.x = fWindowWidthCenter;
		vPosition.y = fWindowHeightCenter;

		break;
	case enSprite_PlayerHp:
		vPosition.x = (m_vpSprite[iSpriteNo]->GetSize().x / 2.0f) + fGageSpacing;
		vPosition.y = (m_vpSprite[iSpriteNo]->GetSize().y / 2.0f) + fGageSpacing;

		break;
	case enSprite_PlayerHpFrame:
		//�v���C���[��Hp�Q�[�W�̈ʒu�ɂ��킹��.
		vPosition.x = m_vpSprite[enSprite_PlayerHp]->GetPos().x;
		vPosition.y = m_vpSprite[enSprite_PlayerHp]->GetPos().y;

		break;
	case enSprite_EnemyHp:
		vPosition.x = g_fWindowWidth - ((m_vpSprite[iSpriteNo]->GetSize().x / 2.0f) + fGageSpacing);
		vPosition.y = (m_vpSprite[iSpriteNo]->GetSize().y / 2.0f) + fGageSpacing;

		break;
	case enSprite_EnemyHpFrame:
		//�G�l�~�[��Hp�Q�[�W�̈ʒu�ɂ��킹��.
		vPosition.x = m_vpSprite[enSprite_EnemyHp]->GetPos().x;
		vPosition.y = m_vpSprite[enSprite_EnemyHp]->GetPos().y;

		break;
	default:
		ERR_MSG("Action::UpdateSpritePositio()", "error");

		break;
	}

	m_vpSprite[iSpriteNo]->SetPos(vPosition.x, vPosition.y);
}

//�X�v���C�g�̃A�j���[�V����.
void ActionScene::UpdateSpriteAnimation(int iSpriteNo)
{
	const float fGageMoveSpeed = 4.0f;

	switch (iSpriteNo)
	{
	case enSprite_Alignment:
		break;
	case enSprite_PlayerHp:
		m_vpSprite[iSpriteNo]->SpriteAsGage(m_pPlayer->GetHpMax(), m_pPlayer->GetHp(), fGageMoveSpeed);

		break;
	case enSprite_PlayerHpFrame:
		break;
	case enSprite_EnemyHp:
		m_vpSprite[iSpriteNo]->SpriteAsGage(m_pEnemy->GetHpMax(), m_pEnemy->GetHp(), fGageMoveSpeed);

		break;
	case enSprite_EnemyHpFrame:
		break;
	default:
		ERR_MSG("Action::UpdateSpriteAnimation()", "error");

		break;
	}
}

#if _DEBUG

//�f�o�b�O�e�L�X�g�̕\��.
void ActionScene::RenderDebugText()
{
	char cStrDbgTxt[512];

	sprintf_s(cStrDbgTxt, "Scene : Action");
	m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 0));

	sprintf_s(cStrDbgTxt, "MouseMoveDis : X[%f] , Y[%f]",
		RawInput::GetInstance()->GetMouseMovingDistance().x, RawInput::GetInstance()->GetMouseMovingDistance().y);

	m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 1));

	if (RawInput::GetInstance()->IsLButtonDown())
	{
		sprintf_s(cStrDbgTxt, "IsLButtonDown : true");
		m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 2));
	}
	else
	{
		sprintf_s(cStrDbgTxt, "IsLButtonDown : false");
		m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 2));
	}

	if (RawInput::GetInstance()->IsLButtonHoldDown())
	{
		sprintf_s(cStrDbgTxt, "IsLButtonHoldDown : true");
		m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 3));
	}
	else
	{
		sprintf_s(cStrDbgTxt, "IsLButtonHoldDown : false");
		m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 3));
	}

	if (RawInput::GetInstance()->IsRButtonDown())
	{
		sprintf_s(cStrDbgTxt, "IsRButtonDown : true");
		m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 4));
	}
	else
	{
		sprintf_s(cStrDbgTxt, "IsRButtonDown : false");
		m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 4));
	}

	if (RawInput::GetInstance()->IsRButtonHoldDown())
	{
		sprintf_s(cStrDbgTxt, "IsRButtonHoldDown : true");
		m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 5));
	}
	else
	{
		sprintf_s(cStrDbgTxt, "IsRButtonHoldDown : false");
		m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 5));
	}

	if (RawInput::GetInstance()->IsWheelForward())
	{
		sprintf_s(cStrDbgTxt, "IsWheelForward : true");
		m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 6));
	}
	else
	{
		sprintf_s(cStrDbgTxt, "IsWheelForward : false");
		m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 6));
	}

	if (RawInput::GetInstance()->IsWheelBackward())
	{
		sprintf_s(cStrDbgTxt, "IsWheelBackward : true");
		m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 7));
	}
	else
	{
		sprintf_s(cStrDbgTxt, "IsWheelBackward : false");
		m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 7));
	}

	float fLength = D3DXVec3Length(&m_pPlayer->GetPos());
	sprintf_s(cStrDbgTxt, "PlayerLength : [%f]", fLength);
	m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 8));

	fLength = D3DXVec3Length(&m_pCamera->GetFocusingSpacePos());
	sprintf_s(cStrDbgTxt, "CameraLength : [%f]", fLength);
	m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 9));

	sprintf_s(cStrDbgTxt, "CameraOldLength : [%f]", m_fOldCameraDistance);
	m_pDebugText->Render(cStrDbgTxt, 0, 50 + (50 * 10));
}

//�f�o�b�O���݂̂̑���.
void ActionScene::DebugKeyControl()
{
}

#endif //#if _DEBUG.