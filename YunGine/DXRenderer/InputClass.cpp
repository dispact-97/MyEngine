#include "InputClass.h"

InputClass::InputClass()
	:m_directInput(nullptr),
	m_keyboard(nullptr),
	m_mouse(nullptr),
	m_keyboardState(),
	m_mouseState(),
	lastKey(),
	lastPressedKey()
{

}

InputClass::InputClass(const InputClass&)
	:m_directInput(nullptr),
	m_keyboard(nullptr),
	m_mouse(nullptr),
	m_keyboardState(),
	m_mouseState(),
	lastKey(),
	lastPressedKey()
{

}

InputClass::~InputClass()
{

}

bool InputClass::IsEscapePressed()
{
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	m_mouseX = mouseX;
	m_mouseY = mouseY;
	return;
}

bool InputClass::ReadKeyBoard()
{
	HRESULT hr;

	hr = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	for (int i = 0; i < 256; i++)
	{
		if (m_keyboardState[i] & 0x80)
		{
			lastKey = static_cast<char>(i);
		}
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT hr;
	hr = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}

void InputClass::ProcessInput()
{
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	return;
}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT hr;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	// 키보드
	hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_keyboard->Acquire();
	if (FAILED(hr))
	{
		return false;
	}

	//마우스
	hr = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_mouse->Acquire();
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void InputClass::ShutDown()
{
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}

bool InputClass::Frame()
{
	bool result;

	result = ReadKeyBoard();
	if (!result)
	{
		return false;
	}

	ProcessInput();

	return true;

}

std::string InputClass::GetLastPressedKey() const
{
	if (lastKey < 1 || lastKey >26)
	{
		//lastPressedKey = lastKey;
		return lastPressedKey;
	}

	return lastPressedKey;
}
