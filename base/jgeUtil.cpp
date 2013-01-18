#include "jgeUtil.h"

char __jge_trace_buffer[1024];
DWORD __jge_timePoint;

char* jgecsclone(const char* lpSrc)
{
	if(lpSrc == null)
	{
		return null;
	}

	size_t strLen = jgecslen(lpSrc) + 1/*'\0'*/;
	char* lpStrClone = null;
	jgeMalloc(lpStrClone, strLen, char*);
	jgeMemCpy(lpSrc, lpStrClone, strLen);
	return lpStrClone;
}