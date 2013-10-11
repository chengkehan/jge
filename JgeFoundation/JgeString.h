#pragma once

#include "JgeStdafx.h"
#include "JgeMemory.h"
#include "JgeAssert.h"
#include "JgeNoncopyable.h"
#include "JgeDefinition.h"

namespace jge
{
	// String-----------------------------------------------------------------------------------
	class JGE_DLL String
	{
	public:
		String();
		String(const String& value);
		String(const wchar_t* lpString);
		~String();

		const String& operator=(const String& value);
		const String& operator=(const wchar_t* lpString);
		bool operator==(const String& value);
		bool operator==(const wchar_t* lpString);

		bool isNull();
		bool isEmpty();
		bool isNullOrEmpty();

	private:
		// -----------------------------------------------------------------------------------
		class StringPool;
		struct StringMemory;

		// -----------------------------------------------------------------------------------
		StringMemory* m_lpStringMemory;

		void release();

		// StringAlloc-----------------------------------------------------------------------------------
		class StringAlloc : private Noncopyable
		{
		friend String;

		private:
			static const uint NUM_BLOCKS_PER_STRINGPOOL;
			static StringMemory m_nullStringMemory;
			static StringMemory m_emptyStringMemory;

			typedef std::map<uint, StringPool*> StringPoolMap;
			static StringPoolMap m_stringPoolMap;

			static StringMemory* alloc(const wchar_t* lpString);
			static uint getCeilPowerOf2(uint value);
		};

		// IndexStack-----------------------------------------------------------------------------------
		class IndexStack : private Noncopyable
		{
		public:
			IndexStack(uint capability);
			~IndexStack();

			bool push(uint index);
			bool pop(uint* index);

		private:
			uint* m_lpIndexList;
			uint m_position;
			uint m_capability;
		};

		// StringPool-----------------------------------------------------------------------------------
		class StringPool : private Noncopyable
		{
		public:
			StringPool(uint numBlocks, uint blockChars);
			~StringPool();

			inline uint getBlockChars() const { return m_blockChars; }
			StringMemory* pushString(const wchar_t* lpString);
			void recycle(StringMemory* lpStringMemory);

		private:
			uint m_numBlocks;
			uint m_blockChars;
			StringMemory* m_lpStringMemoryList;
			StringPool* m_lpNextStringPool;
			IndexStack* m_lpFreeIndexStack;
		};

		// StringMemory-----------------------------------------------------------------------------------
		struct StringMemory : private Noncopyable
		{
		public:
			StringMemory();

			StringPool* lpBelongToWhichStringPool;
			uint usedCount;
			uint indexInStringPool;
			wchar_t* lpStr;
		};
	};
}

// c style string-----------------------------------------------------------------------------------
// char string
inline bool jgecsequ(const char* lpStr1, const char* lpStr2)
{
	return 
		lpStr1 == lpStr2 ? true : 
		lpStr1 == null && lpStr2 != null ? false : 
		lpStr1 != null && lpStr2 == null ? false : strcmp(lpStr1, lpStr2) == 0;
}

inline size_t jgecslen(const char* lpStr)
{
	return lpStr == null ? 0 : strlen(lpStr);
}

JGE_DLL char* jgecsclone(const char* lpSrc, char* lpDest = null);

#define jgecsfree(lpStr) jgeFree(lpStr)

#define jgecs1(lpcs, value1) \
{ \
	jgeAssert(lpcs == null); \
	std::stringstream ss; ss << value1; std::string s; ss >> s; lpcs = jgecsclone(s.c_str()); \
}
#define jgecs2(lpcs, value1, value2) \
{ \
	jgeAssert(lpcs == null); \
	std::stringstream ss; ss << value1 << value2; std::string s; ss >> s; lpcs = jgecsclone(s.c_str()); \
}
#define jgecs3(lpcs, value1, value2, value3) \
{ \
	jgeAssert(lpcs == null); \
	std::stringstream ss; ss << value1 << value2 << value3; std::string s; ss >> s; lpcs = jgecsclone(s.c_str()); \
}

// wide char string
inline bool jgewcsequ(const wchar_t* lpStr1, const wchar_t* lpStr2)
{
	return 
		lpStr1 == lpStr2 ? true : 
		lpStr1 == null && lpStr2 != null ? false : 
		lpStr1 != null && lpStr2 == null ? false : wcscmp(lpStr1, lpStr2) == 0;
}
inline size_t jgewcslen(const wchar_t* lpStr)
{
	return lpStr == null ? 0 : wcslen(lpStr);
}

JGE_DLL wchar_t* jgewcsclone(const wchar_t* lpSrc, wchar_t* lpDest = null);

#define jgewcsfree(lpStr) jgeFree(lpStr)

#define jgewcs1(lpwcs, value1) \
{ \
	jgeAssert(lpwcs == null); \
	std::wstringstream ss; ss << value1; std::wstring s; ss >> s; lpwcs = jgewcsclone(s.c_str()); \
}
#define jgewcs2(lpwcs, value1, value2) \
{ \
	jgeAssert(lpwcs == null); \
	std::wstringstream ss; ss << value1 << value2; std::wstring s; ss >> s; lpwcs = jgewcsclone(s.c_str()); \
}
#define jgewcs3(lpwcs, value1, value2, value3) \
{ \
	jgeAssert(lpwcs == null); \
	std::wstringstream ss; ss << value1 << value2 << value3; std::wstring s; ss >> s; lpwcs = jgewcsclone(s.c_str()); \
}