#pragma once

#include "JgeCommon.h"
#include "JgeMemory.h"

namespace jge
{
	class JGE_DLL File
	{
	public:
		File();
		~File();

		bool open(const wchar_t* lpPath);
		bool exist();
		inline const wchar_t* getPath() { return m_lpPath; }

		static bool exist(const wchar_t* lpPath);
		//static bool read(const wchar_t* lpFileURL, char* lpFileData, uint* numBytes, bool binary = false);

	private:
		File(const File& value);
		File& operator=(const File& value);

		wchar_t* m_lpPath;

		void release();
	};
}