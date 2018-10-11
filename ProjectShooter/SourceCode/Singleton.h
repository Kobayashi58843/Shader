#pragma once
#pragma warning( disable:4005 )	

#include "Global.h"

#include <mutex>

//作成された順番の逆順で破棄するクラス.
class SingletonFinalizer
{
public:
	//破棄用関数を登録するためのタイプデフ.
	typedef void(*FinalizerFunc)();

	//シングルトンで作成されたクラスの破棄用関数を登録.
	static void AddFinalizer(FinalizerFunc Func);

	//登録されたクラスを破棄.
	static void Finalize();
};

template <typename T>
//継承出来ないようにしておく.
class Singleton final
{
public:
	static T* GetInstance()
	{
		//1度だけ「Create」関数を呼び出す.
		std::call_once(InitFlag, Create);
		assert(pInstance);
		return pInstance;
	}

private:
	//クラスを作成し、破棄用の関数を登録.
	static void Create()
	{
		//クラスを作成.
		pInstance = new T;

		//破棄用の関数を登録.
		SingletonFinalizer::AddFinalizer(&Singleton<T>::Destroy);
	}

	//破棄用の関数を登録.
	static void Destroy()
	{
		SAFE_DELETE(pInstance);
	}

	//coll_once()関数のためのフラグ.
	static std::once_flag InitFlag;
	//インスタンス.
	static T* pInstance;
};

//シングルトンクラスの変数の定義.
template <typename T> std::once_flag Singleton<T>::InitFlag;
template <typename T> T* Singleton<T>::pInstance = nullptr;