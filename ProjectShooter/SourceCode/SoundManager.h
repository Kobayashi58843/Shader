#pragma once
#pragma warning( disable:4005 )

#include "Sound.h"

//重複して再生できる最大.
const int g_iDuplicableMax = 2;

//BGMの音量.
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
		//カーソルとボタン選択時のSE.
		enSE_Cursor = 0,
		enSE_PushButton,

		//プレイヤー用のSE.
		enSE_PlayerDamage,
		enSE_PlayerDamageVoice,
		enSE_PlayerDead,
		enSE_PlayerDeadVoice,
		enSE_PlayerAttack,
		enSE_PlayerAttackVoice,
		enSE_PlayerMoveLimit,

		//エネミー用のSE.
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

	//サウンドをロードする.
	void LoadSound(const HWND hWnd);

	//BGMをはじめから再生.
	void PlayBGM(const enBGM Index)
	{
		if (m_pBGM[Index]->IsStopped())
		{
			m_pBGM[Index]->SeekToStart();
			m_pBGM[Index]->Play();
			m_pBGM[Index]->SetVolume(b_iBGMVolume);
		}
	}

	//BGMを停止する.
	void StopBGM(const enBGM Index)
	{
		m_pBGM[Index]->Stop();
	}

	//SEをはじめから再生.
	void PlaySE(const enSE Index, const int iVolume = 1000);

	//SEを停止する.
	void StopSE(const enSE Index);

	//SEの1つ目を再生.
	void PlayFirstSE(const enSE Index);

	//SEの1つ目が停止中か確認する.
	bool IsStoppedFirstSE(const enSE Index);

	//全サウンドを停止する.
	void StopSound();

private:
	Sound* m_pBGM[enBGM_Max];
	Sound* m_pSE[enSE_Max][g_iDuplicableMax];

	//BGMをロードする.
	void LoadBGM(const HWND hWnd);

	//SEをロードする.
	void LoadSE(const HWND hWnd);
};