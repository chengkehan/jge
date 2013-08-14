#pragma once

#define null 0
#define uint unsigned int

#define jgeAssert(c) assert(c)

#define jgeTrace1(format, value1) { char buffer[1024]; sprintf_s(buffer, 1024, format, value1); OutputDebugStringA(buffer); }
#define jgeTrace2(format, value1, value2) { char buffer[1024]; sprintf_s(buffer, 1024, format, value1, value2); OutputDebugStringA(buffer); }
#define jgeTrace3(format, value1, value2, value3) { char buffer[1024]; sprintf_s(buffer, 1024, format, value1, value2, value3); OutputDebugStringA(buffer); }