#pragma once

#include "JgeDefinition.h"

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