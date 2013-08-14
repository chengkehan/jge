#include "JgeStdafx.h"
#include "JgeString.h"
#include "JgeMemory.h"

char* jgecsclone(const char* lpSrc, char* lpDest)
{
	if(lpSrc == null)
	{
		return null;
	}

	size_t strLen = jgecslen(lpSrc) + 1/*'\0'*/;
	char* lpStrClone = null;
	if(lpDest == null)
	{
		jgeMalloc(lpStrClone, strLen, char*);
	}
	else
	{
		lpStrClone = lpDest;
	}
	jgeMemCpy(lpSrc, lpStrClone, strLen);
	return lpStrClone;
}

wchar_t* jgewcsclone(const wchar_t* lpSrc, wchar_t* lpDest)
{
	if(lpSrc == null)
	{
		return null;
	}

	size_t strLen = jgewcslen(lpSrc) + 1/*'\0'*/;
	wchar_t* lpStrClone = null;
	// strLen * 2 是因为wchar占用的空间是char的两倍
	if(lpDest == null)
	{
		jgeMalloc(lpStrClone, strLen * 2, wchar_t*);
	}
	else
	{
		lpStrClone = lpDest;
	}
	jgeMemCpy(lpSrc, lpStrClone, strLen * 2);
	return lpStrClone;
}