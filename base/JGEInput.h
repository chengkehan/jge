#ifndef __JGE_INPUT_H__
#define __JGE_INPUT_H__

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include "jgeUtil.h"

class JGEInput
{
JGE_SINGLETON_DECLARE(JGEInput)

public:
	bool initInput(HINSTANCE hInstance, HWND hWnd);
	bool initKeyboard(HINSTANCE hInstance, HWND hWnd);
	bool initMouse(HINSTANCE hInstance, HWND hWnd);
	bool initClientMouse(HWND hWnd);
	bool updateInput();
	bool updateKeyboard();
	bool updateMouse();
	bool updateClientMouse();
	bool keyDown(int diKeyCode);
	bool mouseLockOnWindow();
	bool mouseUnlockOnWindow();
	inline HWND getHWnd() const { return m_hWnd; }
	inline HINSTANCE getHInstance() const { return m_hInstance; }
	inline int getMouseX() const { return m_mouseX; }
	inline int getMouseY() const { return m_mouseY; }
	inline int getClientMouseX() const { return m_clientMouseX; }
	inline int getClientMouseY() const { return m_clientMouseY; }
	inline bool getMouseLockedOnWindow() const { return m_mouseLockedOnWindow; }
	inline void setMouseSpeed(float speed) { m_mouseSpeed = speed; }
	inline float getMouseSpeed() const { return m_mouseSpeed; }
	inline bool getMouseLeftButtonDown() const { return m_mouseLeftButtonDown; }
	inline bool getMouseRightButtonDown() const { return m_mouseRightButtonDown; }
	inline bool getMouseMiddleButtonDown() const { return m_mouseMiddleButtonDown; }

private:
	JGEInput();
	~JGEInput();

	HWND m_hWnd;
	HINSTANCE m_hInstance;
	IDirectInput8* m_lpInput;
	IDirectInputDevice8* m_lpKeyboard;
	IDirectInputDevice8* m_lpMouse;
	int m_mouseX;
	int m_mouseY;
	int m_clientMouseX;
	int m_clientMouseY;
	bool m_clientMouseOn;
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