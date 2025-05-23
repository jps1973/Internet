// EditWindow.cpp

#include "EditWindow.h"

// Global variables
static HWND g_hWndEdit;

BOOL IsEditWindow( HWND hWnd )
{
	// See if supplied window is edit window
	return( hWnd == g_hWndEdit );

} // End of function IsEditWindow

BOOL EditWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create edit window
	g_hWndEdit = CreateWindowEx( EDIT_WINDOW_EXTENDED_STYLE, EDIT_WINDOW_CLASS_NAME, EDIT_WINDOW_TEXT, EDIT_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that edit window was created
	if( g_hWndEdit )
	{
		// Successfully created edit window

		// Update return value
		bResult = TRUE;

	} // End of successfully created edit window
	return bResult;

} // End of function EditWindowCreate

BOOL EditWindowGetRect( LPRECT lpRect )
{
	// Get edit window rect
	return GetWindowRect( g_hWndEdit, lpRect );

} // End of function EditWindowGetRect

int EditWindowGetText( LPTSTR lpszText, DWORD dwMaxTextLength )
{
	// Get edit window text
	return SendMessage( g_hWndEdit, WM_GETTEXT, ( WPARAM )dwMaxTextLength, ( LPARAM )lpszText );

} // End of function EditWindowGetText

BOOL EditWindowHandleCommandMessage( WPARAM wParam, LPARAM, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	// Select edit window notification code
	switch( HIWORD( wParam ) )
	{
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for edit window notification code

	return bResult;

} // End of function EditWindowHandleCommandMessage

BOOL EditWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	LPNMHDR lpNmhdr;

	// Get notify message handler
	lpNmhdr = ( ( LPNMHDR )lParam );

	// Select edit window notification code
	switch( lpNmhdr->code )
	{
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for edit window notification code

	return bResult;

} // End of function EditWindowHandleNotifyMessage

BOOL EditWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move edit window
	return MoveWindow( g_hWndEdit, nX, nY, nWidth, nHeight, bRepaint );

} // End of function EditWindowMove

void EditWindowSelect( int nStart, int nEnd )
{
	// Select edit window text
	SendMessage( g_hWndEdit, EM_SETSEL, ( WPARAM )nStart, ( LPARAM )nEnd );

} // End of function EditWindowSelect

HWND EditWindowSetFocus()
{
	// Focus on edit window
	return SetFocus( g_hWndEdit );

} // End of function EditWindowSetFocus

void EditWindowSetFont( HFONT hFont )
{
	// Set edit window font
	SendMessage( g_hWndEdit, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function EditWindowSetFont
