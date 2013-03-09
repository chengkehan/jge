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

	JGECallbackThis<ClassA, void> cb = jgeCallbackThis(&classA, &ClassA::funcA);
	JGEAbstractCallback<void>* lpcb = &cb;
	cb();

	JGECallbackStd<void> cb2 = jgeCallbackStd(&funcB);
	JGEAbstractCallback<void>* lpcb2 = &cb2;
	cb2();

	JGEAbstractCallback<void>* lpcallback;
	lpcallback = lpcb;
	(*lpcallback)();
	lpcallback = lpcb2;
	(*lpcallback)();

	return 0;
}