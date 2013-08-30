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
		bool read(Buffer* lpBuffer, bool binary);

		static bool exist(const wchar_t* lpPath);

	private:
		File(const File& value);
		File& operator=(const File& value);

		wchar_t* m_lpPath;

		void release();
		bool readInvoke(char* lpFileData, uint* numBytes, bool binary);
	};
}