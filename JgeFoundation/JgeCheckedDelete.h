#pragma once

#include "JgeDefinition.h"

namespace jge
{
	template<class T>
	inline void checkedDelete(T* lp)
	{
		if(lp != null)
		{
			typedef char type_must_be_complete[sizeof(T)? 1: -1];
			(void)sizeof(type_must_be_complete);
			delete lp;
		}
	}

	template<class T> 
	inline void checkedDeleteArray(T * lp)
	{
		if(lp != null)
		{
			typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
			(void) sizeof(type_must_be_complete);
			delete[] lp;
		}
	}
}