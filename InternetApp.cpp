// InternetApp.cpp

#include "InternetApp.h"

BOOL ProcessItemFunction( LPCTSTR lpszAttributeValue )
{
	// Add attribute to list box window
	ListBoxWindowAddString( lpszAttributeValue );

	return TRUE;

} // End of function ProcessItemFunction

BOOL DownloadFile( LPCTSTR lpszUrl, LPTSTR lpszLocalFilePath )
{
	BOOL bResult = FALSE;

	// Allocate string memory
	LPTSTR lpszStatusMessage = new char[ STRING_LENGTH ];

	// Format status message
	wsprintf( lpszStatusMessage, INTERNET_DOWNLOADING_STATUS_MESSAGE_FORMAT_STRING, lpszUrl );

	// Show status message on status bar window
	StatusBarWindowSetText( lpszStatusMessage );

	// Download file
	if( InternetDownloadFile( lpszUrl, lpszLocalFilePath ) )
	{
		// Successfully downloaded file

		// Format status message
		wsprintf( lpszStatusMessage, INTERNET_SUCCESSFULLY_DOWNLOADED_STATUS_MESSAGE_FORMAT_STRING, lpszUrl, lpszLocalFilePath );

		// Update return value
		bResult = TRUE;

	} // End of successfully downloaded file
	else
	{
		// Unable to download file

		// Format status message
		wsprintf( lpszStatusMessage, INTERNET_UNABLE_TO_DOWNLOAD_STATUS_MESSAGE_FORMAT_STRING, lpszUrl );

	} // End of unable to download file

	// Show status message on status bar window
	StatusBarWindowSetText( lpszStatusMessage );

	// Free string memory
	delete [] lpszStatusMessage;

	return bResult;

} // End of function DownloadFile

void EditWindowUpdateFunction( int nTextLength )
{

	// See if edit window contains text
	if( nTextLength > 0 )
	{
		// Edit window contains text

		// Enable button window
		ButtonWindowEnable( TRUE );

	} // End of edit window contains text
	else
	{
		// Edit window is empty

		// Disable button window
		ButtonWindowEnable( FALSE );

	} // End of edit window is empty

} // End of function EditWindowUpdateFunction

void ListBoxWindowDoubleClickFunction( LPCTSTR lpszItemText )
{
	// Display item text
	MessageBox( NULL, lpszItemText, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

} // End of function ListBoxWindowDoubleClickFunction

void ListBoxWindowSelectionChangedFunction( LPCTSTR lpszItemText )
{
	// Show item text on status bar window
	StatusBarWindowSetText( lpszItemText );

} // End of function ListBoxWindowSelectionChangedFunction

int ShowAboutMessage( HWND hWndParent )
{
	int nResult = 0;

	MSGBOXPARAMS mbp;

	// Clear message box parameter structure
	ZeroMemory( &mbp, sizeof( mbp ) );

	// Initialise message box parameter structure
	mbp.cbSize		= sizeof( MSGBOXPARAMS );
	mbp.hwndOwner	= hWndParent;
	mbp.hInstance	= NULL;
	mbp.lpszText	= ABOUT_MESSAGE_TEXT;
	mbp.lpszCaption	= ABOUT_MESSAGE_CAPTION;
	mbp.dwStyle		= ( MB_OK | MB_USERICON );
	mbp.lpszIcon	= MAIN_WINDOW_CLASS_ICON_NAME;

	// Show message box
	nResult = MessageBoxIndirect( &mbp );

	return nResult;

} // End of function ShowAboutMessage

LRESULT CALLBACK MainWindowProcedure( HWND hWndMain, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	LRESULT lr = 0;

	// Select message
	switch( uMessage )
	{
		case WM_CREATE:
		{
			// A create message
			HINSTANCE hInstance;

			// Get instance
			hInstance = GetModuleHandle( NULL );

			// Create edit window
			if( EditWindowCreate( hWndMain, hInstance ) )
			{
				// Successfully created edit window
				HFONT hFont;

				// Get font
				hFont = ( HFONT )GetStockObject( DEFAULT_GUI_FONT );

				// Set edit window font
				EditWindowSetFont( hFont );

				// Create button window
				if( ButtonWindowCreate( hWndMain, hInstance ) )
				{
					// Successfully created button window

					// Set button window font
					ButtonWindowSetFont( hFont );

					// Create list box window
					if( ListBoxWindowCreate( hWndMain, hInstance ) )
					{
						// Successfully created list box window

						// Set list box window font
						ListBoxWindowSetFont( hFont );

						// Create status bar window
						if( StatusBarWindowCreate( hWndMain, hInstance ) )
						{
							// Successfully created status bar window

							// Set status bar window font
							StatusBarWindowSetFont( hFont );

							// Select edit window text
							EditWindowSelect();

						} // End of successfully created status bar window

					} // End of successfully created list box window

				} // End of successfully created button window

			} // End of successfully created edit window

			// Break out of switch
			break;

		} // End of a create message
		case WM_SIZE:
		{
			// A size message
			int nClientWidth;
			int nClientHeight;
			RECT rcStatusBar;
			int nStatusBarWindowHeight;
			int nListBoxWindowHeight;
			int nListBoxWindowTop;
			int nEditWindowWidth;
			int nButtonWindowLeft;

			// Get client size
			nClientWidth	= LOWORD( lParam );
			nClientHeight	= HIWORD( lParam );

			// Size status bar window
			StatusBarWindowSize();

			// Get status bar window size
			StatusBarWindowGetRect( &rcStatusBar );

			// Calculate window sizes
			nStatusBarWindowHeight	= ( rcStatusBar.bottom - rcStatusBar.top );
			nListBoxWindowHeight	= ( nClientHeight - ( nStatusBarWindowHeight + BUTTON_WINDOW_HEIGHT ) + WINDOW_BORDER_HEIGHT );
			nEditWindowWidth		= ( ( nClientWidth - BUTTON_WINDOW_WIDTH ) + WINDOW_BORDER_WIDTH );

			// Calculate window positions
			nListBoxWindowTop		= ( BUTTON_WINDOW_HEIGHT - WINDOW_BORDER_HEIGHT );
			nButtonWindowLeft		= ( nEditWindowWidth - WINDOW_BORDER_WIDTH );

			// Move control windows
			EditWindowMove( 0, 0, nEditWindowWidth, BUTTON_WINDOW_HEIGHT, TRUE );
			ButtonWindowMove( nButtonWindowLeft, 0, BUTTON_WINDOW_WIDTH, BUTTON_WINDOW_HEIGHT, TRUE );
			ListBoxWindowMove( 0, nListBoxWindowTop, nClientWidth, nListBoxWindowHeight, TRUE );

			// Break out of switch
			break;

		} // End of a size message
		case WM_ACTIVATE:
		{
			// An activate message

			// Focus on edit window
			EditWindowSetFocus();

			// Break out of switch
			break;

		} // End of an activate message
		case WM_GETMINMAXINFO:
		{
			// A get min max info message
			MINMAXINFO *lpMinMaxInfo = ( MINMAXINFO * )lParam;

			// Update minimum size
			lpMinMaxInfo->ptMinTrackSize.x = MAIN_WINDOW_MINIMUM_WIDTH;
			lpMinMaxInfo->ptMinTrackSize.y = MAIN_WINDOW_MINIMUM_HEIGHT;

			// Break out of switch
			break;

		} // End of a get min max info message
		case WM_COMMAND:
		{
			// A command message

			// Select command
			switch( LOWORD( wParam ) )
			{
				case BUTTON_WINDOW_ID:
				{
					// A button window command

					// Allocate string memory
					LPTSTR lpszUrl = new char[ STRING_LENGTH ];

					// Get url from edit window
					if( EditWindowGetText( lpszUrl ) )
					{
						// Successfully got url from edit window

						// Allocate string memory
						LPTSTR lpszLocalFilePath = new char[ STRING_LENGTH ];

						// Download file
						if( DownloadFile( lpszUrl, lpszLocalFilePath ) )
						{
							// Successfully downloaded file

							// Allocate string memory
							LPTSTR lpszStatusMessage = new char[ STRING_LENGTH ];

							// Format status message
							wsprintf( lpszStatusMessage, HTML_FILE_OPENING_STATUS_MESSAGE_FORMAT_STRING, lpszLocalFilePath );

							// Show status message on status bar window
							StatusBarWindowSetText( lpszStatusMessage );

							// Open local file
							if( HtmlFileOpen( lpszLocalFilePath ) )
							{
								// Successfully opened local file
								int nItemCount;

								// Process tags in local file
								nItemCount = HtmlFileProcessItems( HTML_FILE_ANCHOR_TAG_NAME, HTML_FILE_ANCHOR_TAG_ATTRIBUTE, lpszUrl, &ProcessItemFunction );

								// Format status message
								wsprintf( lpszStatusMessage, HTML_FILE_SUCCESSFULLY_PROCESSED_STATUS_MESSAGE_FORMAT_STRING, nItemCount, lpszLocalFilePath );

							} // End of successfully opened local file
							else
							{
								// Unable to open local file

								// Format status message
								wsprintf( lpszStatusMessage, HTML_FILE_UNABLE_TO_OPEN_STATUS_MESSAGE_FORMAT_STRING, lpszLocalFilePath );

							} // End of unable to open local file

							// Show status message on status bar window
							StatusBarWindowSetText( lpszStatusMessage );

						} // End of successfully downloaded file

						// Free string memory
						delete [] lpszLocalFilePath;

					} // End of successfully got url from edit window

					// Free string memory
					delete [] lpszUrl;

					// Break out of switch
					break;

				} // End of a button window command
				case IDM_HELP_ABOUT:
				{
					// A help about command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about command
				case IDM_FILE_EXIT:
				{
					// A file exit command

					// Destroy main window
					DestroyWindow( hWndMain );

					// Break out of switch
					break;

				} // End of a file exit command
				default:
				{
					// Default command

					// See if command is from edit window
					if( IsEditWindow( ( HWND )lParam ) )
					{
						// Command is from edit window

						// Handle command from edit window
						if( !( EditWindowHandleCommandMessage( wParam, lParam, &EditWindowUpdateFunction ) ) )
						{
							// Command was not handled from edit window

							// Call default procedure
							lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

						} // End of command was not handled from edit window

					} // End of command is from edit window
					else if( IsListBoxWindow( ( HWND )lParam ) )
					{
						// Command is from list box window

						// Handle command from list box window
						if( !( ListBoxWindowHandleCommandMessage( wParam, lParam, &ListBoxWindowDoubleClickFunction, &ListBoxWindowSelectionChangedFunction ) ) )
						{
							// Command was not handled from list box window

							// Call default procedure
							lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

						} // End of command was not handled from list box window

					} // End of command is from list box window
					else
					{
						// Command is not from list box window

						// Call default procedure
						lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

					} // End of command is not from list box window

					// Break out of switch
					break;

				} // End of default command

			}; // End of selection for command

			// Break out of switch
			break;

		} // End of a command message
		case WM_SYSCOMMAND:
		{
			// A system command message

			// Select system command 
			switch( LOWORD( wParam ) )
			{
				case IDM_HELP_ABOUT:
				{
					// A help about system command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about system command
				default:
				{
					// Default system command

					// Call default window procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

					// Break out of switch
					break;

				} // End of default system command

			}; // End of selection for system command

			// Break out of switch
			break;

		} // End of a system command message
		case WM_CONTEXTMENU:
		{
			// A context menu message
			HMENU hMenuContext;

			// Load context menu
			hMenuContext = LoadMenu( NULL, MAKEINTRESOURCE( IDR_CONTEXT_MENU ) );

			// Show context menu
			TrackPopupMenu( GetSubMenu( hMenuContext, 0 ), ( TPM_LEFTALIGN | TPM_LEFTBUTTON ), LOWORD( lParam ), HIWORD( lParam ), 0, hWndMain, NULL );

			// Break out of switch
			break;

		} // End of a context menu message
		case WM_CLOSE:
		{
			// A close message

			// Destroy main window
			DestroyWindow( hWndMain );

			// Break out of switch
			break;

		} // End of a close message
		case WM_DESTROY:
		{
			// A destroy message

			// Terminate thread
			PostQuitMessage( 0 );

			// Break out of switch
			break;

		} // End of a destroy message
		default:
		{
			// Default message

			// Call default procedure
			lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lr;

} // End of function MainWindowProcedure

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE,  LPSTR, int nCmdShow )
{
	MSG msg;

	// Clear message structure
	ZeroMemory( &msg, sizeof( msg ) );
	
	// Connect to internet
	if( InternetConnect() )
	{
		// Successfully connected to internet
		WNDCLASSEX wcMain;

		// Clear main window class structure
		ZeroMemory( &wcMain, sizeof( wcMain ) );

		// Initialise main window class structure
		wcMain.cbSize			= sizeof( WNDCLASSEX );
		wcMain.style			= MAIN_WINDOW_CLASS_STYLE;
		wcMain.lpfnWndProc		= MainWindowProcedure;
		wcMain.hInstance		= hInstance;
		wcMain.hIcon			= MAIN_WINDOW_CLASS_ICON;
		wcMain.hCursor			= MAIN_WINDOW_CLASS_CURSOR;
		wcMain.hbrBackground	= MAIN_WINDOW_CLASS_BACKGROUND;
		wcMain.lpszMenuName		= MAIN_WINDOW_CLASS_MENU_NAME;
		wcMain.lpszClassName	= MAIN_WINDOW_CLASS_NAME;
		wcMain.hIconSm			= MAIN_WINDOW_CLASS_ICON;

		// Register main window class
		if( RegisterClassEx( &wcMain ) )
		{
			// Successfully registered main window class
			HWND hWndMain;

			// Create main window
			hWndMain = CreateWindowEx( MAIN_WINDOW_EXTENDED_STYLE, MAIN_WINDOW_CLASS_NAME, MAIN_WINDOW_TEXT, MAIN_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL );

			// Ensure that main window was created
			if( hWndMain )
			{
				// Successfully created main window
				HMENU hMenuSystem;

				// Get system menu
				hMenuSystem = GetSystemMenu( hWndMain, FALSE );

				// Add separator to system menu
				InsertMenu( hMenuSystem, SYSTEM_MENU_SEPARATOR_ITEM_POSITION, ( MF_BYPOSITION | MF_SEPARATOR ), 0, NULL );

				// Add about item to system menu
				InsertMenu( hMenuSystem, SYSTEM_MENU_ABOUT_ITEM_POSITION, MF_BYPOSITION, IDM_HELP_ABOUT, SYSTEM_MENU_ABOUT_ITEM_TEXT );

				// Show main window
				ShowWindow( hWndMain, nCmdShow );

				// Update main window
				UpdateWindow( hWndMain );

				// Main message loop
				while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
				{
					// Translate message
					TranslateMessage( &msg );

					// Dispatch message
					DispatchMessage( &msg );

				}; // End of main message loop

			} // End of successfully created main window
			else
			{
				// Unable to create main window

				// Display error message
				MessageBox( NULL, UNABLE_TO_CREATE_MAIN_WINDOW_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

			} // End of unable to create main window

		} // End of successfully registered main window class
		else
		{
			// Unable to register main window class

			// Display error message
			MessageBox( NULL, UNABLE_TO_REGISTER_MAIN_WINDOW_CLASS_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

		} // End of unable to register main window class

		// Close internet
		InternetClose();

	} // End of successfully connected to internet
	else
	{
		// Unable to connect to internet

		// Display error message
		MessageBox( NULL, INTERNET_UNABLE_TO_CONNECT_TO_INTERNET_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

	} // End of unable to connect to internet

	return msg.wParam;

} // End of function WinMain