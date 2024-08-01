// TreeViewWindow.cpp

#include "TreeViewWindow.h"

// Global variables
static HWND g_hWndTreeView;

BOOL IsTreeViewWindow( HWND hWnd )
{
	// See if supplied window is tree view window
	return( hWnd == g_hWndTreeView );

} // End of function IsTreeViewWindow

BOOL TreeViewWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create tree view window
	g_hWndTreeView = ::CreateWindowEx( TREE_VIEW_WINDOW_EXTENDED_STYLE, TREE_VIEW_WINDOW_CLASS_NAME, TREE_VIEW_WINDOW_TEXT, TREE_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that tree view window was created
	if( g_hWndTreeView )
	{
		// Successfully created tree view window

		// Update return value
		bResult = TRUE;

	} // End of successfully created tree view window
	return bResult;

} // End of function TreeViewWindowCreate

BOOL TreeViewWindowGetItemText( HTREEITEM htiItem, LPTSTR lpszItemText )
{
	BOOL bResult;

	TVITEM tvItem;

	// Clear tree view item structure
	ZeroMemory( &tvItem, sizeof( tvItem ) );

	// Initialise tree view item structure
	tvItem.mask			= TVIF_TEXT;
	tvItem.pszText		= lpszItemText;
	tvItem.cchTextMax	= STRING_LENGTH;
	tvItem.hItem		= htiItem;

	// Get item text
	bResult = SendMessage( g_hWndTreeView, TVM_GETITEM, ( WPARAM )0, ( LPARAM )&tvItem );

	return bResult;

} // End of function TreeViewWindowGetItemText

BOOL TreeViewWindowGetRect( LPRECT lpRect )
{
	// Get tree view window rect
	return ::GetWindowRect( g_hWndTreeView, lpRect );

} // End of function TreeViewWindowGetRect

BOOL TreeViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	// Select tree view window notification code
	switch( ( ( LPNMHDR )lParam )->code )
	{
		case NM_RCLICK:
		{
			// A right click notification code
			HTREEITEM htiHighlighted;

			// Get highlighted tree item
			htiHighlighted = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_DROPHILITE, ( LPARAM )0 );

			// Ensure that highlighted tree item was got
			if( htiHighlighted )
			{
				// Successfully got highlighted tree item

				// Select highlighted tree item
				SendMessage( g_hWndTreeView, TVM_SELECTITEM, ( WPARAM )TVGN_CARET, ( LPARAM )htiHighlighted );

			} // End of successfully got highlighted tree item

			// Break out of switch
			break;

		} // End of a right click notification code
		case TVN_SELCHANGED:
		{
			// A tree view selection changed notification code
			LPNMTREEVIEW lpNmTreeView;

			// Allocate string memory
			LPTSTR lpszItemText = new char[ STRING_LENGTH ];

			// Get tree view notification message handler
			lpNmTreeView = ( LPNMTREEVIEW )lParam;

			// Get item text
			if( TreeViewWindowGetItemText( lpNmTreeView->itemNew.hItem, lpszItemText ) )
			{
				// Successfully got item text

				// Call selection changed function
				( *lpSelectionChangedFunction )( lpszItemText );

				// Update return value
				bResult = TRUE;

			} // End of successfully got item text

			// Free string memory
			delete [] lpszItemText;

			// Break out of switch
			break;

		} // End of a tree view selection changed notification code
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for tree view window notification code

	return bResult;

} // End of function TreeViewWindowHandleNotifyMessage

HTREEITEM TreeViewWindowInsertItem( LPCTSTR lpszItemText, HTREEITEM htiParent, HTREEITEM htiInsertAfter )
{
	HTREEITEM htiResult;

	TVINSERTSTRUCT treeViewInsert;

	// Clear tree view insert structure
	ZeroMemory( &treeViewInsert, sizeof( treeViewInsert ) );

	// Initialise tree view insert structure
	treeViewInsert.hParent		= htiParent;
	treeViewInsert.hInsertAfter	= htiInsertAfter;
	treeViewInsert.item.mask	= TVIF_TEXT;
	treeViewInsert.item.pszText	= ( LPTSTR )lpszItemText;

	// Insert item
	htiResult = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_INSERTITEM, ( WPARAM )0, ( LPARAM )&treeViewInsert );

	// Update tree view window
	UpdateWindow( g_hWndTreeView );

	return htiResult;

} // End of function TreeViewWindowInsertItem

BOOL TreeViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move tree view window
	return ::MoveWindow( g_hWndTreeView, nX, nY, nWidth, nHeight, bRepaint );

} // End of function TreeViewWindowMove

HWND TreeViewWindowSetFocus()
{
	// Focus on tree view window
	return ::SetFocus( g_hWndTreeView );

} // End of function TreeViewWindowSetFocus

void TreeViewWindowSetFont( HFONT hFont )
{
	// Set tree view window font
	::SendMessage( g_hWndTreeView, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function TreeViewWindowSetFont
