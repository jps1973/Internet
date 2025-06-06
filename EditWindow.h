// EditWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define EDIT_WINDOW_CLASS_NAME													WC_EDIT

#define EDIT_WINDOW_EXTENDED_STYLE												WS_EX_WINDOWEDGE
#define EDIT_WINDOW_STYLE														( WS_CHILD | WS_VISIBLE | WS_BORDER )

BOOL IsEditWindow( HWND hWnd );

BOOL EditWindowCreate( HWND hWndParent, HINSTANCE hInstance, LPCTSTR lpszInitialText );

BOOL EditWindowGetRect( LPRECT lpRect );

int EditWindowGetText( LPTSTR lpszText, DWORD dwMaxTextLength = STRING_LENGTH );

BOOL EditWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpChangeFunction )( DWORD dwTextLength ) );

BOOL EditWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

HWND EditWindowSetFocus();

void EditWindowSelect( int nStart = 0, int nEnd = -1 );

void EditWindowSetFont( HFONT hFont );
