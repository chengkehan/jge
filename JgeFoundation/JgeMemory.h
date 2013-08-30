#pragma once

#include "JgeCommon.h"

#define jgeNew(lp, newType) { assert(lp == null); lp = new newType(); assert(lp != null); }
#define jgeNewArgs1(lp, newType, arg1) { assert(lp == null); lp = new newType(arg1); assert(lp != null); }
#define jgeNewArgs2(lp, newType, arg1, arg2) { assert(lp == null); lp = new newType(arg1, arg2); assert(lp != null); }
#define jgeNewArgs3(lp, newType, arg1, arg2, arg3) { assert(lp == null); lp = new newType(arg1, arg2, arg3); assert(lp != null); }
#define jgeNewArgs4(lp, newType, arg1, arg2, arg3, arg4) { assert(lp == null); lp = new newType(arg1, arg2, arg3, arg4); assert(lp != null); }
#define jgeNewArgs5(lp, newType, arg1, arg2, arg3, arg4, arg5) { assert(lp == null); lp = new newType(arg1, arg2, arg3, arg4, arg5); assert(lp != null); }
#define jgeNewArgs6(lp, newType, arg1, arg2, arg3, arg4, arg5, arg6) { assert(lp == null); lp = new newType(arg1, arg2, arg3, arg4, arg5, arg6); assert(lp != null); }
#define jgeNewArgs7(lp, newType, arg1, arg2, arg3, arg4, arg5, arg6, arg7) { assert(lp == null); lp = new newType(arg1, arg2, arg3, arg4, arg5, arg6, arg7); assert(lp != null); }
#define jgeNewArgs8(lp, newType, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) { assert(lp == null); lp = new newType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); assert(lp != null); }
#define jgeNewArgs9(lp, newType, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) { assert(lp == null); lp = new newType(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); assert(lp != null); }
#define jgeNewArray(lp, newType, size) { assert(lp == null); assert(size >= 0); lp = new newType[size]; assert(lp != null); }

#define jgeDelete(lp) { if(lp != null){ delete lp; lp = null; } }
#define jgeDeleteArray(lp) { if(lp != null){ delete[] lp; lp = null; } }
#define jgeReleaseCom(lpCom) { if(lpCom != null){ lpCom->Release(); lpCom = null; } }

#define jgeMalloc(lp, numBytes, lpType) { assert(lp == null); lp = (lpType)malloc(numBytes); assert(lp != null); memset(lp, 0, numBytes); }
// The magic value 0x78 can be use to mark the memory has been released.
#define jgeFree(lp) { if(lp != null) { *(uchar*)lp = 0x78; free(lp); lp = null; } }
#define jgeMemCpy(lpSrc, lpDest, numBytes) { if(lpSrc != lpDest){ assert(lpSrc != null); assert(lpDest != null); memcpy(lpDest, lpSrc, numBytes); } }
#define jgeZeroMem(lp, numBytes) { if(lp != null){ memset(lp, 0, numBytes); } }

namespace jge
{
	class JGE_DLL Buffer
	{
	public:
		Buffer(const char* lpBuffer, uint numBytes);
		~Buffer();

		inline const char* getBufferPointer() { return m_lpBuffer; }
		inline uint getNumBytes() { return m_numBytes; }

	private:
		Buffer(const Buffer& value);
		Buffer& operator=(const Buffer& value);

		char* m_lpBuffer;
		uint m_numBytes;
	};
}