#include "Singleton.h"

//破棄用関数を登録できる最大.
const  int g_iMaxFinalizersSize = 256;

//現在の破棄用関数の番号.
static int g_iNumFinalizersSize = 0;

//登録されている破棄用関数.
static SingletonFinalizer::FinalizerFunc g_Finalizers[g_iMaxFinalizersSize];

//シングルトンで作成されたクラスの破棄用関数を登録.
void SingletonFinalizer::AddFinalizer(FinalizerFunc Func)
{
	assert(g_iNumFinalizersSize < g_iMaxFinalizersSize);
	g_Finalizers[g_iNumFinalizersSize++] = Func;
}

//登録されたクラスを破棄.
void SingletonFinalizer::Finalize()
{
	for (int i = g_iNumFinalizersSize - 1; i >= 0; --i)
	{
		(*g_Finalizers[i])();
	}
	g_iNumFinalizersSize = 0;
}