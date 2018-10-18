#include "Singleton.h"

//”jŠü—pŠÖ”‚ğ“o˜^‚Å‚«‚éÅ‘å.
const  int g_iMaxFinalizersSize = 256;

//Œ»İ‚Ì”jŠü—pŠÖ”‚Ì”Ô†.
static int g_iNumFinalizersSize = 0;

//“o˜^‚³‚ê‚Ä‚¢‚é”jŠü—pŠÖ”.
static SingletonFinalizer::FinalizerFunc g_Finalizers[g_iMaxFinalizersSize];

//ƒVƒ“ƒOƒ‹ƒgƒ“‚Åì¬‚³‚ê‚½ƒNƒ‰ƒX‚Ì”jŠü—pŠÖ”‚ğ“o˜^.
void SingletonFinalizer::AddFinalizer(FinalizerFunc Func)
{
	assert(g_iNumFinalizersSize < g_iMaxFinalizersSize);
	g_Finalizers[g_iNumFinalizersSize++] = Func;
}

//“o˜^‚³‚ê‚½ƒNƒ‰ƒX‚ğ”jŠü.
void SingletonFinalizer::Finalize()
{
	for (int i = g_iNumFinalizersSize - 1; i >= 0; --i)
	{
		(*g_Finalizers[i])();
	}
	g_iNumFinalizersSize = 0;
}