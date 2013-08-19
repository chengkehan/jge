#pragma once

#include "JgeCommon.h"

#define jgeHWND HWND
#define jgeHINSTANCE HINSTANCE
#define jgeWNDPROC WNDPROC
#define jgeLRESULT LRESULT
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
#define jgeRECT RECT
#define jgeCreateWindowEx CreateWindowEx
#define jgeWS_POPUP WS_POPUP
#define jgeWS_CAPTION WS_CAPTION
#define jgeWS_MINIMIZEBOX WS_MINIMIZEBOX
#define jgeWS_VISIBLE WS_VISIBLE
#define jgeWS_EX_TOPMOST WS_EX_TOPMOST
#define jgeSW_SHOW SW_SHOW
#define jgeMSG MSG

JGE_DLL void jgeGetWindowAdjustedSize(uint widthSrc, uint heightSrc, int* lpXResult = null, int* lpYResult = null, uint* lpWidthResult = null, uint* lpHeightResult = null);