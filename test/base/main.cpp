#include <Windows.h>
#include "jge.h"

#pragma comment(lib, "base.lib")

using namespace jge;

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	if(!init(hInstance))
	{
		return 0;
	}
	run();

	return 0;
}