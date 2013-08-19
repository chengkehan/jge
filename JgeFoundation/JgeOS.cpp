#include "JgeStdafx.h"
#include "JgeOS.h"

void jgeGetWindowAdjustedSize(uint widthSrc, uint heightSrc, int* lpXResult, int* lpYResult, uint* lpWidthResult, uint* lpHeightResult)
{
	uint widthResult = widthSrc + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
	uint heightResult = heightSrc + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);
	if(lpWidthResult != null)
	{
		*lpWidthResult = widthResult;
	}
	if(lpHeightResult != null)
	{
		*lpHeightResult = heightResult;
	}
	if(lpXResult != null)
	{
		*lpXResult = (int)((GetSystemMetrics(SM_CXSCREEN) - widthResult) * 0.5f);
	}
	if(lpYResult != null)
	{
		*lpYResult = (int)((GetSystemMetrics(SM_CYSCREEN) - heightResult) * 0.5f);
	}
}