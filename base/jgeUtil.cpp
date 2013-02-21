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
	// strLen * 2 ����Ϊwcharռ�õĿռ���char������
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

//�������߶�AB�����˵�A��xa,ya��,B(xb,yb)
//��һ��C(xc,yc)
//float f = (xb - xa) * (yc - ya) - (xc - xa) * (yb - ya);
//if(f > 0)
//	��Cλ�������߶�AB�����
//else if(f == 0)
//��Cλ�������߶�AB�ϣ�Ҳ�п������ӳ����ϣ�
//else
//��Cλ�������߶�AB���Ҳ�
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