#pragma once
#pragma warning( disable:4005 )

#include "Sound.h"

//�d�����čĐ��ł���ő�.
const int g_iDuplicableMax = 4;

//BGM�̉���.
const int b_iBGMVolume = 600;

class SoundManager
{
public:
	enum enBGM
	{
		enBGM_Title = 0,
		enBGM_Action,
		enBGM_Over,
		enBGM_Clear,

		enBGM_Max,
	};

	enum enSE
	{
		//�J�[�\���ƃ{�^���I������SE.
		enSE_Cursor = 0,
		enSE_PushButton,

		//�v���C���[�p��SE.
		enSE_PlayerDamage,
		enSE_PlayerDamageVoice,
		enSE_PlayerDead,
		enSE_PlayerDeadVoice,
		enSE_PlayerAttack,
		enSE_PlayerAttackVoice,
		enSE_PlayerMoveLimit,

		//�G�l�~�[�p��SE.
		enSE_EnemyDamage,
		enSE_EnemyDamageVoice,
		enSE_EnemyDead,
		enSE_EnemyDeadVoice,
		enSE_EnemyAttack,
		enSE_EnemyAttackVoice,
		enSE_EnemyGuard,

		enSE_Explosion,

		enSE_Max,
	};

	SoundManager();
	~SoundManager();

	//�T�E���h�����[�h����.
	void LoadSound(const HWND hWnd);

	//BGM���͂��߂���Đ�.
	void PlayBGM(const enBGM Index)
	{
		if (m_pBGM[Index]->IsStopped())
		{
			m_pBGM[Index]->SeekToStart();
			m_pBGM[Index]->Play();
			m_pBGM[Index]->SetVolume(b_iBGMVolume);
		}
	}

	//BGM���~����.
	void StopBGM(const enBGM Index)
	{
		m_pBGM[Index]->Stop();
	}

	//SE���͂��߂���Đ�.
	void PlaySE(const enSE Index, const int iVolume = 1000);

	//SE���~����.
	void StopSE(const enSE Index);

	//SE��1�ڂ��Đ�.
	void PlayFirstSE(const enSE Index);

	//SE��1�ڂ���~�����m�F����.
	bool IsStoppedFirstSE(const enSE Index);

	//�S�T�E���h���~����.
	void StopSound();

private:
	Sound* m_pBGM[enBGM_Max];
	Sound* m_pSE[enSE_Max][g_iDuplicableMax];

	//BGM�����[�h����.
	void LoadBGM(const HWND hWnd);

	//SE�����[�h����.
	void LoadSE(const HWND hWnd);
};