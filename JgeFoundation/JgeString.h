#pragma once

#include "JgeMemory.h"
#include "JgeAssert.h"
#include "JgeNoncopyable.h"
#include "JgeDefinition.h"

namespace jge
{
	class JGE_DLL String
	{
	public:
		//String();
		//String(const String& str);
		//String(const wchar_t* lpStr);
		//~String();

		//const String& operator=(const String& str);
		//const String& operator=(const wchar_t* lpStr);
		//bool operator==(const String& str) const;
		//bool operator==(const wchar_t* lpStr) const;
		//bool operator!=(const String& str) const;
		//bool operator!=(const wchar_t* lpStr) const;
		//String operator+(const String& str) const;
		//String operator+(const wchar_t* lpStr) const;
		//String operator+=(const String& str);
		//String operator+=(const wchar_t* lpStr);
		
		//uint length() const;
		//void release();

	private:
		//wchar_t* m_lpStr;

		//void cleanup();
		//wchar_t* clone(const wchar_t* lpSrc, wchar_t* lpDest = null);

		class StringAlloc;

		template<uint blockBytes>
		class StringPool;

		template<uint numChars>
		class StringMemory;

		// IndexStack-----------------------------------------------------------------------------------
		class IndexStack
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

		// StringAlloc-----------------------------------------------------------------------------------
		class StringAlloc
		{
		private:
			
		};

		// StringPool-----------------------------------------------------------------------------------
		template<uint blockBytes>
		class StringPool
		{
		public:
			StringPool(uint numBlocks):
				m_blockBytes(blockBytes), m_numBlocks(numBlocks)
			{
				jgeNewArray(m_lpStringMemoryList, StringMemory<blockBytes>, numBlocks);
				jgeNewArgs1(m_lpFreeIndexStack, IndexStack, numBlocks);
				jgeNewArgs1(m_lpUsedIndexStack, IndexStack, numBlocks);
				m_lpNextStringPool = null;
			}
			~StringPool()
			{
				jgeDeleteArray(m_lpStringMemoryList);
				jgeDelete(m_lpFreeIndexStack);
				jgeDelete(m_lpUsedIndexStack);
			}

		private:
			uint m_blockBytes;
			uint m_numBlocks;
			StringMemory<blockBytes>* m_lpStringMemoryList;
			StringPool* m_lpNextStringPool;
			IndexStack* m_lpFreeIndexStack;
			IndexStack* m_lpUsedIndexStack;
		};

		// StringMemory-----------------------------------------------------------------------------------
		template<uint numChars>
		class StringMemory
		{
		public:
			StringMemory(StringPool<numChars>* lpStringPool, uint indexInPool):
				m_lpStringPool(lpStringPool), m_indexInPool(indexInPool), m_usedCount(0)
			{
				// Do nothing
			}
			~StringMemory()
			{
				// Do nothing
			}

		private:
			StringPool<numChars>* m_lpStringPool;
			uint m_usedCount;
			uint m_indexInPool;
			wchar_t m_str[numChars];
		};
	};
}

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