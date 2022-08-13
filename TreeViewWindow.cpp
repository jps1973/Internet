// TreeViewWindow.cpp

#include "TreeViewWindow.h"

void TreeViewWindowAddTag( HWND hWndTreeView, LPCTSTR lpszTag, LPCTSTR lpszParentUrl )
{
	LPTSTR lpszEndOfTagName;

	// Allocate string memory
	LPTSTR lpszTagName = new char[ lstrlen( lpszTag ) ];

	// Store tag name
	lstrcpy( lpszTagName, ( lpszTag + sizeof( char ) ) );

	// Find end of tag name
	lpszEndOfTagName = strchr( lpszTagName, ASCII_SPACE_CHARACTER );

	// Ensure that end of tag name was found
	if( lpszEndOfTagName )
	{
		// Successfully found end of tag name

		// Terminate tag name
		lpszEndOfTagName[ 0 ] = ( char )NULL;

		// See if this is an anchor tag
		if( lstrcmpi( lpszTagName, HTML_TAG_TYPE_ANCHOR ) == 0 )
		{
			// This is an anchor tag

			// Add tag
			TreeViewWindowAddTag( hWndTreeView, lpszTag, TREE_VIEW_WINDOW_ANCHOR_TAG_HEADER, HTML_TAG_ATTRIBUTE_HREF, lpszParentUrl );

		} // End of this is an anchor tag
		else if( lstrcmpi( lpszTagName, HTML_TAG_TYPE_IMAGE ) == 0 )
		{
			// This is an image tag

			// Add tag
			TreeViewWindowAddTag( hWndTreeView, lpszTag, TREE_VIEW_WINDOW_IMAGE_TAG_HEADER, HTML_TAG_ATTRIBUTE_SOURCE, lpszParentUrl );

		} // End of this is an image tag

	} // End of successfully found end of tag name

	// Free stringmemory
	delete [] lpszTagName;

} // End of function TreeViewWindowAddTag

void TreeViewWindowAddTag( HWND hWndTreeView, LPCTSTR lpszTag, LPCTSTR lpszHeader, LPCTSTR lpszAttributeName, LPCTSTR lpszParentUrl )
{
	// Allocate string memory
	LPTSTR lpszAttributeValue = new char[ STRING_LENGTH ];

	// Get attribute
	if( HtmlGetAttribute( lpszTag, lpszAttributeName, lpszAttributeValue ) )
	{
		// Successfully got attribute
		TVINSERTSTRUCT tvis;
		HTREEITEM htiHeader;

		// Allocate string memory
		LPTSTR lpszAbsoluteUrl = new char[ STRING_LENGTH ];

		// Get absolute url from attribute
		HtmlGetAbsoluteUrl( lpszAttributeValue, lpszParentUrl, lpszAbsoluteUrl );

		// Get header item
		htiHeader = TreeViewWindowGetHeader( hWndTreeView, lpszHeader );

		// Clear tree view insert structure
		ZeroMemory( &tvis, ( sizeof( tvis ) ) );

		// Initialise tree view insert structure
		tvis.hParent		= htiHeader;
		tvis.hInsertAfter	= TVI_SORT;
		tvis.item.mask		= TVIF_TEXT;
		tvis.item.pszText	= ( LPTSTR )lpszAbsoluteUrl;

		// Add tag to tree view window
		SendMessage( hWndTreeView, TVM_INSERTITEM, ( WPARAM )0, ( LPARAM )&tvis );

	} // End of successfully got attribute

	// Free string memory
	delete [] lpszAttributeValue;

} // End of function TreeViewWindowAddTag

HWND TreeViewWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	HWND hWndTreeView = NULL;

	// Create tree view window
	hWndTreeView = CreateWindowEx( TREE_VIEW_WINDOW_EXTENDED_STYLE, TREE_VIEW_WINDOW_CLASS, TREE_VIEW_WINDOW_TEXT, TREE_VIEW_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that tree view window was created
	if( hWndTreeView )
	{
		// Successfully created tree view window

	} // End of successfully created tree view window

	return hWndTreeView;

} // End of function TreeViewWindowCreate

HTREEITEM TreeViewWindowGetHeader( HWND hWndTreeView, LPCTSTR lpszHeader )
{
	HTREEITEM htiHeader = NULL;

	HTREEITEM htiChild;
	TVITEM tvi;

	// Allocate string memory
	LPTSTR lpszChild = new char[ STRING_LENGTH ];

	// Clear tree view item structure
	ZeroMemory( &tvi, sizeof( tvi ) );

	// Initialise tree view item structure
	tvi.mask		= TVIF_TEXT;
	tvi.pszText		= lpszChild;
	tvi.cchTextMax	= STRING_LENGTH;

	// First of all, we will attempt to find the header item
	// If it is not found, we will insert a header

	// Get first child item
	htiChild = ( HTREEITEM )SendMessage( hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_CHILD, ( LPARAM )TVI_ROOT );

	// Loop through child items
	while( htiChild )
	{
		// Update tree view item structure for child item
		tvi.hItem = htiChild;

		// Get child item text
		SendMessage( hWndTreeView, TVM_GETITEM, ( WPARAM )NULL, ( LPARAM )&tvi );

		// See if child item is what we want
		if( lstrcmpi( lpszChild, lpszHeader ) == 0 )
		{
			// Child item is what we want

			// Update return value
			htiHeader = htiChild;

			// Force exit from loop
			htiChild = NULL;

		} // End of child item is what we want
		else
		{
			// Child item is not what we want

			// Get next child item
			htiChild = ( HTREEITEM )SendMessage( hWndTreeView, TVM_GETNEXTITEM, ( WPARAM )TVGN_NEXT, ( LPARAM )htiChild );

		} // End of child item is not what we want

	}; // End of loop through child items

	// Ensure that head was found
	if( htiHeader == NULL )
	{
		// Header was not found
		TV_INSERTSTRUCT tvis;

		// Clear tree view insert structure
		ZeroMemory( &tvis, sizeof( tvis ) );

		// Initialise tree view insert structure
		tvis.hParent		= TVI_ROOT;
		tvis.hInsertAfter	= TVI_SORT;
		tvis.item.mask		= TVIF_TEXT;
		tvis.item.pszText	= ( LPTSTR )lpszHeader;

		// Insert header
		htiHeader = ( HTREEITEM )SendMessage( hWndTreeView, TVM_INSERTITEM, ( WPARAM )NULL, ( LPARAM )&tvis );

	} // End of header was not found

	// Free string memory
	delete [] lpszChild;

	return htiHeader;

} // End of function TreeViewWindowGetHeader