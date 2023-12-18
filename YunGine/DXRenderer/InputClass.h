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

	bool ReadKeyBoard();
	bool ReadMouse();
	void ProcessInput();

	bool Initialize(HINSTANCE, HWND, int, int);
	void ShutDown();
	bool Frame();

	std::string GetLastPressedKey() const;

private:

	IDirectInput8* m_directInput;				// ���̷�Ʈ ��ǲ
	IDirectInputDevice8* m_keyboard;					// Ű���� ��ġ
	IDirectInputDevice8* m_mouse;					// ���콺 ��ġ

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	char lastKey;
	std::string lastPressedKey;

	int m_screenWidth = 0;
	int m_screenHeight = 0;
	int m_mouseX = 0;
	int m_mouseY = 0;

};