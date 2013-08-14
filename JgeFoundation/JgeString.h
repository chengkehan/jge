#pragma once

#include "JgeCommon.h"
#include "JgeMemory.h"

// char string
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

JGE_DLL char* jgecsclone(const char* lpSrc, char* lpDest = null);

inline void jgecsfree(char* lpStr)
{
	jgeFree(lpStr);
}

// wide char string
inline bool jgewcsequ(const wchar_t* lpStr1, const wchar_t* lpStr2)
{
	return 
		lpStr1 == lpStr2 ? true : 
		lpStr1 == null && lpStr2 != null ? false : 
		lpStr1 != null && lpStr2 == null ? false : wcscmp(lpStr1, lpStr2) == 0;
}
inline size_t jgewcslen(const wchar_t* lpStr)
{
	return lpStr == null ? 0 : wcslen(lpStr);
}

JGE_DLL wchar_t* jgewcsclone(const wchar_t* lpSrc, wchar_t* lpDest = null);

inline void jgewcsfree(wchar_t* lpStr)
{
	jgeFree(lpStr);
}