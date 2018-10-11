#pragma once
#pragma warning( disable:4005 )

#include "Global.h"

class RawInput
{
public:
	static RawInput* GetInstance()
	{
		static RawInput s_Instance;
		return &s_Instance;
	}
	~RawInput();

	//RawInput������.
	void InitRawInput(const HWND hWnd);

	//���b�Z�[�W���̏���.
	void MsgProc(const UINT uMsg, const LPARAM lParam);

	void ClearPerFrame()
	{
		CrearVECTOR2(m_vMouseMovingDistance);

		m_bLButtonDown = false;
		m_bLButtonUp = false;
		m_bRButtonDown = false;
		m_bRButtonUp = false;

		m_bWheelForward = false;
		m_bWheelBackward = false;
	}

	//�}�E�X�̈ړ���.
	D3DXVECTOR2 GetMouseMovingDistance() const
	{
		return m_vMouseMovingDistance;
	}

	//���N���b�N���ꂽ�u��.
	bool IsLButtonDown() const
	{
		return m_bLButtonDown;
	}

	//���N���b�N�������ꂽ�u��.
	bool IsLButtonUp() const
	{
		return m_bLButtonUp;
	}

	//���N���b�N����Ă��邩.
	bool IsLButtonHoldDown() const
	{
		return m_bLButtonHoldDown;
	}

	//�E�N���b�N���ꂽ�u��.
	bool IsRButtonDown() const
	{
		return m_bRButtonDown;
	}

	//�E�N���b�N�������ꂽ�u��.
	bool IsRButtonUp() const
	{
		return m_bRButtonUp;
	}

	//�E�N���b�N����Ă��邩.
	bool IsRButtonHoldDown() const
	{
		return m_bRButtonHoldDown;
	}

	bool IsWheelForward() const
	{
		return m_bWheelForward;
	}

	bool IsWheelBackward() const
	{
		return m_bWheelBackward;
	}


private:
	RawInput();
	RawInput(const RawInput& rhs);
	RawInput& operator = (const RawInput& rhs);

	//�}�E�X�̈ړ���.
	D3DXVECTOR2 m_vMouseMovingDistance;

	//���N���b�N.
	bool m_bLButtonDown;
	bool m_bLButtonUp;
	bool m_bLButtonHoldDown;

	//�E�N���b�N.
	bool m_bRButtonDown;
	bool m_bRButtonUp;
	bool m_bRButtonHoldDown;

	//�z�C�[���̒[��.
	int m_iWheelFraction;
	bool m_bWheelForward;
	bool m_bWheelBackward;

	//���݂̉�����Ă���{�^���̊m�F.
	void ConfirmationPushedButton(RAWINPUT* RawInput);
};