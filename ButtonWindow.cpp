// ButtonWindow.cpp

#include "ButtonWindow.h"

// Global variables
static HWND g_hWndButton;

BOOL IsButtonWindow( HWND hWnd )
{
	// See if supplied window is button window
	return( hWnd == g_hWndButton );

} // End of function IsButtonWindow

BOOL ButtonWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create button window
	g_hWndButton = CreateWindowEx( BUTTON_WINDOW_EXTENDED_STYLE, BUTTON_WINDOW_CLASS_NAME, BUTTON_WINDOW_TEXT, BUTTON_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )BUTTON_WINDOW_ID, hInstance, NULL );

	// Ensure that button window was created
	if( g_hWndButton )
	{
		// Successfully created button window

		// Update return value
		bResult = TRUE;

	} // End of successfully created button window
	return bResult;

} // End of function ButtonWindowCreate

BOOL ButtonWindowGetRect( LPRECT lpRect )
{
	// Get button window rect
	return GetWindowRect( g_hWndButton, lpRect );

} // End of function ButtonWindowGetRect

BOOL ButtonWindowHandleCommandMessage( WPARAM wParam, LPARAM, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	// Select button window notification code
	switch( HIWORD( wParam ) )
	{
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for button window notification code

	return bResult;

} // End of function ButtonWindowHandleCommandMessage

BOOL ButtonWindowHandleNotifyMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	LPNMHDR lpNmhdr;

	// Get notify message handler
	lpNmhdr = ( ( LPNMHDR )lParam );

	// Select button window notification code
	switch( lpNmhdr->code )
	{
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for button window notification code

	return bResult;

} // End of function ButtonWindowHandleNotifyMessage

BOOL ButtonWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move button window
	return MoveWindow( g_hWndButton, nX, nY, nWidth, nHeight, bRepaint );

} // End of function ButtonWindowMove

HWND ButtonWindowSetFocus()
{
	// Focus on button window
	return SetFocus( g_hWndButton );

} // End of function ButtonWindowSetFocus

void ButtonWindowSetFont( HFONT hFont )
{
	// Set button window font
	SendMessage( g_hWndButton, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function ButtonWindowSetFont
