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
#define TREE_VIEW_WINDOW_STYLE													( WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT )

#define TREE_VIEW_WINDOW_ANCHOR_TAG_HEADER										"Anchor"
#define TREE_VIEW_WINDOW_IMAGE_TAG_HEADER										"Image"

void TreeViewWindowAddTag( HWND hWndTreeView, LPCTSTR lpszTag );

void TreeViewWindowAddTag( HWND hWndTreeView, LPCTSTR lpszTag, LPCTSTR lpszHeader, LPCTSTR lpszAttributeName );

HWND TreeViewWindowCreate( HWND hWndParent, HINSTANCE hInstance );

HTREEITEM TreeViewWindowGetHeader( HWND hWndTreeView, LPCTSTR lpszHeader );
