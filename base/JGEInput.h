#ifndef __JGE_INPUT_H__
#define __JGE_INPUT_H__

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

class JGEInput
{
public:
	JGEInput();
	~JGEInput();

	bool initInput(HINSTANCE hInstance, HWND hWnd);
	bool initKeyboard(HINSTANCE hInstance, HWND hWnd);
	bool initMouse(HINSTANCE hInstance, HWND hWnd);
	bool updateInput();
	bool updateKeyboard();
	bool updateMouse();
	bool keyDown(int diKeyCode);
	bool mouseLockOnWindow();
	bool mouseUnlockOnWindow();
	HWND getHWnd() const;
	HINSTANCE getHInstance() const;
	int getMouseX() const;
	int getMouseY() const;
	bool getMouseLockedOnWindow() const;
	void setMouseSpeed(float speed);
	float getMouseSpeed() const;
	bool getMouseLeftButtonDown() const;
	bool getMouseRightButtonDown() const;
	bool getMouseMiddleButtonDown() const;

private:
	JGEInput(const JGEInput& value);

	HWND m_hWnd;
	HINSTANCE m_hInstance;
	IDirectInput8* m_lpInput;
	IDirectInputDevice8* m_lpKeyboard;
	IDirectInputDevice8* m_lpMouse;
	int m_mouseX;
	int m_mouseY;
	int m_mouseClientWidth;
	int m_mouseClientHeight;
	bool m_mouseLockedOnWindow;
	RECT m_mouseClipOriginal;
	float m_mouseSpeed;
	DIMOUSESTATE m_mouseState;
	bool m_mouseLeftButtonDown;
	bool m_mouseRightButtonDown;
	bool m_mouseMiddleButtonDown;
	unsigned char m_keyboardState[256];

	bool initInputDevice(HINSTANCE hInstance);
};

#endif