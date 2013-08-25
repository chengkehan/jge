#pragma once

#include "JgeCommon.h"

namespace jge
{
	class JGE_DLL File
	{
	public:
		File();
		~File();

		static bool read(const wchar_t* lpFileURL, char* lpFileData, uint* numBytes, bool binary);

	private:
		File(const File& value);
		File& operator=(const File& value);
	};
}