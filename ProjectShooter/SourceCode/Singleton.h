#pragma once
#pragma warning( disable:4005 )	

#include "Global.h"

#include <mutex>

//�쐬���ꂽ���Ԃ̋t���Ŕj������N���X.
class SingletonFinalizer
{
public:
	//�j���p�֐���o�^���邽�߂̃^�C�v�f�t.
	typedef void(*FinalizerFunc)();

	//�V���O���g���ō쐬���ꂽ�N���X�̔j���p�֐���o�^.
	static void AddFinalizer(FinalizerFunc Func);

	//�o�^���ꂽ�N���X��j��.
	static void Finalize();
};

template <typename T>
//�p���o���Ȃ��悤�ɂ��Ă���.
class Singleton final
{
public:
	static T& GetInstance()
	{
		//1�x�����uCreate�v�֐����Ăяo��.
		std::call_once(m_InitFlag, Create);
		assert(m_pInstance);
		return *m_pInstance;
	}

private:
	//�N���X���쐬���A�j���p�̊֐���o�^.
	static void Create()
	{
		//�N���X���쐬.
		m_pInstance = new T;

		//�j���p�̊֐���o�^.
		SingletonFinalizer::AddFinalizer(&Singleton<T>::Destroy);
	}

	//�j���p�̊֐���o�^.
	static void Destroy()
	{
		SAFE_DELETE(m_pInstance);
	}

	//coll_once()�֐��̂��߂̃t���O.
	static std::once_flag m_InitFlag;
	//�C���X�^���X.
	static T* m_pInstance;
};

template <typename T> std::once_flag Singleton<T>::m_InitFlag;
template <typename T> T* Singleton<T>::m_pInstance = nullptr;