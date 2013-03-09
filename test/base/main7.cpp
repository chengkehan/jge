#include "jge.h"
#include "JGECallback.h"

#pragma comment(lib, "base.lib")

using namespace jge;

struct ClassB
{

};

struct ClassA
{
public:
	void funcA(void* lpData)
	{
		char buffer[256];
		sprintf_s(buffer, 256, "this call\n");
		OutputDebugStringA(buffer);
	}
};

void funcB(void* lpData)
{
	char buffer[256];
	sprintf_s(buffer, 256, "std call\n");
	OutputDebugStringA(buffer);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	ClassA classA;

	JGEAbstractCallback<void>* lpCallback = null;

	lpCallback = jgeCallbackThis(&classA, &ClassA::funcA);
	lpCallback->invoke();
	jgeCallbackRelease(lpCallback);

	lpCallback = jgeCallbackStd(&funcB);
	lpCallback->invoke();
	jgeCallbackRelease(lpCallback);

	return 0;
}