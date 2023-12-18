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

	IDirectInput8* m_directInput;				// 다이렉트 인풋
	IDirectInputDevice8* m_keyboard;					// 키보드 장치
	IDirectInputDevice8* m_mouse;					// 마우스 장치

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	char lastKey;
	std::string lastPressedKey;

	int m_screenWidth = 0;
	int m_screenHeight = 0;
	int m_mouseX = 0;
	int m_mouseY = 0;

};