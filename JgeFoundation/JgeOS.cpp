#include "JgeStdafx.h"
#include "JgeOS.h"
#include "JgeMemory.h"

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

// ScreenResolution-----------------------------------------------------------------------------------
jge::ScreenResolution::ScreenResolution():
	pixelWidth(0), pixelHeight(0), bitsPerPixel(0), displayFrequency(0)
{
	// Do nothing
}

jge::ScreenResolution::ScreenResolution(uint pixelWidth, uint pixelHeight, uint bitsPerPixel, uint displayFrequency)
{
	this->pixelWidth = pixelWidth;
	this->pixelHeight = pixelHeight;
	this->bitsPerPixel = bitsPerPixel;
	this->displayFrequency = displayFrequency;
}

jge::ScreenResolution::~ScreenResolution()
{
	// Do nothing
}

jge::ScreenResolution& jge::ScreenResolution::operator=(const jge::ScreenResolution& value)
{
	pixelWidth = value.pixelWidth;
	pixelHeight = value.pixelHeight;
	bitsPerPixel = value.bitsPerPixel;
	displayFrequency = value.displayFrequency;
	return *this;
}

void jge::ScreenResolution::getScreenResolutions(uint* count, jge::ScreenResolution* lpBuffer)
{
	BOOL r = TRUE;
	DEVMODE devMode;
	DWORD index = 0;
	while(r)
	{
		r = EnumDisplaySettings(NULL, index, &devMode);
		if(r)
		{
			if(lpBuffer != null)
			{
				lpBuffer[index].pixelWidth = devMode.dmPelsWidth;
				lpBuffer[index].pixelHeight = devMode.dmPelsHeight;
				lpBuffer[index].bitsPerPixel = devMode.dmBitsPerPel;
				lpBuffer[index].displayFrequency = devMode.dmDisplayFrequency;
			}
		}
		++index;
	}
	if(count != null)
	{
		*count = index;
	}
}

bool jge::ScreenResolution::checkScreenResolution(uint pixelWidth, uint pixelHeight)
{
	uint numResolution = 0;
	jge::ScreenResolution::getScreenResolutions(&numResolution, null);
	jge::ScreenResolution* resolutionList = null;
	jgeNewArray(resolutionList, jge::ScreenResolution, numResolution);
	jge::ScreenResolution::getScreenResolutions(null, resolutionList);
	bool legal = false;
	for(uint i = 0; i < numResolution; ++i)
	{
		if(resolutionList[i].pixelWidth == pixelWidth && resolutionList[i].pixelHeight == pixelHeight)
		{
			legal = true;
			break;
		}
	}
	jgeDeleteArray(resolutionList);
	return legal;
}
