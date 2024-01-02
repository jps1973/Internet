// EditWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define EDIT_WINDOW_CLASS_NAME													WC_EDIT

#define EDIT_WINDOW_EXTENDED_STYLE												0
#define EDIT_WINDOW_STYLE														( WS_CHILD | WS_VISIBLE | WS_BORDER )
#define EDIT_WINDOW_TEXT														"https://www.google.com"

BOOL IsEditWindow( HWND hWnd );

BOOL EditWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL EditWindowGetRect( LPRECT lpRect );

int EditWindowGetText( LPTSTR lpszText, int nMaxCount = STRING_LENGTH );

BOOL EditWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, void( *lpUpdateFunction )( int nTextLength ) );

BOOL EditWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

HWND EditWindowSetFocus();

void EditWindowSelect( int nStart = 0, int nEnd = -1 );

void EditWindowSetFont( HFONT hFont );
