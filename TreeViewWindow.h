// TreeViewWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define TREE_VIEW_WINDOW_CLASS_NAME												WC_TREEVIEW

#define TREE_VIEW_WINDOW_EXTENDED_STYLE											0
#define TREE_VIEW_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | WS_BORDER | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT )
#define TREE_VIEW_WINDOW_TEXT													NULL

#define TREE_VIEW_WINDOW_PROCESS_GROUP_STATUS_MESSAGE_FORMAT_STRING				"Successfully processed %d items in group"

BOOL IsTreeViewWindow( HWND hWnd );

BOOL TreeViewWindowCreate( HWND hWndParent, HINSTANCE hInstance );

BOOL TreeViewWindowGetItemText( HTREEITEM htiItem, LPTSTR lpszItemText );

BOOL TreeViewWindowGetItemText( LPTSTR lpszItemText );

BOOL TreeViewWindowGetRect( LPRECT lpRect );

HTREEITEM TreeViewWindowGetSelected();

BOOL TreeViewWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) );

HTREEITEM TreeViewWindowInsertItem( LPCTSTR lpszItemText, HTREEITEM htiParent = TVI_ROOT, HTREEITEM htiInsertAfter = TVI_SORT );

HTREEITEM TreeViewWindowInsertUniqueItem( LPCTSTR lpszItemText, HTREEITEM htiParent = TVI_ROOT, HTREEITEM htiInsertAfter = TVI_SORT );

BOOL TreeViewWindowIsGroup( HTREEITEM hti );

BOOL TreeViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

int TreeViewWindowProcessGroup( BOOL( *lpProcessTagFunction )( LPCTSTR lpszTag ) );

int TreeViewWindowProcessGroup( HTREEITEM htiParent, BOOL( *lpProcessTagFunction )( LPCTSTR lpszTag ) );

HWND TreeViewWindowSetFocus();

void TreeViewWindowSetFont( HFONT hFont );

BOOL TreeViewWindowUpdate();
