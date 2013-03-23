#ifndef __JGE_3D_H__
#define __JGE_3D_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include "jgeUtil.h"

// window process
namespace jge3d
{
	typedef void (*WinProcCallback)(HWND hWnd, uint msg, WPARAM wparam, LPARAM lparam);
	// message mapping, key msg, value callback
	typedef std::map<uint, WinProcCallback> WinProcCallbackMap;

	extern WinProcCallbackMap winProcCallbacks;

	LRESULT CALLBACK winProc(HWND hWnd, uint msg, WPARAM wparam, LPARAM lparam);
}

class JGE3D
{
JGE_SINGLETON_DECLARE(JGE3D)

public:
	typedef bool (*SETUPCALLBACK)(void);
	typedef void (*RELEASECALLBACK)(void);
	typedef void (*FRAMECALLBACK)(uint timeDelta);
	typedef bool (*WMDESTROYCALLBACK)();
	typedef bool (*WMESCAPEKEYDOWNCALLBACK)();
	typedef void (*DEVICELOSECALLBACK)();
	typedef bool (*DEVICELOSERESETCALLBACK)();

	SETUPCALLBACK m_setupCallback;
	RELEASECALLBACK m_releaseCallback;
	FRAMECALLBACK m_frameCallback;
	WMDESTROYCALLBACK m_wmDestroyCallback;
	WMESCAPEKEYDOWNCALLBACK m_wmEscapeKeyDownCallback;
	DEVICELOSECALLBACK m_deviceLoseCallback;
	DEVICELOSERESETCALLBACK m_deviceLoseResetCallback;

	bool init(HINSTANCE hInstance, int windowX = -1, int windowY = -1, uint windowWidth = 800, uint windowHeight = 600, bool windowed = true, bool systemMenu = false);
	bool initManual(HINSTANCE hInstance, HWND hwnd, const D3DVIEWPORT9* lpViewPort);
	void run();
	void runManual();
	inline HWND getHWnd() const { return m_hWnd; }
	inline HINSTANCE getHInstance() const { return m_hInstance; }
	inline IDirect3DDevice9* getDirect3DDevice() const { return m_lpd3dd; }
	inline uint getWindowWidth() const { return m_windowWidth; }
	inline uint getWindowHeight() const { return m_windowHeight; }
	inline uint getFPS() const { return m_fps; }
	void setFPS(uint value);
	inline bool getWindowd() const { return m_windowd; }
	bool setMessageCallback(uint msg, jge3d::WinProcCallback callback);
	bool clearMessageCallback(uint msg);
	bool containsMesageCallback(uint msg) const;
	jge3d::WinProcCallback getMessageCallback(uint msg) const;

	static bool setRenderState(
		IDirect3DDevice9* lpd3dd, dword cullMode = D3DCULL_CCW, bool lighting = true, bool zEnable = true, 
		dword shadeMode = D3DSHADE_GOURAUD, dword fillMode = D3DFILL_SOLID, bool alphaBlendEnable = false
	);
	static bool setProjectionPerspectiveTransform(IDirect3DDevice9* lpd3dd, uint windowWidth, uint windowHeight);
	static bool setViewTransform(IDirect3DDevice9* lpd3dd, D3DXVECTOR3* lpeye, D3DXVECTOR3* lptarget, D3DXVECTOR3* lpup);
	static bool setViewTransform(IDirect3DDevice9* lpd3dd, float eyeX, float eyeY, float eyeZ, float targetX, float targetY, float targetZ, float upX, float upY, float upZ);

private:
	JGE3D();
	~JGE3D();

	HWND m_hWnd;
	HINSTANCE m_hInstance;
	IDirect3DDevice9* m_lpd3dd;
	uint m_windowWidth;
	uint m_windowHeight;
	bool m_windowd;
	bool m_init;
	bool m_running;
	uint m_fps;
	uint m_fpsTime;
	D3DPRESENT_PARAMETERS m_presentParams;
	D3DVIEWPORT9 m_viewPort;
};

#endif