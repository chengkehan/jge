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
public:
	JGE3D();
	~JGE3D();

	typedef bool (*SETUPCALLBACK)(void);
	typedef void (*RELEASECALLBACK)(void);
	typedef void (*FRAMECALLBACK)(uint timeDelta);

	SETUPCALLBACK setupCallback;
	RELEASECALLBACK releaseCallback;
	FRAMECALLBACK frameCallback;

	bool init(HINSTANCE hInstance, int windowX = 0, int windowY = 0, uint windowWidth = 800, uint windowHeight = 600, 
		bool windowed = true, D3DDEVTYPE deviceType = D3DDEVTYPE_HAL, uint maxTextureBlendStages = 1, uint fps = 30);
	void run();
	HWND getHWnd() const;
	HINSTANCE getHInstance() const;
	IDirect3DDevice9* getDirect3DDevice() const;
	int getWindowX() const;
	int getWindowY() const;
	int getWindowWidth() const;
	int getWindowHeight() const;
	uint getFPS() const;
	void setFPS(uint value);
	bool getWindowd() const;
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
	JGE3D(const JGE3D& jcd3d);

	HWND m_hWnd;
	HINSTANCE m_hInstance;
	IDirect3DDevice9* m_lpd3dd;
	int m_windowX;
	int m_windowY;
	int m_windowWidth;
	int m_windowHeight;
	bool m_windowd;
	bool m_init;
	bool m_running;
	uint m_fps;
	uint m_fpsTime;
};

#endif