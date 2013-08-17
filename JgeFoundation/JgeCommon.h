#pragma once

#define null 0
#define uint unsigned int
#define uchar unsigned char

#define jgeAssert(c) assert(c)

#define jgeTraceA1(format, value1) { char buffer[1024]; sprintf_s(buffer, 1024, format, value1); OutputDebugStringA(buffer); }
#define jgeTraceA2(format, value1, value2) { char buffer[1024]; sprintf_s(buffer, 1024, format, value1, value2); OutputDebugStringA(buffer); }
#define jgeTraceA3(format, value1, value2, value3) { char buffer[1024]; sprintf_s(buffer, 1024, format, value1, value2, value3); OutputDebugStringA(buffer); }

#define jgeTraceW1(format, value1) { wchar_t buffer[1024]; swprintf_s(buffer, 1024, format, value1); OutputDebugStringW(buffer); }
#define jgeTraceW2(format, value1, value2) { wchar_t buffer[1024]; swprintf_s(buffer, 1024, format, value1, value2); OutputDebugStringW(buffer); }
#define jgeTraceW3(format, value1, value2, value3) { wchar_t buffer[1024]; swprintf_s(buffer, 1024, format, value1, value2, value3); OutputDebugStringW(buffer); }