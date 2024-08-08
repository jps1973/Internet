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

BOOL TreeViewWindowGetItemText( LPTSTR lpszItemText )
{
	BOOL bResult;

	HTREEITEM htiSelected;

	// Get selected tree item
	htiSelected = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_CARET, ( LPARAM )0 );

	// Ensure that selected tree item was got
	if( htiSelected )
	{
		// Successfully got selected tree item

		// Get selected item text
		if( TreeViewWindowGetItemText( htiSelected, lpszItemText ) )
		{
			// Successfuly got selected item text

			// Update return value
			bResult = TRUE;

		} // End of successfuly got selected item text

	} // End of successfully got selected tree item

	return bResult;

} // End of function TreeViewWindowGetItemText

BOOL TreeViewWindowGetRect( LPRECT lpRect )
{
	// Get tree view window rect
	return ::GetWindowRect( g_hWndTreeView, lpRect );

} // End of function TreeViewWindowGetRect

HTREEITEM TreeViewWindowGetSelected()
{
	// Get selected tree item
	return ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_CARET, ( LPARAM )0 );

} // End of function TreeViewWindowGetSelected

BOOL TreeViewWindowHandleNotifyMessage( WPARAM, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	// Select tree view window notification code
	switch( ( ( LPNMHDR )lParam )->code )
	{
		case NM_DBLCLK:
		{
			// A Double click notification code
			HTREEITEM htiSelected;

			// Get selected tree item
			htiSelected = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_CARET, ( LPARAM )0 );

			// Ensure that selected tree item was got
			if( htiSelected )
			{
				// Successfully got selected tree item

				// See if this is a top level tree item
				if( SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_PARENT, ( LPARAM )htiSelected ) )
				{
					// This is not a top level tree item

					// Allocate string memory
					LPTSTR lpszItemText = new char[ STRING_LENGTH ];

					// Get item text
					if( TreeViewWindowGetItemText( htiSelected, lpszItemText ) )
					{
						// Successfully got item text

						// Call double click function
						( *lpDoubleClickFunction )( lpszItemText );

						// Update return value
						bResult = TRUE;

					} // End of successfully got item text

					// Free string memory
					delete [] lpszItemText;

				} // End of this is not a top level tree item

			} // End of successfully got selected tree item

			// Break out of switch
			break;

		} // End of a double click notification code
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

HTREEITEM TreeViewWindowInsertUniqueItem( LPCTSTR lpszItemText, HTREEITEM htiParent, HTREEITEM htiInsertAfter )
{
	HTREEITEM htiResult = NULL;

	TVITEM tvItem;

	// Allocate string memory
	LPTSTR lpszCurrentItemText = new char[ STRING_LENGTH ];

	// Clear tree view item structure
	ZeroMemory( &tvItem, sizeof( tvItem ) );

	// Initialise tree view item structure
	tvItem.mask			= TVIF_TEXT;
	tvItem.pszText		= lpszCurrentItemText;
	tvItem.cchTextMax	= STRING_LENGTH;

	// Get first tree item
	tvItem.hItem = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_CHILD, ( LPARAM )htiParent );

	// Loop through all tree items
	while( tvItem.hItem )
	{

		// Get current item text
		if( SendMessage( g_hWndTreeView, TVM_GETITEM, ( WPARAM )0, ( LPARAM )&tvItem ) )
		{
			// Successfully got current item text

			// See if current item text matches item
			if( lstrcmpi( lpszCurrentItemText, lpszItemText ) == 0 )
			{
				// Current item text matches item

				// Update return value
				htiResult = tvItem.hItem;

				// Force exit from loop
				tvItem.hItem = NULL;

			} // End of current item text matches item
			else
			{
				// Current item text does not match item

				// Get nect tree item
				tvItem.hItem = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_NEXT, ( LPARAM )tvItem.hItem );

			} // End of current item text does not match item

		} // End of successfully got current item text
		else
		{
			// Unable to get current item text

			// Force exit from loop
			tvItem.hItem = NULL;

		} // End of unable to get current item text

	}; // End of loop through all tree items

	// Ensure that tree item is valid
	if( htiResult == NULL )
	{
		// Tree item is not valid

		// Insert tree item
		htiResult = TreeViewWindowInsertItem( lpszItemText, htiParent, htiInsertAfter );

	} // End of tree item is not valid

	// Free string memory
	delete [] lpszCurrentItemText;

	return htiResult;

} // End of function TreeViewWindowInsertUniqueItem

BOOL TreeViewWindowIsGroup( HTREEITEM hti )
{
	BOOL bResult = TRUE; // Assume item is a group

	// See if item has a parent
	if( SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_PARENT, ( LPARAM )hti ) )
	{
		// Item has a parent (so is a tag, not a group)

		// Update return value
		bResult = FALSE;

	} // End of item has a parent (so is a tag, not a group)

	return bResult;

} // End of function TreeViewWindowIsGroup
BOOL TreeViewWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move tree view window
	return ::MoveWindow( g_hWndTreeView, nX, nY, nWidth, nHeight, bRepaint );

} // End of function TreeViewWindowMove

int TreeViewWindowProcessGroup( BOOL( *lpProcessTagFunction )( LPCTSTR lpszTag ) )
{
	int nResult = 0;

	HTREEITEM htiSelected;

	// Get selected tree item
	htiSelected = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_CARET, ( LPARAM )0 );

	// Ensure that selected tree item was got
	if( htiSelected )
	{
		// Successfully got selected tree item

		// Process group
		nResult = TreeViewWindowProcessGroup( htiSelected, lpProcessTagFunction );

	} // End of successfully got selected tree item

	return nResult;

} // End of function TreeViewWindowProcessGroup

int TreeViewWindowProcessGroup( HTREEITEM htiParent, BOOL( *lpProcessTagFunction )( LPCTSTR lpszTag ) )
{
	int nResult = 0;

	TVITEM tvItem;

	// Allocate string memory
	LPTSTR lpszItemText = new char[ STRING_LENGTH ];

	// Clear tree view item structure
	ZeroMemory( &tvItem, sizeof( tvItem ) );

	// Initialise tree view item structure
	tvItem.mask			= TVIF_TEXT;
	tvItem.pszText		= lpszItemText;
	tvItem.cchTextMax	= STRING_LENGTH;

	// Get first tree item
	tvItem.hItem = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_CHILD, ( LPARAM )htiParent );

	// Loop through all tree items
	while( tvItem.hItem )
	{

		// Get current item text
		if( SendMessage( g_hWndTreeView, TVM_GETITEM, ( WPARAM )0, ( LPARAM )&tvItem ) )
		{
			// Successfully got current item text

			// Process item
			if( ( *lpProcessTagFunction )( lpszItemText ) )
			{
				// Successfully processed item

				// Update return value
				nResult ++;

			} // End of successfully processed item

			// Get nect tree item
			tvItem.hItem = ( HTREEITEM )SendMessage( g_hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_NEXT, ( LPARAM )tvItem.hItem );

		} // End of successfully got current item text
		else
		{
			// Unable to get current item text

			// Force exit from loop
			tvItem.hItem = NULL;

		} // End of unable to get current item text

	}; // End of loop through all tree items

	// Free string memory
	delete [] lpszItemText;

	return nResult;

} // End of function TreeViewWindowProcessGroup

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

BOOL TreeViewWindowUpdate()
{
	BOOL bResult;

	// Invalidate entire tree view window
	InvalidateRect( g_hWndTreeView, NULL, TRUE );

	// Update tree view window
	bResult = UpdateWindow( g_hWndTreeView );

	return bResult;

} // End of function TreeViewWindowUpdate