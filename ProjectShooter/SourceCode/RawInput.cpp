#include "RawInput.h"

RawInput::RawInput()
{
	CrearVECTOR2(m_vMouseMovingDistance);
	m_bLButtonDown = false;
	m_bLButtonUp = false;
	m_bLButtonHoldDown = false;

	m_bRButtonDown = false;
	m_bRButtonUp = false;
	m_bRButtonHoldDown = false;

	m_iWheelFraction = 0;
	m_bWheelForward = false;
	m_bWheelBackward = false;
}

RawInput::~RawInput()
{
}

//RawInput������.
void RawInput::InitRawInput(const HWND hWnd)
{
	//WM_INPUT�Ŏ󂯎���悤�ɓo�^.
	RAWINPUTDEVICE device[1];
	device[0].usUsagePage = 0x01;
	device[0].usUsage = 0x02;
	device[0].dwFlags = RIDEV_INPUTSINK;
	device[0].hwndTarget = hWnd;

	if (RegisterRawInputDevices(device, 1, sizeof(device[0])) == 0)
	{
		ERR_MSG("Main::InitRawInput() : �f�o�C�X�̓o�^���s", "error");
	}
}

//���b�Z�[�W���̏���.
void RawInput::MsgProc(const UINT uMsg, const LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ACTIVATE:
		m_iWheelFraction = NULL;

		break;
	case WM_INPUT:
	{
		UINT dwSize = 40;
		BYTE lpb[40];

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

		RAWINPUT* RawInput = (RAWINPUT*)lpb;

		if (RawInput->header.dwType == RIM_TYPEMOUSE)
		{
			//�}�E�X�̈ړ���.
			m_vMouseMovingDistance.x = (float)RawInput->data.mouse.lLastX;
			m_vMouseMovingDistance.y = (float)RawInput->data.mouse.lLastY;

			//���݂̉�����Ă���{�^���̊m�F.
			ConfirmationPushedButton(RawInput);
		}
	}
		break;
	}
}

//���݂̉�����Ă���{�^���̊m�F.
void RawInput::ConfirmationPushedButton(RAWINPUT* RawInput)
{
	assert(RawInput != nullptr);
	USHORT usButtonFlags = RawInput->data.mouse.usButtonFlags;

	switch (usButtonFlags)
	{
	case RI_MOUSE_LEFT_BUTTON_DOWN:
		m_bLButtonDown = true;
		m_bLButtonHoldDown = true;

		break;
	case RI_MOUSE_LEFT_BUTTON_UP:
		m_bLButtonUp = true;
		m_bLButtonHoldDown = false;

		break;
	case RI_MOUSE_RIGHT_BUTTON_DOWN:
		m_bRButtonDown = true;
		m_bRButtonHoldDown = true;

		break;
	case RI_MOUSE_RIGHT_BUTTON_UP:
		m_bRButtonUp = true;
		m_bRButtonHoldDown = false;

		break;
	case RI_MOUSE_WHEEL:
		{
			//��]��.
			short sDelta = (int)RawInput->data.mouse.usButtonData;

			//�O��̒[����ǉ�.
			sDelta += m_iWheelFraction;

			//�m�b�`�������߂�.
			int iNotch = sDelta / WHEEL_DELTA;

			//�[����ۑ�����.
			m_iWheelFraction = sDelta % WHEEL_DELTA;

			if (iNotch > 0)//�O�ɉ�].
			{
				m_bWheelForward = true;
				m_bWheelBackward = false;
			}
			else if (iNotch < 0)//���ɉ�].
			{
				m_bWheelForward = false;
				m_bWheelBackward = true;
			}
		}

		break;
	default:

		break;
	}
}