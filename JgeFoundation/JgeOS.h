#pragma once

#include "JgeCommon.h"

#define jgeHWND HWND
#define jgeHINSTANCE HINSTANCE
#define jgeWNDPROC WNDPROC
#define JgeLRESULT LRESULT
#define jgeCALLBACK CALLBACK
#define jgeWPARAM WPARAM
#define jgeLPARAM LPARAM
#define jgeWNDCLASS WNDCLASS
#define jgeCS_HREDRAW CS_HREDRAW
#define jgeCS_VREDRAW CS_VREDRAW
#define jgeLoadIcon LoadIcon
#define jgeLoadCursor LoadCursor
#define jgeIDI_APPLICATION IDI_APPLICATION
#define jgeIDC_ARROW IDC_ARROW
#define jgeHBRUSH HBRUSH
#define jgeWHITE_BRUSH WHITE_BRUSH
#define jgeRegisterClass RegisterClass
#define jgeDefWindowProc DefWindowProc
#define jgeGetSystemMetrics GetSystemMetrics
#define jgeSM_CXFIXEDFRAME SM_CXFIXEDFRAME
#define jgeSM_CYFIXEDFRAME SM_CYFIXEDFRAME
#define jgeSM_CYCAPTION SM_CYCAPTION
#define jgeSM_CXSCREEN SM_CXSCREEN
#define jgeSM_CYSCREEN SM_CYSCREEN

JGE_DLL void jgeGetWindowAdjustedSize(uint widthSrc, uint heightSrc, int* lpXResult = null, int* lpYResult = null, uint* lpWidthResult = null, uint* lpHeightResult = null);