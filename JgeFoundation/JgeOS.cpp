#include "JgeStdafx.h"
#include "JgeOS.h"

void jgeWin32GetWindowAdjustedSize(uint widthSrc, uint heightSrc, int* lpXResult, int* lpYResult, uint* lpWidthResult, uint* lpHeightResult)
{
	uint widthResult = widthSrc + jgeGetSystemMetrics(jgeSM_CXFIXEDFRAME) * 2;
	uint heightResult = heightSrc + jgeGetSystemMetrics(jgeSM_CYFIXEDFRAME) * 2 + jgeGetSystemMetrics(jgeSM_CYCAPTION);
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
		*lpXResult = (int)((jgeGetSystemMetrics(jgeSM_CXSCREEN) - widthResult) * 0.5f);
	}
	if(lpYResult != null)
	{
		*lpYResult = (int)((jgeGetSystemMetrics(jgeSM_CYSCREEN) - heightResult) * 0.5f);
	}
}