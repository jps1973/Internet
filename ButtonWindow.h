// ButtonWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define BUTTON_WINDOW_CLASS_NAME												WC_BUTTON

#define BUTTON_WINDOW_EXTENDED_STYLE											WS_EX_WINDOWEDGE
#define BUTTON_WINDOW_STYLE														( WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT )
#define BUTTON_WINDOW_TEXT														"Go"

#define BUTTON_WINDOW_ID														WM_USER

#define BUTTON_WINDOW_WIDTH														30
#define BUTTON_WINDOW_HEIGHT													25

BOOL IsButtonWindow( HWND hWnd );

BOOL ButtonWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL ButtonWindowEnable( BOOL bState );

BOOL ButtonWindowGetRect( LPRECT lpRect );

BOOL ButtonWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

HWND ButtonWindowSetFocus();

void ButtonWindowSetFont( HFONT hFont );
