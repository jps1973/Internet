// TreeViewWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define TREE_VIEW_WINDOW_CLASS_NAME												WC_TREEVIEW

#define TREE_VIEW_WINDOW_EXTENDED_STYLE											0
#define TREE_VIEW_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT )
#define TREE_VIEW_WINDOW_TEXT													NULL

BOOL IsTreeViewWindow( HWND hWnd );

HTREEITEM TreeViewWindowAddString( LPCTSTR lpszString, HTREEITEM htiParent = TVI_ROOT, HTREEITEM htiPrevious = TVI_LAST );

BOOL TreeViewWindowCreate( HWND hWndParent, HINSTANCE hInstance );

HTREEITEM TreeViewWindowFindItem( LPCTSTR lpszRequiredItemText, HTREEITEM htiParent = TVI_ROOT );

BOOL TreeViewWindowGetRect( LPRECT lpRect );

BOOL TreeViewWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) );

BOOL TreeViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

BOOL TreeViewWindowProcessSelected( void( *lpProcessFunction )( LPCTSTR lpszItemText ) );

HWND TreeViewWindowSetFocus();

void TreeViewWindowSetFont( HFONT hFont );
