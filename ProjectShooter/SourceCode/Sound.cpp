#include "Sound.h"
#include <stdio.h>

//�R���X�g���N�^.
Sound::Sound()
{
	m_hWnd = nullptr;

	for (unsigned int i = 0; i < STR_BUFF_MAX; i++)
	{
		m_sAlias[i] = NULL;
	}
}

//�f�X�g���N�^.
Sound::~Sound()
{
	//Close();

	for (unsigned int i = 0; i < STR_BUFF_MAX; i++)
	{
		m_sAlias[i] = NULL;
	}

	m_hWnd = nullptr;
}

//�����t�@�C�����J��.
bool Sound::Open(LPCTSTR sFName, char *sAlias, HWND hWnd)
{
	//�����ݒ�.
	SetInitParam( sAlias, hWnd );

	//�R�}���h.
	char command[STR_BUFF_MAX] = "open ";

	strcat_s( command, sizeof( command ), sFName );//�t�@�C����������.
	strcat_s( command, sizeof( command ), " alias " );
	strcat_s( command, sizeof( command ), m_sAlias );//�G�C���A�X������.

	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 )
	{
		return true;
	}
	return false;
}

//�����t�@�C�������.
bool Sound::Close()
{
	//�R�}���h.
	char command[STR_BUFF_MAX] = "close ";

	strcat_s( command, sizeof( command ), m_sAlias );//�G�C���A�X������.

	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 )
	{
		return true;
	}
	return false;
}

//�Đ��֐�.
bool Sound::Play(bool bNotify)
{
	//�R�}���h.
	char command[STR_BUFF_MAX] = "play ";

	strcat_s( command, sizeof( command ), m_sAlias );//�G�C���A�X������.
	if( bNotify )
	{
		strcat_s( command, sizeof( command ), " notify" );
		//notify:�������I�������Ƃ���hWnd��MM_MCINOTIFY���b�Z�[�W���|�X�g����.
	}

	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//��~�֐�.
bool Sound::Stop()
{
	//�R�}���h.
	char command[STR_BUFF_MAX] = "stop ";

	strcat_s( command, sizeof( command ), m_sAlias );//�G�C���A�X������.

	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//��Ԃ��擾����֐�.
//	sStatus �̔z�񐔂� 256�ȉ��ɂ��邱��.
//���������A��Ԃ��擾����ꍇ�́A�Đ����Ɂunotify�v��ݒ肵�A
//  �E�B���h�E�n���h���Ƀ��b�Z�[�W�𑗂��Ă����K�v������.
bool Sound::GetStatus(char *sStatus)
{
	//�R�}���h.
	char command[STR_BUFF_MAX] = "status ";

	strcat_s( command, sizeof( command ), m_sAlias );//�G�C���A�X������.
	strcat_s( command, sizeof( command ), " mode" );

	if( mciSendString( command, sStatus, STR_BUFF_MAX, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//���̒�~���m�F����֐�.
bool Sound::IsStopped()
{
	char sStatus[STR_BUFF_MAX] = "";

	//��Ԃ̎擾.
	if( GetStatus( sStatus ) )
	{
		//�������r.
		if( strcmp( sStatus, "stopped" ) == 0 )
		{
			return true;
		}
	}
	return false;
}

//���̍Đ������m�F����֐�.
bool Sound::IsPlaying()
{
	char sStatus[STR_BUFF_MAX] = "";

	//��Ԃ̎擾.
	if( GetStatus( sStatus ) )
	{
		//�������r.
		if( strcmp( sStatus, "playing" ) == 0 )
		{
			return true;
		}
	}
	return false;
}

//�Đ��ʒu�����߂ɖ߂��֐�.
bool Sound::SeekToStart()
{
	//�R�}���h.
	char command[STR_BUFF_MAX] = "seek ";

	strcat_s( command, sizeof( command ), m_sAlias );//�G�C���A�X������.
	strcat_s( command, sizeof( command ), " to start" );

	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//���ʂ�ݒ肷��֐�.
bool Sound::SetVolume(int iVolume)
{
	//�͈͓��Ɋۂ߂�.
	if( iVolume < 0 )		{ iVolume = 0;		}
	if( iVolume > 1000 )	{ iVolume = 1000;	}

	//���ʂ𕶎���ɕϊ�.
	char sVolume[STR_BUFF_MAX] = "";
	sprintf_s( sVolume, sizeof( sVolume ), "%d", iVolume );


	//�R�}���h.
	char command[STR_BUFF_MAX] = "setaudio ";

	strcat_s( command, sizeof( command ), m_sAlias );//�G�C���A�X������.
	strcat_s( command, sizeof( command ), " volume to " );
	strcat_s( command, sizeof( command ), sVolume );//���ʂ�����.

	if( mciSendString( command, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//�����ݒ�.
void Sound::SetInitParam(char *sAlias, HWND hWnd)
{
	//�E�B���h�E�n���h����n��.
	m_hWnd = hWnd;

	//�G�C���A�X��n��.
	strcpy_s( m_sAlias, sizeof( m_sAlias ), sAlias );
}
