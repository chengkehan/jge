#pragma once

#include "JgeCommon.h"

JGE_DLL void jgeGetWindowAdjustedSize(uint widthSrc, uint heightSrc, int* lpXResult = null, int* lpYResult = null, uint* lpWidthResult = null, uint* lpHeightResult = null);

namespace jge
{
	class JGE_DLL ScreenResolution
	{
	public:
		ScreenResolution();
		ScreenResolution(uint pixelWidth, uint pixelHeight, uint bitsPerPixel, uint displayFrequency);
		~ScreenResolution();

		ScreenResolution& operator=(const ScreenResolution& value);

		uint pixelWidth;
		uint pixelHeight;
		uint bitsPerPixel;
		uint displayFrequency;

		static void getScreenResolutions(uint* count, ScreenResolution* lpBuffer);
		static bool checkScreenResolution(uint pixelWidth, uint pixelHeight);
	};
}