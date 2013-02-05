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