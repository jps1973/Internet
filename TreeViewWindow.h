// TreeViewWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Common.h"

#include "Html.h"

#define STRING_LENGTH															2048

#define TREE_VIEW_WINDOW_EXTENDED_STYLE											0
#define TREE_VIEW_WINDOW_CLASS													WC_TREEVIEW
#define TREE_VIEW_WINDOW_TEXT													NULL
#define TREE_VIEW_WINDOW_STYLE													( WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES )

void TreeViewWindowAddTag( HWND hWndTreeView, LPCTSTR lpszTag );

HWND TreeViewWindowCreate( HWND hWndParent, HINSTANCE hInstance );
