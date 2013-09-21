#include "JgeStdafx.h"
#include "JgeString.h"
#include "JgeMemory.h"

jge::String::String():
	m_lpStr(null)
{
	// Do nothing
}

jge::String::String(const jge::String& str)
{
	*this = str;
}

jge::String::String(const wchar_t* lpStr)
{
	*this = lpStr;
}

jge::String::~String()
{
	cleanup();
}

const jge::String& jge::String::operator=(const jge::String& str)
{
	return *this = (&str == null ? null : str.m_lpStr);
}

const jge::String& jge::String::operator=(const wchar_t* lpStr)
{
	if(*this != lpStr)
	{
		if(lpStr == null || wcslen(lpStr) == 0)
		{
			cleanup();
		}
		else
		{
			if(m_lpStr == null)
			{
				m_lpStr = clone(lpStr);
			}
			else
			{
				if(wcslen(lpStr) > length())
				{
					cleanup();
					m_lpStr = clone(lpStr);
				}
				else
				{
					clone(lpStr, m_lpStr);
				}
			}
		}
	}
	return *this;
}

bool jge::String::operator==(const jge::String& str) const
{
	return *this == (&str == null ? null : str.m_lpStr);
}

bool jge::String::operator==(const wchar_t* lpStr) const
{
	if(length() == 0 && (wcslen(lpStr) == 0 || lpStr == null))
	{
		return true;
	}
	if(length() != wcslen(lpStr))
	{
		return false;
	}
	return wcscmp(m_lpStr, lpStr) == 0;
}

bool jge::String::operator!=(const jge::String& str) const
{
	return !(*this == str);
}

bool jge::String::operator!=(const wchar_t* lpStr) const
{
	return !(*this == lpStr);
}

jge::String jge::String::operator+(const jge::String& str) const
{
	if(&str == null || str.length() == 0)
	{
		return String(*this);
	}
	else
	{
		std::wstringstream ss;
		if(length() != 0)
		{
			ss << m_lpStr;
		}
		ss << str.m_lpStr;
		std::wstring s;
		ss >> s;
		return String(s.c_str());
	}
}

jge::String jge::String::operator+(const wchar_t* lpStr) const
{
	if(lpStr == null || wcslen(lpStr) == 0)
	{
		return String(*this);
	}
	else
	{
		std::wstringstream ss;
		if(length() != 0)
		{
			ss << m_lpStr;
		}
		ss << lpStr;
		std::wstring s;
		ss >> s;
		return String(s.c_str());
	}
}

jge::String jge::String::operator+=(const jge::String& str)
{
	return *this += (&str == null ? null : str.m_lpStr);
}

jge::String jge::String::operator+=(const wchar_t* lpStr)
{
	if(lpStr == null || wcslen(lpStr) == 0)
	{
		return *this;
	}
	else
	{
		std::wstringstream ss;
		if(length() != 0)
		{
			ss << m_lpStr;
		}
		ss << lpStr;
		std::wstring s;
		ss >> s;
		*this = s.c_str();
		return *this;
	}
}

uint jge::String::length() const
{
	return m_lpStr == null ? 0 : wcslen(m_lpStr);
}

void jge::String::release()
{
	cleanup();
}

void jge::String::cleanup()
{
	jgeFree(m_lpStr);
}

wchar_t* jge::String::clone(const wchar_t* lpSrc, wchar_t* lpDest)
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
	jgeMemCpy(lpSrc, lpStrClone, strLen * sizeof(wchar_t));
	return lpStrClone;
}

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