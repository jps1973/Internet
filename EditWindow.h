// EditWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#include "Clipboard.h"

#define EDIT_WINDOW_CLASS_NAME													WC_EDIT

#define EDIT_WINDOW_EXTENDED_STYLE												0
#define EDIT_WINDOW_STYLE														( WS_CHILD | WS_VISIBLE | WS_BORDER )
#define EDIT_WINDOW_TEXT														"https://www.bbc.co.uk/news"

BOOL IsEditWindow( HWND hWnd );

BOOL EditWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL EditWindowGetRect( LPRECT lpRect );

LRESULT EditWindowGetText( LPTSTR lpszText, DWORD dwMaximumTextLength = STRING_LENGTH );

BOOL EditWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, void( *lpUpdateFunction )( int nTextLength ) );

BOOL EditWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

void EditWindowSelect( int nStart = 0, int nEnd = -1 );

HWND EditWindowSetFocus();

void EditWindowSetFont( HFONT hFont );
