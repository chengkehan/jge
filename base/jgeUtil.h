#ifndef __JGE_UTIL_H__
#define __JGE_UTIL_H__

#include <Windows.h>
#include <DxErr.h>
#include <cassert>
#include <cstdio>
#include <ctime>
#include <memory>
#include "JGEPoint.h"

#define JGE_SINGLETON_DECLARE(clazz) \
	friend class std::auto_ptr<##clazz##>; \
	public: \
	static clazz##* getInstance(); \
	private: \
	clazz##(const clazz##& object); \
	static std::auto_ptr<##clazz##> m_instance;

#define JGE_SINGLETON_IMPLEMENTS(clazz) \
	std::auto_ptr<##clazz##> clazz##::m_instance(NULL); \
	clazz##* clazz##::getInstance() \
	{ \
	if(m_instance.get() == NULL) \
		{ m_instance.reset(new clazz##()); } \
		return m_instance.get(); \
	}

#define null 0
#define dword unsigned long
#define word unsigned short
#define uint unsigned int
#define jgeAssert(c) { assert(c); }

#define jgeReleaseCom(lpCom) { if(lpCom){ lpCom->Release(); lpCom = null; } }
#define jgeNew(lp, newType) { assert(lp == null); lp = new newType(); assert(lp != null); }
#define jgeNewArgs1(lp, newType, arg1) { assert(lp == null); lp = new newType(arg1); assert(lp != null); }
#define jgeNewArgs2(lp, newType, arg1, arg2) { assert(lp == null); lp = new newType(arg1, arg2); assert(lp != null); }
#define jgeNewArgs3(lp, newType, arg1, arg2, arg3) { assert(lp == null); lp = new newType(arg1, arg2, arg3); assert(lp != null); }
#define jgeNewArgs4(lp, newType, arg1, arg2, arg3, arg4) { assert(lp == null); lp = new newType(arg1, arg2, arg3, arg4); assert(lp != null); }
#define jgeNewArgs5(lp, newType, arg1, arg2, arg3, arg4, arg5) { assert(lp == null); lp = new newType(arg1, arg2, arg3, arg4, arg5); assert(lp != null); }
#define jgeNewArgs6(lp, newType, arg1, arg2, arg3, arg4, arg5, arg6) { assert(lp == null); lp = new newType(arg1, arg2, arg3, arg4, arg5, arg6); assert(lp != null); }
#define jgeNewArgs7(lp, newType, arg1, arg2, arg3, arg4, arg5, arg6, arg7) { assert(lp == null); lp = new newType(arg1, arg2, arg3, arg4, arg5, arg6, arg7); assert(lp != null); }
#define jgeNewArgs8(lp, newType, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) { assert(lp == null); lp = new newType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); assert(lp != null); }
#define jgeNewArgs9(lp, newType, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) { assert(lp == null); lp = new newType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); assert(lp != null); }
#define jgeNewArray(lp, newType, size) { assert(lp == null); lp = new newType[size]; assert(lp != null); }
#define jgeDelete(lp) { if(lp){ delete lp; lp = null; } }
#define jgeDeleteArray(lp) { if(lp){ delete[] lp; lp = null; } }

#define jgeMalloc(lp, numBytes, lpType) { assert(lp == null); lp = (lpType)malloc(numBytes); assert(lp != null); memset(lp, 0, numBytes); }
#define jgeFree(lp) { if(lp != null) { free(lp); lp = null; } }
#define jgeMemCpy(lpSrc, lpDest, numBytes) { if(lpSrc != lpDest){ assert(lpSrc != null); assert(lpDest != null); memcpy(lpDest, lpSrc, numBytes); } }
#define jgeZeroMem(lp, numBytes) { if(lp != null){ memset(lp, 0, numBytes); } }

#define jgeDXVerifyIf(hr) if(FAILED(hr)){ DXTRACE_ERR_MSGBOX(DXGetErrorDescription(hr), hr); DXTRACE_ERR_MSGBOX(DXGetErrorString(hr), hr); DXTRACE_MSG(DXGetErrorDescription(hr)); DXTRACE_ERR(DXGetErrorDescription(hr), hr); 
#define jgeDXVerifyEndIf }

#define jgeMessageBoxError(msg) { MessageBox(NULL, L##msg, L"Error", MB_OK); }

extern char __jge_trace_buffer[1024];
#define jgeTrace1(format, value1) { sprintf_s(__jge_trace_buffer, 1024, format, value1); OutputDebugStringA(__jge_trace_buffer); }
#define jgeTrace2(format, value1, value2) { sprintf_s(__jge_trace_buffer, 1024, format, value1, value2); OutputDebugStringA(__jge_trace_buffer); }
#define jgeTrace3(format, value1, value2, value3) { sprintf_s(__jge_trace_buffer, 1024, format, value1, value2, value3); OutputDebugStringA(__jge_trace_buffer); }

extern dword __jge_timePoint;
inline void jgeTimeDeltaStart()
{
	__jge_timePoint = timeGetTime();
}
inline void jgeTimeDeltaStop()
{
	dword timeDelta = timeGetTime() - __jge_timePoint;
	jgeTrace1("time delte:%d\n", timeDelta);
}

inline bool jgecsequ(const char* lpStr1, const char* lpStr2)
{
	return 
		lpStr1 == lpStr2 ? true : 
		lpStr1 == null && lpStr2 != null ? false : 
		lpStr1 != null && lpStr2 == null ? false : strcmp(lpStr1, lpStr2) == 0;
}
inline size_t jgecslen(const char* lpStr)
{
	return lpStr == null ? 0 : strlen(lpStr);
}
char* jgecsclone(const char* lpSrc);
inline void jgecsfree(char* lpStr)
{
	jgeFree(lpStr);
}

inline void jgeWin32Exit(int exitCode = 0)
{
	PostQuitMessage(exitCode);
}

inline int jgeWin32CursorShow()
{
	return ShowCursor(TRUE);
}
inline int jgeWin32CursorHide()
{
	return ShowCursor(FALSE);
}

inline BOOL jgeWin32GetWindowRect(HWND hwnd, RECT* lpRect)
{
	return GetWindowRect(hwnd, lpRect);
}
inline BOOL jgeWin32GetClientRect(HWND hwnd, RECT* lpRect)
{
	return GetClientRect(hwnd, lpRect);
}

inline void jgeWin32GetCursorPos(POINT* lpPoint)
{
	GetCursorPos(lpPoint);
}

inline BOOL jgeWin32ClientToScreen(HWND hwnd, POINT* lpPoint)
{
	return ClientToScreen(hwnd, lpPoint);
}
inline BOOL jgeWin32ScreenToClient(HWND hwnd, POINT* lpPoint)
{
	return ScreenToClient(hwnd, lpPoint);
}

inline BOOL jgeWin32ClipCursor(RECT* lpRect)
{
	return ClipCursor(lpRect);
}
inline BOOL jgeWin32GetClipCursor(RECT* lpRect)
{
	return GetClipCursor(lpRect);
}

inline BOOL jgeWin32KeyDown(int vkCode)
{
	return GetAsyncKeyState(vkCode) & 0x8000 ? TRUE : FALSE;
}
inline BOOL jgeWin32KeyUp(int vkCode)
{
	return GetAsyncKeyState(vkCode) & 0x8000 ? FALSE : TRUE;
}

int jgeVectorABPointSide(JGEPoint* lpPointA, JGEPoint* lpPointB, JGEPoint* lpPoint);

inline bool jgeFloatEqual(float a, float b)
{
	float value = a - b;
	return value > -0.0001 && value < 0.0001;
}

#endif