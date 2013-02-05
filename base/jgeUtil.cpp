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