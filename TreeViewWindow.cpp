// TreeViewWindow.cpp

#include "TreeViewWindow.h"

// Global variables
HWND g_hWndTreeView;

BOOL IsTreeViewWindow( HWND hWnd )
{
	// See if supplied window is tree view window
	return( hWnd == g_hWndTreeView );

} // End of function IsTreeViewWindow

HTREEITEM TreeViewWindowAddString( LPCTSTR lpszString, HTREEITEM htiParent, HTREEITEM htiPrevious )
{
	HTREEITEM htiResult = NULL;

	TV_INSERTSTRUCT tvis;

	// Clear tree view insert structure
	ZeroMemory( &tvis, sizeof( tvis ) );

	// Initialise tree view insert structure
	tvis.hParent		= htiParent;
	tvis.hInsertAfter	= htiPrevious;
	tvis.item.mask		= TVIF_TEXT;
	tvis.item.pszText	= ( LPTSTR )lpszString;

	// Add string
	htiResult = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_INSERTITEM, ( WPARAM )0, ( LPARAM )&tvis );

	return htiResult;

} // End of function TreeViewWindowAddString

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

BOOL TreeViewWindowGetRect( LPRECT lpRect )
{
	// Get tree view window rect
	return ::GetWindowRect( g_hWndTreeView, lpRect );

} // End of function TreeViewWindowGetRect

BOOL TreeViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	LPNMHDR lpNmHdr;

	// Get notify message handler
	lpNmHdr = ( LPNMHDR )lParam;

	// Select tree view window notification code
	switch( lpNmHdr->code )
	{
		case TVN_SELCHANGED:
		{
			// A tree view window selection changed notification code

			// Call selection changed function for selected item
			TreeViewWindowProcessSelected( lpSelectionChangedFunction );

			// Break out of switch
			break;

		} // End of a tree view window selection changed notification code
		case NM_DBLCLK:
		{
			// A double click notification code

			// Call double click function for selected item
			TreeViewWindowProcessSelected( lpDoubleClickFunction );

			// Break out of switch
			break;

		} // End of a double click notification code
		case NM_RCLICK:
		{
			// A right click notification code

			// Break out of switch
			break;

		} // End of a right click notification code
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

BOOL TreeViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move tree view window
	return ::MoveWindow( g_hWndTreeView, nX, nY, nWidth, nHeight, bRepaint );

} // End of function TreeViewWindowMove

BOOL TreeViewWindowProcessSelected( void( *lpProcessFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	HTREEITEM htiSelected;

	// Get selected tree item
	htiSelected = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_CARET, ( LPARAM )NULL );

	// Ensure that selected tree item was goto
	if( htiSelected )
	{
		// Successfully got selected tree item
		TVITEM tvItem;

		// Allocate string memory
		LPTSTR lpszItemText = new char[ STRING_LENGTH ];

		// Clear tree viev item structure
		ZeroMemory( &tvItem, sizeof( tvItem ) );

		// Update tree view item structure to get text
		tvItem.mask			= TVIF_TEXT;
		tvItem.pszText		= lpszItemText;
		tvItem.cchTextMax	= STRING_LENGTH;
		tvItem.hItem		= htiSelected;

		// Get item text
		if( SendMessage( g_hWndTreeView, TVM_GETITEM, 0, ( LPARAM )&tvItem ) )
		{
			// Successfully got item text

			// Process item
			( *lpProcessFunction )( lpszItemText );

			// Update return value
			bResult = TRUE;

		} // End of successfully got item text

		// Free string memory
		delete [] lpszItemText;

	} // End of successfully got selected tree item

	return bResult;

} // End of function TreeViewWindowProcessSelected

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
