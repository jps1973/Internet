// TreeViewWindow.cpp

#include "TreeViewWindow.h"

void TreeViewWindowAddTag( HWND hWndTreeView, LPCTSTR lpszTag )
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
		if( lstrcmpi( lpszTagName, HTML_ANCHOR_TAG_NAME ) == 0 )
		{
			// This is an anchor tag
			TVINSERTSTRUCT tvis;

			// Clear tree view insert structure
			ZeroMemory( &tvis, ( sizeof( tvis ) ) );

			// Initialise tree view insert structure
			tvis.hParent		= TVI_ROOT;
			tvis.hInsertAfter	= TVI_SORT;
			tvis.item.mask		= TVIF_TEXT;
			tvis.item.pszText	= ( LPTSTR )lpszTag;
			
			// Add tag to tree view window
			SendMessage( hWndTreeView, TVM_INSERTITEM, ( WPARAM )0, ( LPARAM )&tvis );

		} // End of this is an anchor tag

	} // End of successfully found end of tag name

	// Free stringmemory
	delete [] lpszTagName;

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
