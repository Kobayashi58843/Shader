#include "SoundManager.h"

#include "MyMacro.h"

#include <stdlib.h>

SoundManager::SoundManager()
{
	for (int i = 0; i < enBGM::enBGM_Max; i++)
	{
		m_pBGM[i] = nullptr;
	}

	for (int i = 0; i < enSE::enSE_Max; i++)
	{
		for (int j = 0; j < g_iDuplicableMax; j++)
		{
			m_pSE[i][j] = nullptr;
		}
	}
}

SoundManager::~SoundManager()
{
	for (int i = 0; i < enBGM::enBGM_Max; i++)
	{
		m_pBGM[i]->Close();
		SAFE_DELETE(m_pBGM[i]);
	}

	//���̎�ނ̐�.
	for (int i = 0; i < enSE::enSE_Max; i++)
	{
		//�����d�˂鐔.
		for (int j = 0; j < g_iDuplicableMax; j++)
		{
			m_pSE[i][j]->Close();
			SAFE_DELETE(m_pSE[i][j]);
		}
	}
}

//�T�E���h�����[�h����.
void SoundManager::LoadSound(const HWND hWnd)
{
	//BGM.
	LoadBGM(hWnd);

	//SE.
	LoadSE(hWnd);
}

//�S�T�E���h���~����.
void SoundManager::StopSound()
{
	for (int i = 0; i < enBGM::enBGM_Max; i++)
	{
		m_pBGM[i]->Stop();
	}

	for (int i = 0; i < enSE::enSE_Max; i++)
	{
		for (int j = 0; j < g_iDuplicableMax; j++)
		{
			m_pSE[i][j]->Stop();
		}
	}
}

//SE���͂��߂���Đ�.
void SoundManager::PlaySE(const enSE Index, const int iVolume)
{
	for (int i = 0; i < g_iDuplicableMax; i++)
	{
		//��~���̉��̂ݍĐ�����.
		if (m_pSE[Index][i]->IsStopped())
		{
			m_pSE[Index][i]->SeekToStart();
			m_pSE[Index][i]->Play();
			m_pSE[Index][i]->SetVolume(iVolume);
			return;
		}
	}
}

//SE���~����.
void SoundManager::StopSE(const enSE Index)
{
	for (int i = 0; i < g_iDuplicableMax; i++)
	{
		m_pSE[Index][i]->Stop();
	}
}

//SE��1�ڂ��Đ�.
void SoundManager::PlayFirstSE(const enSE Index)
{
	m_pSE[Index][0]->SeekToStart();
	m_pSE[Index][0]->Play();
}

//SE��1�ڂ���~�����m�F����.
bool SoundManager::IsStoppedFirstSE(const enSE Index)
{
	if (m_pSE[Index][0]->IsStopped())
	{
		return true;
	}
	return false;
}

//BGM�����[�h����.
void SoundManager::LoadBGM(const HWND hWnd)
{
	for (int i = 0; i < enBGM::enBGM_Max; i++)
	{
		SoundData sData;

		switch (i)
		{
		case enBGM_Title:
			sData = { "Data\\Sound\\BGM\\Title.mp3", "Title" };

			break;
		case enBGM_Action:
			sData = { "Data\\Sound\\BGM\\Action.mp3", "Action" };

			break;
		case enBGM_Over:
			sData = { "Data\\Sound\\BGM\\Over.mp3", "Over" };

			break;
		case enBGM_Clear:
			sData = { "Data\\Sound\\BGM\\Clear.mp3", "Clear" };

			break;
		default:
			break;
		}

		m_pBGM[i] = new Sound;
		//�����t�@�C�����J��.
		m_pBGM[i]->Open(sData.sPath, sData.sAlias, hWnd);
	}
}

//SE�����[�h����.
void SoundManager::LoadSE(const HWND hWnd)
{
	for (int i = 0; i < enSE::enSE_Max; i++)
	{
		SoundData sData;

		for (int j = 0; j < g_iDuplicableMax; j++)
		{
			switch (i)
			{
			case enSE_Cursor:
				sData = { "Data\\Sound\\SE\\System\\Cursor.mp3", "Cursor" };
				
				break;
			case enSE_PushButton:
				sData = { "Data\\Sound\\SE\\System\\PushButton.mp3", "PushButton" };

				break;
			case enSE_PlayerDamage:
				sData = { "Data\\Sound\\SE\\Player\\PlayerDamage.mp3", "PlayerDamage" };

				break;
			case enSE_PlayerDamageVoice:
				sData = { "Data\\Sound\\SE\\Player\\Voice\\PlayerDamageVoice.mp3", "PlayerDamageVoice" };

				break;
			case enSE_PlayerDead:
				sData = { "Data\\Sound\\SE\\Player\\PlayerDead.mp3", "PlayerDead" };

				break;
			case enSE_PlayerDeadVoice:
				sData = { "Data\\Sound\\SE\\Player\\Voice\\PlayerDeadVoice.mp3", "PlayerDeadVoice" };

				break;
			case enSE_PlayerAttack:
				sData = { "Data\\Sound\\SE\\Player\\PlayerAttack.mp3", "PlayerAttack" };

				break;
			case enSE_PlayerAttackVoice:
				sData = { "Data\\Sound\\SE\\Player\\Voice\\PlayerAttackVoice.mp3", "PlayerAttackVoice" };

				break;
			case enSE_PlayerMoveLimit:
				sData = { "Data\\Sound\\SE\\Player\\PlayerMoveLimit.mp3", "PlayerMoveLimit" };

				break;
			case enSE_EnemyDamage:
				sData = { "Data\\Sound\\SE\\Enemy\\EnemyDamage.mp3", "EnemyDamage" };

				break;
			case enSE_EnemyDamageVoice:
				sData = { "Data\\Sound\\SE\\Enemy\\Voice\\EnemyDamageVoice.mp3", "EnemyDamageVoice" };

				break;
			case enSE_EnemyDead:
				sData = { "Data\\Sound\\SE\\Enemy\\EnemyDead.mp3", "EnemyDead" };

				break;
			case enSE_EnemyDeadVoice:
				sData = { "Data\\Sound\\SE\\Enemy\\Voice\\EnemyDeadVoice.mp3", "EnemyDeadVoice" };

				break;
			case enSE_EnemyAttack:
				sData = { "Data\\Sound\\SE\\Enemy\\EnemyAttack.mp3", "EnemyAttack" };

				break;
			case enSE_EnemyAttackVoice:
				sData = { "Data\\Sound\\SE\\Enemy\\Voice\\EnemyAttackVoice.mp3", "EnemyAttackVoice" };

				break;
			case enSE_EnemyGuard:
				sData = { "Data\\Sound\\SE\\Enemy\\EnemyGuard.mp3", "EnemyGuard" };

				break;
			case enSE_Explosion:
				sData = { "Data\\Sound\\SE\\Player\\Explosion.mp3", "Explosion" };

				break;
			default:
				break;
			}

			char cNo[8];
			_itoa_s(j, cNo, 10);
			strcat_s(sData.sAlias, cNo);

			m_pSE[i][j] = new Sound;
			//�����t�@�C�����J��.
			m_pSE[i][j]->Open(sData.sPath, sData.sAlias, hWnd);
		}
	}
}