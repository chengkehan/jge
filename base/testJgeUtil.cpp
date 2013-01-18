#include "testJgeUtil.h"

extern void testMainJgeUtil()
{
	testJgeNewArgs();
	testJgeTrace();
}

void testJgeNewArgs()
{
	struct ClassA
	{
		ClassA(float a)
		{

		}
	};

	ClassA* lpClassA = null;
	jgeNewArgs1(lpClassA, ClassA, 1.0f);
	jgeDelete(lpClassA);
}

void testJgeTrace()
{
	jgeTrace1("%s\n", "testJgeTrace");
}