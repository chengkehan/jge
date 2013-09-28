#include "JgeStdafx.h"
#include "JgeString.h"
#include "JgeMemory.h"

using namespace jge;

// String-----------------------------------------------------------------------------------
String::String()
{

}

String::String(const String& value)
{

}

String::String(const wchar_t* lpString)
{

}

String::~String()
{

}

// StringAlloc-----------------------------------------------------------------------------------
const uint String::StringAlloc::NUM_BLOCKS_PER_STRINGPOOL = 100;
String::StringMemory String::StringAlloc::m_nullStringMemory;
String::StringAlloc String::StringAlloc::m_instance;

String::StringAlloc* String::StringAlloc::getInstance()
{
	return &m_instance;
}

String::StringAlloc::StringAlloc()
{
	// Do nothing
}

String::StringAlloc::~StringAlloc()
{
	// Do nothing
}

String::StringMemory* String::StringAlloc::alloc(const wchar_t* lpString)
{
	if(lpString == null)
	{
		return &String::StringAlloc::m_nullStringMemory;
	}
	else
	{
		uint length = jgewcslen(lpString);
		uint fixedLength = getCeilPowerOf2(length);
		String::StringAlloc::StringPoolMap::iterator findResult = m_stringPoolMap.find(fixedLength);
		if(findResult != m_stringPoolMap.end())
		{
			return findResult->second->pushString(lpString);
		}
		else
		{
			String::StringPool* lpStringPool = null;
			jgeNewArgs2(lpStringPool, String::StringPool, NUM_BLOCKS_PER_STRINGPOOL, fixedLength);
			m_stringPoolMap.insert(String::StringAlloc::StringPoolMap::value_type(fixedLength, lpStringPool));
			return lpStringPool->pushString(lpString);
		}
	}
}

uint String::StringAlloc::getCeilPowerOf2(uint value)
{
	uint v = 1;
	while(true)
	{
		if(v >= value)
		{
			return v;	
		}
		v *= 2;
	}
	jgeAssert(false);
	return 0;
}

// IndexStack-----------------------------------------------------------------------------------
String::IndexStack::IndexStack(uint capability):
	m_lpIndexList(null), m_position(0), m_capability(capability)
{
	jgeNewArray(m_lpIndexList, uint, capability);
}

String::IndexStack::~IndexStack()
{
	jgeDeleteArray(m_lpIndexList);
}

bool String::IndexStack::push(uint index)
{
	if(m_position >= m_capability)
	{
		return false;
	}
	else
	{
		m_lpIndexList[m_position++] = index;
		return true;
	}
}

bool String::IndexStack::pop(uint* index)
{
	if(m_position == 0)
	{
		return false;
	}
	else
	{
		if(index != null)
		{
			*index = m_lpIndexList[m_position--];
		}
		return true;
	}
}

// StringPool-----------------------------------------------------------------------------------
String::StringPool::StringPool(uint numBlocks, uint blockChars):
	m_numBlocks(numBlocks), m_blockChars(blockChars), m_lpNextStringPool(null)
{
	jgeAssert(m_numBlocks > 0);
	jgeAssert(m_blockChars > 0);
	jgeMalloc(m_lpStringMemoryList, numBlocks * (sizeof(String::StringMemory) + sizeof(wchar_t)* blockChars), String::StringMemory*);
	jgeNewArgs1(m_lpFreeIndexStack, String::IndexStack, numBlocks);
	jgeNewArgs1(m_lpUsedIndexStack, String::IndexStack, numBlocks);

	for(uint i = 0; i < numBlocks; ++i)
	{
		String::StringMemory* lpStringMemory = &m_lpStringMemoryList[i];
		lpStringMemory->lpBelongToWhichStringPool = this;
		lpStringMemory->usedCount = 0;
		lpStringMemory->indexInStringPool = i;
		lpStringMemory->lpStr = (wchar_t*)(lpStringMemory + sizeof(String::StringMemory));
		m_lpFreeIndexStack->push(i);
	}
}

String::StringPool::~StringPool()
{
	jgeDeleteArray(m_lpStringMemoryList);
	jgeDelete(m_lpFreeIndexStack);
	jgeDelete(m_lpUsedIndexStack);

	String::StringPool* lpStringPoolNext = m_lpNextStringPool;
	while(lpStringPoolNext != null)
	{
		String::StringPool* lpStringPoolNextNext = lpStringPoolNext->m_lpNextStringPool;
		jgeDelete(lpStringPoolNext);
		lpStringPoolNext = lpStringPoolNextNext;
	}
}

String::StringMemory* String::StringPool::pushString(const wchar_t* lpString)
{
	String::StringPool* lpStringPoolPrev = null;
	String::StringPool* lpStringPoolThis = this;
	String::StringMemory* lpStringMemory = null;
	while(true)
	{
		if(lpStringPoolThis == null)
		{
			jgeNewArgs2(lpStringPoolThis, String::StringPool, m_numBlocks, m_blockChars);
			jgeAssert(lpStringPoolPrev != null);
			lpStringPoolPrev->m_lpNextStringPool = lpStringPoolThis;
			lpStringPoolThis->pushString(lpString);
			break;
		}
		else
		{
			uint index;
			if(m_lpFreeIndexStack->pop(&index))
			{
				m_lpUsedIndexStack->push(index);
				jgewcsclone(lpString, m_lpStringMemoryList[index].lpStr);
				lpStringMemory = &m_lpStringMemoryList[index];
				break;
			}
			else
			{
				lpStringPoolPrev = lpStringPoolThis;
				lpStringPoolThis = m_lpNextStringPool;
			}
		}
	}
	return lpStringMemory;
}

// StringMemory-----------------------------------------------------------------------------------
String::StringMemory::StringMemory():
	lpBelongToWhichStringPool(null), usedCount(0), indexInStringPool(0), lpStr(null)
{
	// Do nothing
}

// c style string-----------------------------------------------------------------------------------
char* jgecsclone(const char* lpSrc, char* lpDest)
{
	if(lpSrc == null)
	{
		return null;
	}

	size_t strLen = jgecslen(lpSrc) + 1/*'\0'*/;
	char* lpStrClone = null;
	if(lpDest == null)
	{
		jgeMalloc(lpStrClone, strLen, char*);
	}
	else
	{
		lpStrClone = lpDest;
	}
	jgeMemCpy(lpSrc, lpStrClone, strLen);
	return lpStrClone;
}

wchar_t* jgewcsclone(const wchar_t* lpSrc, wchar_t* lpDest)
{
	if(lpSrc == null)
	{
		return null;
	}

	size_t strLen = jgewcslen(lpSrc) + 1/*'\0'*/;
	wchar_t* lpStrClone = null;
	if(lpDest == null)
	{
		jgeMalloc(lpStrClone, strLen * sizeof(wchar_t), wchar_t*);
	}
	else
	{
		lpStrClone = lpDest;
	}
	jgeMemCpy(lpSrc, lpStrClone, strLen * 2);
	return lpStrClone;
}