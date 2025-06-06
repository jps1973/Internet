// ListBoxWindow.cpp

#include "ListBoxWindow.h"

// Global variables
static HWND g_hWndListBox;

BOOL IsListBoxWindow( HWND hWnd )
{
	// See if supplied window is list box window
	return( hWnd == g_hWndListBox );

} // End of function IsListBoxWindow

int ListBoxWindowAddString( LPCTSTR lpszString )
{
	// Add string to list box window
	return SendMessage( g_hWndListBox, LB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszString );

} // End of function ListBoxWindowAddString

int ListBoxWindowAddStringEx( LPCTSTR lpszString )
{
	int nResult;

	// Add string to list box window
	nResult = SendMessage( g_hWndListBox, LB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszString );

	// Ensure that string was added to list box window
	if( ( nResult != LB_ERR ) && ( nResult != LB_ERRSPACE ) )
	{
		// Successfully added string to list box window

		// Update list box window
		UpdateWindow( g_hWndListBox );

	} // End of successfully added string to list box window

	return nResult;

} // End of function ListBoxWindowAddStringEx

BOOL ListBoxWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create list box window
	g_hWndListBox = CreateWindowEx( LIST_BOX_WINDOW_EXTENDED_STYLE, LIST_BOX_WINDOW_CLASS_NAME, LIST_BOX_WINDOW_TEXT, LIST_BOX_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list box window was created
	if( g_hWndListBox )
	{
		// Successfully created list box window

		// Update return value
		bResult = TRUE;

	} // End of successfully created list box window
	return bResult;

} // End of function ListBoxWindowCreate

BOOL ListBoxWindowGetRect( LPRECT lpRect )
{
	// Get list box window rect
	return GetWindowRect( g_hWndListBox, lpRect );

} // End of function ListBoxWindowGetRect

BOOL ListBoxWindowHandleCommandMessage( WPARAM wParam, LPARAM, BOOL( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	// Select list box window notification code
	switch( HIWORD( wParam ) )
	{
		case LBN_DBLCLK:
		{
			// A list box window double click notification code
			int nSelectedItem;

			// Allocate string memory
			LPTSTR lpszSelected = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get selected item
			nSelectedItem = SendMessage( g_hWndListBox, LB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

			// Get selected item text
			if( SendMessage( g_hWndListBox, LB_GETTEXT, ( WPARAM )nSelectedItem, ( LPARAM )lpszSelected ) )
			{
				// Successfully got selected item text

				// Call double click function
				( *lpDoubleClickFunction )( lpszSelected );

			} // End of successfully got selected item text

			// Free string memory
			delete [] lpszSelected;

			// Break out of switch
			break;

		} // End of a list box window double click notification code
		case LBN_SELCHANGE:
		{
			// A list box window selection change notification code
			int nSelectedItem;

			// Allocate string memory
			LPTSTR lpszSelected = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get selected item
			nSelectedItem = SendMessage( g_hWndListBox, LB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

			// Get selected item text
			if( SendMessage( g_hWndListBox, LB_GETTEXT, ( WPARAM )nSelectedItem, ( LPARAM )lpszSelected ) )
			{
				// Successfully got selected item text

				// Show selected item text on status bar window
				( *lpStatusFunction )( lpszSelected );

			} // End of successfully got selected item text

			// Free string memory
			delete [] lpszSelected;

			// Break out of switch
			break;

		} // End of a list box window selection change notification code
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for list box window notification code

	return bResult;

} // End of function ListBoxWindowHandleCommandMessage

BOOL ListBoxWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move list box window
	return MoveWindow( g_hWndListBox, nX, nY, nWidth, nHeight, bRepaint );

} // End of function ListBoxWindowMove

HWND ListBoxWindowSetFocus()
{
	// Focus on list box window
	return SetFocus( g_hWndListBox );

} // End of function ListBoxWindowSetFocus

void ListBoxWindowSetFont( HFONT hFont )
{
	// Set list box window font
	SendMessage( g_hWndListBox, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function ListBoxWindowSetFont
