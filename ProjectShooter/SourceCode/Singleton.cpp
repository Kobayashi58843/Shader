#include "Singleton.h"

//�j���p�֐���o�^�ł���ő�.
const  int g_iMaxFinalizersSize = 256;

//���݂̔j���p�֐��̔ԍ�.
static int g_iNumFinalizersSize = 0;

//�o�^����Ă���j���p�֐�.
static SingletonFinalizer::FinalizerFunc g_Finalizers[g_iMaxFinalizersSize];

//�V���O���g���ō쐬���ꂽ�N���X�̔j���p�֐���o�^.
void SingletonFinalizer::AddFinalizer(FinalizerFunc Func)
{
	assert(g_iNumFinalizersSize < g_iMaxFinalizersSize);
	g_Finalizers[g_iNumFinalizersSize++] = Func;
}

//�o�^���ꂽ�N���X��j��.
void SingletonFinalizer::Finalize()
{
	for (int i = g_iNumFinalizersSize - 1; i >= 0; --i)
	{
		(*g_Finalizers[i])();
	}
	g_iNumFinalizersSize = 0;
}