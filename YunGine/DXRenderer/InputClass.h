#pragma once
#include <dinput.h>
#include <string>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool IsEscapePressed();
	void GetMouseLocation(int& mouseX, int& mouseY);
	int GetMouseX();
	int GetMouseY();

	bool ReadKeyBoard();
	bool ReadMouse();
	void ProcessInput();

	bool Initialize(HINSTANCE, HWND, int, int);
	void ShutDown();
	bool Frame();

	// Ű����
	bool GetKeyDown(int keyCode);
	bool GetKeyUp(int keyCode);
	bool GetKey(int keyCode);

	// ���콺
	bool GetMouseDown(int mouseCode);
	bool GetMouseUp(int mouseCode);
	bool GetMouse(int mouseCode);

	std::string GetLastPressedkey();

	DIMOUSESTATE m_currMouseState;
	DIMOUSESTATE m_prevMouseState;

private:

	IDirectInput8* m_directInput;				// ���̷�Ʈ ��ǲ
	IDirectInputDevice8* m_keyboard;			// Ű���� ��ġ
	IDirectInputDevice8* m_mouse;				// ���콺 ��ġ

	unsigned char m_curKeyboardState[256];
	unsigned char m_prevKeyboardState[256];
	DIMOUSESTATE m_mouseState;

	std::string m_strLastKey;
	char m_lastKey;
	int virtualKeyCode;

	int m_screenWidth = 0;
	int m_screenHeight = 0;

	int m_mouseX = 0;
	int m_mouseY = 0;
};