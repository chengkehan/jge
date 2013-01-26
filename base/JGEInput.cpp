#include "JGEInput.h"

JGE_SINGLETON_IMPLEMENTS(JGEInput)

JGEInput::JGEInput()
{
	m_hWnd = null;
	m_hInstance = null;
	m_lpInput = null;
	m_lpKeyboard = null;
	m_lpMouse = null;
	m_mouseX = 0; m_mouseY = 0;
	m_clientMouseX = 0; m_clientMouseY = 0; m_clientMouseOn = false;
	m_mouseClientWidth = 0; m_mouseClientHeight = 0;
	m_mouseSpeed = 1.0f;
	m_mouseLeftButtonDown = false; m_mouseRightButtonDown = false; m_mouseMiddleButtonDown = false;
}

JGEInput::~JGEInput()
{
	if(m_lpKeyboard != null)
	{
		m_lpKeyboard->Unacquire();
	}
	jgeReleaseCom(m_lpKeyboard);

	if(m_lpMouse != null)
	{
		m_lpMouse->Unacquire();
	}
	jgeReleaseCom(m_lpMouse);
	mouseUnlockOnWindow();

	jgeReleaseCom(m_lpInput);
}

bool JGEInput::initInput(HINSTANCE hInstance, HWND hWnd)
{
	bool result = initKeyboard(hInstance, hWnd) && initMouse(hInstance, hWnd) && initClientMouse(hWnd);
	if(result)
	{
		return true;
	}
	else
	{
		jgeMessageBoxError("Init Input Failed");
		return false;
	}
}

bool JGEInput::initKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	if(hInstance == null || hWnd == null)
	{
		return false;
	}

	if(m_lpKeyboard == null)
	{
		if(!initInputDevice(hInstance))
		{
			return false;
		}

		if(FAILED(m_lpInput->CreateDevice(GUID_SysKeyboard, &m_lpKeyboard, null)))
		{
			jgeMessageBoxError("Keyboard CreateDevice Failed");
			return false;
		}

		if(FAILED(m_lpKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		{
			jgeMessageBoxError("Keyboard SetDataFormat Failed");
			return false;
		}

		if(FAILED(m_lpKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		{
			jgeMessageBoxError("Keyboard SetCooperativeLevel Failed");
			return false;
		}

		if(FAILED(m_lpKeyboard->Acquire()))
		{
			jgeMessageBoxError("Keyboard Acquire Failed");
			return false;
		}
		m_hWnd = hWnd;
		m_hInstance = hInstance;
	}

	return true;
}

bool JGEInput::initMouse(HINSTANCE hInstance, HWND hWnd)
{
	if(hInstance == null || hWnd == null)
	{
		return false;
	}

	if(m_lpMouse == null)
	{
		if(!initInputDevice(hInstance))
		{
			return false;
		}

		if(FAILED(m_lpInput->CreateDevice(GUID_SysMouse, &m_lpMouse, null)))
		{
			jgeMessageBoxError("Mouse CreateDevice Failed");
			return false;
		}

		if(FAILED(m_lpMouse->SetDataFormat(&c_dfDIMouse)))
		{
			jgeMessageBoxError("Mouse SetDataFormat Failed");
			return false;
		}

		if(FAILED(m_lpMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		{
			jgeMessageBoxError("Mouse SetCooperativeLevel Failed");
			return false;
		}

		if(FAILED(m_lpMouse->Acquire()))
		{
			jgeMessageBoxError("Mouse Acquire Failed");
			return false;
		}

		RECT windowRect;
		jgeWin32GetWindowRect(hWnd, &windowRect);
		m_mouseClientWidth = windowRect.right - windowRect.left;
		m_mouseClientHeight = windowRect.bottom - windowRect.top;
		m_hWnd = hWnd;
		m_hInstance = hInstance;
	}

	return true;
}

bool JGEInput::initClientMouse(HWND hWnd)
{
	if(hWnd == null)
	{
		return false;
	}

	m_clientMouseOn = true;
	m_hWnd = hWnd;
	return true;
}

bool JGEInput::updateInput()
{
	bool keyboardResult = m_lpKeyboard == null ? true : updateKeyboard();
	bool mouseResult = m_lpMouse == null ? true : updateMouse();
	bool clientMouseResult = m_clientMouseOn ? updateClientMouse() : true;

	return keyboardResult && mouseResult && clientMouseResult;
}

bool JGEInput::updateKeyboard()
{
	if(m_lpKeyboard == null)
	{
		return false;
	}

	HRESULT result = m_lpKeyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if(FAILED(result))
	{
		if(result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_lpKeyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool JGEInput::updateMouse()
{
	if(m_lpMouse == null)
	{
		return false;
	}

	HRESULT result = m_lpMouse->GetDeviceState(sizeof(m_mouseState), (LPVOID)&m_mouseState);
	if(FAILED(result))
	{
		if(result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			if(FAILED(m_lpMouse->Acquire()))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		m_mouseX = max(min(m_mouseX + (int)((FLOAT)m_mouseState.lX * m_mouseSpeed), m_mouseClientWidth), 0);
		m_mouseY = max(min(m_mouseY + (int)((FLOAT)m_mouseState.lY * m_mouseSpeed), m_mouseClientHeight), 0);
		m_mouseLeftButtonDown = m_mouseState.rgbButtons[0] != 0;
		m_mouseRightButtonDown = m_mouseState.rgbButtons[1] != 0;
		m_mouseMiddleButtonDown = m_mouseState.rgbButtons[2] != 0;
	}

	return true;
}

bool JGEInput::updateClientMouse()
{
	if(m_clientMouseOn)
	{
		static POINT p;
		jgeWin32GetCursorPos(&p);
		jgeWin32ScreenToClient(m_hWnd, &p);
		m_clientMouseX = p.x;
		m_clientMouseY = p.y;

		return true;
	}
	else
	{
		return false;
	}
}

bool JGEInput::keyDown(int diKeyCode)
{
	if(diKeyCode < 0 || diKeyCode >= 256)
	{
		return false;
	}
	else
	{
		return (m_keyboardState[diKeyCode] & 0x80) != 0;
	}	
}

bool JGEInput::mouseLockOnWindow()
{
	if(m_hWnd == null)
	{
		return false;
	}

	if(!jgeWin32GetClipCursor(&m_mouseClipOriginal))
	{
		return false;
	}

	RECT lockRect;

	POINT screenPoint = { 0, 0 };
	if(!jgeWin32ClientToScreen(m_hWnd, &screenPoint))
	{
		return false;
	}
	lockRect.left = screenPoint.x;
	lockRect.top = screenPoint.y;

	RECT clientRect;
	if(!jgeWin32GetClientRect(m_hWnd, &clientRect))
	{
		return false;
	}
	screenPoint.x = clientRect.right;
	screenPoint.y = clientRect.bottom;
	if(!jgeWin32ClientToScreen(m_hWnd, &screenPoint))
	{
		return false;
	}
	lockRect.right = screenPoint.x;
	lockRect.bottom = screenPoint.y;

	if(!jgeWin32ClipCursor(&lockRect))
	{
		return false;
	}

	m_mouseLockedOnWindow = true;

	return true;
}

bool JGEInput::mouseUnlockOnWindow()
{
	if(m_hWnd == null)
	{
		return false;
	}
	if(!m_mouseLockedOnWindow)
	{
		return false;
	}

	if(!jgeWin32ClipCursor(&m_mouseClipOriginal))
	{
		return false;
	}

	m_mouseLockedOnWindow = false;

	return true;
}

bool JGEInput::initInputDevice(HINSTANCE hInstance)
{
	if(hInstance == null)
	{
		return false;
	}

	if(m_lpInput == null)
	{
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_lpInput, null)))
		{
			jgeMessageBoxError("DirectInput8Create Failed");
			return false;
		}
	}
	return true;
}