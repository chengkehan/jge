#include "jgeUtil.h"

char __jge_trace_buffer[1024];
DWORD __jge_timePoint;

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

//设有向线段AB，两端点A（xa,ya）,B(xb,yb)
//另一点C(xc,yc)
//float f = (xb - xa) * (yc - ya) - (xc - xa) * (yb - ya);
//if(f > 0)
//	点C位于有向线段AB的左侧
//else if(f == 0)
//点C位于有向线段AB上（也有可能在延长线上）
//else
//点C位于有向线段AB的右侧
int jgeVectorABPointSide(JGEPoint* lpPointA, JGEPoint* lpPointB, JGEPoint* lpPoint)
{
	if(lpPointA == null || lpPointB == null || lpPoint == null)
	{
		return -2;
	}
	
	float f = (lpPointB->m_x - lpPointA->m_x) * (lpPoint->m_y - lpPointA->m_y) - (lpPoint->m_x - lpPointA->m_x) * (lpPointB->m_y - lpPointA->m_y);
	if(f > 0.0f)
	{
		return 1;
	}
	else if(jgeFloatEqual(f, 0.0f))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}