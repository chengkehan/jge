#pragma once

#include "JgeMemory.h"
#include "JgeAssert.h"

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

#define jgecsfree(lpStr) jgeFree(lpStr)

#define jgecs1(lpcs, value1) \
{ \
	jgeAssert(lpcs == null); \
	std::stringstream ss; ss << value1; std::string s; ss >> s; lpcs = jgecsclone(s.c_str()); \
}
#define jgecs2(lpcs, value1, value2) \
{ \
	jgeAssert(lpcs == null); \
	std::stringstream ss; ss << value1 << value2; std::string s; ss >> s; lpcs = jgecsclone(s.c_str()); \
}
#define jgecs3(lpcs, value1, value2, value3) \
{ \
	jgeAssert(lpcs == null); \
	std::stringstream ss; ss << value1 << value2 << value3; std::string s; ss >> s; lpcs = jgecsclone(s.c_str()); \
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

#define jgewcsfree(lpStr) jgeFree(lpStr)

#define jgewcs1(lpwcs, value1) \
{ \
	jgeAssert(lpwcs == null); \
	std::wstringstream ss; ss << value1; std::wstring s; ss >> s; lpwcs = jgewcsclone(s.c_str()); \
}
#define jgewcs2(lpwcs, value1, value2) \
{ \
	jgeAssert(lpwcs == null); \
	std::wstringstream ss; ss << value1 << value2; std::wstring s; ss >> s; lpwcs = jgewcsclone(s.c_str()); \
}
#define jgewcs3(lpwcs, value1, value2, value3) \
{ \
	jgeAssert(lpwcs == null); \
	std::wstringstream ss; ss << value1 << value2 << value3; std::wstring s; ss >> s; lpwcs = jgewcsclone(s.c_str()); \
}