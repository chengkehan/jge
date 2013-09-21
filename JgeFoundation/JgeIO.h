#pragma once

#include "JgeMemory.h"
#include "JgeNoncopyable.h"

namespace jge
{
	class JGE_DLL File : private Noncopyable
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
		wchar_t* m_lpPath;

		void release();
		bool readInvoke(char* lpFileData, uint* numBytes, bool binary);
	};
}