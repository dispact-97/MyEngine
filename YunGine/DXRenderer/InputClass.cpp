#include "InputClass.h"

InputClass::InputClass()
	:m_directInput(nullptr),
	m_keyboard(nullptr),
	m_mouse(nullptr),
	m_curKeyboardState(),
	m_prevKeyboardState(),
	m_mouseState(),
	m_lastKey(),
	virtualKeyCode()
{

}

InputClass::InputClass(const InputClass&)
	:m_directInput(nullptr),
	m_keyboard(nullptr),
	m_mouse(nullptr),
	m_curKeyboardState(),
	m_prevKeyboardState(),
	m_mouseState(),
	m_lastKey(),
	virtualKeyCode()
{

}

InputClass::~InputClass()
{

}

bool InputClass::IsEscapePressed()
{
	if (m_curKeyboardState[DIK_ESCAPE] & 0x80)
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

int InputClass::GetMouseX()
{
	return m_mouseX;
}

int InputClass::GetMouseY()
{
	return m_mouseY;
}

bool InputClass::ReadKeyBoard()
{
	for (int i = 0; i < 256; i++)
	{
		m_prevKeyboardState[i] = m_curKeyboardState[i];
	}

	HRESULT hr;

	hr = m_keyboard->GetDeviceState(sizeof(m_curKeyboardState), (LPVOID)&m_curKeyboardState);
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
 		if (m_curKeyboardState[i] & 0x80)	
 		// keyboardState배열안에 키마다 고유의 번호를 가지고있음
 		{
 			virtualKeyCode = i;
 		}
 	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT hr;

	m_prevMouseState = m_currMouseState;

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
	else
	{
		m_mouseX = m_mouseState.lX;
		m_mouseY = m_mouseState.lY;
	}

	return true;
}

void InputClass::ProcessInput()
{
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth)
	{
		m_mouseX = m_screenWidth; 
	}

	if (m_mouseY > m_screenHeight)
	{
		m_mouseY = m_screenHeight; 
	}

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

	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	//ProcessInput();

	return true;

}

bool InputClass::GetKeyDown(int keyCode)
{
	return m_curKeyboardState[keyCode] && !m_prevKeyboardState[keyCode];
}

bool InputClass::GetKeyUp(int keyCode)
{
	return !m_curKeyboardState[keyCode] && m_prevKeyboardState[keyCode];
}

bool InputClass::GetKey(int keyCode)
{
	return m_curKeyboardState[keyCode] && m_prevKeyboardState[keyCode];
}

bool InputClass::GetMouseDown(int mouseCode)
{
	return m_currMouseState.rgbButtons[mouseCode] && !m_prevMouseState.rgbButtons[mouseCode];
}

bool InputClass::GetMouseUp(int mouseCode)
{
	return m_currMouseState.rgbButtons[mouseCode] && !m_prevMouseState.rgbButtons[mouseCode];
}

bool InputClass::GetMouse(int mouseCode)
{
	return m_currMouseState.rgbButtons[mouseCode] && !m_prevMouseState.rgbButtons[mouseCode];
}

std::string InputClass::GetLastPressedkey()
{
	switch (virtualKeyCode)
	{
		case 30:
			m_strLastKey = L'A';
			return m_strLastKey;
		case 31:
			m_strLastKey = L'S';
			return m_strLastKey;
		case 32:
			m_strLastKey = L'D';
			return m_strLastKey;
		case 17:
			m_strLastKey = L'W';
			return m_strLastKey;
		case 46:
			m_strLastKey = L'C';
			return m_strLastKey;
		case 57:
			m_strLastKey = "SPACE";
			return m_strLastKey;
		default:
			return m_strLastKey;
	}

	return m_strLastKey;
}

