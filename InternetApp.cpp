// InternetApp.cpp

#include "InternetApp.h"

// Global variables
LPTSTR g_lpszParentUrl;
LPTSTR g_lpszRequiredTagName;
LPTSTR g_lpszRequiredAttributeName;
LPTSTR g_lpszAttributeMustContain;

int TagFunction( LPCTSTR lpszTag, LPCTSTR lpszRequiredTagName, LPCTSTR lpszRequiredAttributeName, LPCTSTR lpszAttributeMustContain )
{
	int nResult = -1;

	// See if this is an image tag
	if( HtmlFileIsTagName( lpszTag, lpszRequiredTagName ) )
	{
		// This is an image tag

		// Allocate string memory
		LPTSTR lpszAttributeUrl = new char[ STRING_LENGTH + sizeof( char ) ];

		// Get attribute url
		if( HtmlFileGetAttributeUrl( g_lpszParentUrl, lpszTag, lpszRequiredAttributeName, lpszAttributeUrl ) )
		{
			// Successfully got attribute url

			// See if attribute contains required text
			if( ( FindTextInString( lpszAttributeUrl, lpszAttributeMustContain ) >= 0 ) || ( lstrlen( lpszAttributeMustContain ) == 0 ) )
			{
				// Attribute contains required text (or required text is empty)

				// Allocate string memory
				LPTSTR lpszLocalFilePath = new char[ STRING_LENGTH + sizeof( char ) ];

				// Add url to list view window
				nResult = ListViewWindowAddItem( lpszAttributeUrl );

				// Ensure that url was added to list view window
				if( nResult >= 0 )
				{
					// Successfully added url to list view window

					// Download file
					if( InternetDownloadFile( lpszAttributeUrl, lpszLocalFilePath, &StatusBarWindowSetText ) )
					{
						// Successfully downloaded file

						// Add local file path to list view window
						ListViewWindowSetItemTextEx( nResult, LIST_VIEW_WINDOW_LOCAL_FILE_PATH_COLUMN_ID, lpszLocalFilePath );

					} // End of successfully downloaded file
					else
					{
						// Unable to download file

						// Add local file path to list view window
						ListViewWindowSetItemTextEx( nResult, LIST_VIEW_WINDOW_LOCAL_FILE_PATH_COLUMN_ID, INTERNET_UNABLE_TO_DOWNLOAD_FILE_STATUS_MESSAGE );

					} // End of unable to download file

				} // End of successfully added url to list view window

				// Free string memory
				delete [] lpszLocalFilePath;

			} // End of attribute contains required text (or required text is empty)

		} // End of successfully got attribute url

		// Free string memory
		delete [] lpszAttributeUrl;

	} // End of this is an image tag

	return nResult;

} // End of function TagFunction

BOOL EditWindowChangeFunction( DWORD dwTextLength )
{
	BOOL bResult = FALSE;

	// See if edit window contains text
	if( dwTextLength )
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

	return bResult;

} // End of function EditWindowChangeFunction

BOOL ListViewWindowDoubleClickFunction( LPCTSTR lpszItemText )
{
	BOOL bResult = FALSE;

	// Allocate string memory
	LPTSTR lpszAttributeValue = new char[ STRING_LENGTH + sizeof( char ) ];

	// Show tag
	HtmlFileGetAttributeValue( lpszItemText, "src", lpszAttributeValue );

	// Free string memory
	delete [] lpszAttributeValue;

	return bResult;

} // End of function ListViewWindowDoubleClickFunction

int ShowAboutMessage( HWND hWndParent )
{
	int nResult = 0;

	MSGBOXPARAMS mbp;

	// Clear message box parameter structure
	ZeroMemory( &mbp, sizeof( mbp ) );

	// Initialise message box parameter structure
	mbp.cbSize		= sizeof( MSGBOXPARAMS );
	mbp.hwndOwner	= hWndParent;
	mbp.hInstance	= GetModuleHandle( NULL );
	mbp.lpszText	= ABOUT_MESSAGE_TEXT;
	mbp.lpszCaption	= ABOUT_MESSAGE_CAPTION;
	mbp.dwStyle		= ( MB_OK | MB_USERICON );
	mbp.lpszIcon	= MAIN_WINDOW_CLASS_ICON_NAME;

	// Show message box
	nResult = MessageBoxIndirect( &mbp );

	return nResult;

} // End of function ShowAboutMessage

LRESULT CALLBACK MainWindowProcedure( HWND hWndMain, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lr = 0;

	// Select message
	switch( uMsg )
	{
		case WM_CREATE:
		{
			// A create message
			HINSTANCE hInstance;
			HFONT hFont;

			// Get instance
			hInstance = ( ( LPCREATESTRUCT )lParam )->hInstance;

			// Get font
			hFont = ( HFONT )GetStockObject( DEFAULT_GUI_FONT );

			// Create edit window
			if( EditWindowCreate( hWndMain, hInstance, DEFAULT_URL ) )
			{
				// Successfully created edit window

				// Set edit window font
				EditWindowSetFont( hFont );

			// Create button window
				if( ButtonWindowCreate( hWndMain, hInstance ) )
				{
					// Successfully created button window

					// Set button window font
					ButtonWindowSetFont( hFont );

					// Create list view window
					if( ListViewWindowCreate( hWndMain, hInstance ) )
					{
						// Successfully created list view window

						// Set list view window font
						ListViewWindowSetFont( hFont );

						// Create status bar window
						if( StatusBarWindowCreate( hWndMain, hInstance ) )
						{
							// Successfully created status bar window

							// Set status bar window font
							StatusBarWindowSetFont( hFont );

							// Select edit window text
							EditWindowSelect();

						} // End of successfully created status bar window

					} // End of successfully created list view window

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
			RECT rcStatus;
			int nStatusWindowHeight;
			int nListViewWindowHeight;
			int nListViewWindowTop;
			int nEditWindowWidth;
			int nButtonWindowLeft;

			// Store client width and height
			nClientWidth	= ( int )LOWORD( lParam );
			nClientHeight	= ( int )HIWORD( lParam );

			// Size status bar window
			StatusBarWindowSize();

			// Get status window size
			StatusBarWindowGetRect( &rcStatus );

			// Calculate window sizes
			nStatusWindowHeight		= ( rcStatus.bottom - rcStatus.top );
			nListViewWindowHeight	= ( nClientHeight - ( nStatusWindowHeight + BUTTON_WINDOW_HEIGHT ) + WINDOW_BORDER_HEIGHT );
			nEditWindowWidth		= ( ( nClientWidth - BUTTON_WINDOW_WIDTH ) + WINDOW_BORDER_WIDTH );

			// Calculate window positions
			nListViewWindowTop		= ( BUTTON_WINDOW_HEIGHT - WINDOW_BORDER_HEIGHT );
			nButtonWindowLeft		= ( nEditWindowWidth - WINDOW_BORDER_WIDTH );

			// Move control windows
			ListViewWindowMove( 0, nListViewWindowTop, nClientWidth, nListViewWindowHeight, TRUE );
			EditWindowMove( 0, 0, nEditWindowWidth, BUTTON_WINDOW_HEIGHT, TRUE );
			ButtonWindowMove( nButtonWindowLeft, 0, BUTTON_WINDOW_WIDTH, BUTTON_WINDOW_HEIGHT, TRUE );

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
			MINMAXINFO FAR *lpMinMaxInfo;

			// Get min max info structure
			lpMinMaxInfo = ( MINMAXINFO FAR * )lParam;

			// Update min max info structure
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
					LPTSTR lpszUrl = new char[ STRING_LENGTH + sizeof( char ) ];

					// Get url from edit window
					if( EditWindowGetText( lpszUrl, STRING_LENGTH ) )
					{
						// Successfully got url from edit window

						// Allocate string memory
						LPTSTR lpszLocalFilePath = new char[ STRING_LENGTH + sizeof( char ) ];

						// Download file
						if( InternetDownloadFile( lpszUrl, lpszLocalFilePath, &StatusBarWindowSetText ) )
						{
							// Successfully downloaded file

							// Load html file
							if( HtmlFileLoad( lpszLocalFilePath ) )
							{
								// Successfully loaded html file
								int nTagCount;

								// Allocate string memory
								LPTSTR lpszStatusMessage = new char[ STRING_LENGTH + sizeof( char ) ];

								// Update global parent url (this neesd to be done before we process the tags)
								lstrcpy( g_lpszParentUrl, lpszUrl );

								// Ensure that global parent url ends with a forward slash
								if( g_lpszParentUrl[ lstrlen( g_lpszParentUrl ) - sizeof( char ) ] != ASCII_FORWARD_SLASH_CHARACTER )
								{
									// Global parent url does not end with a forward slash

									// Append a forward slash onto global parent url
									lstrcat( g_lpszParentUrl, ASCII_FORWARD_SLASH_STRING );

								} // End of global parent url does not end with a forward slash

								// Process tags
								nTagCount = HtmlFileProcessTags( g_lpszRequiredTagName, g_lpszRequiredAttributeName, g_lpszAttributeMustContain, &TagFunction );

								// Auto-size all list view window columns
								ListViewWindowAutoSizeAllColumns()
;
								// Format status message
								wsprintf( lpszStatusMessage, HTML_FILE_PROCESS_TAGS_STATUS_MESSAGE_FORMAT_STRING, lpszLocalFilePath, nTagCount );

								// Show status message on status bar window
								StatusBarWindowSetText( lpszStatusMessage );

								// Free memory
								HtmlFileFreeMemory();

								// Free string memory
								delete [] lpszStatusMessage;

							} // End of successfully loaded html file

						} // End of successfully downloaded file

						// Free string memory
						delete [] lpszLocalFilePath;

					} // End of successfully got url from edit window

					// Free string memory
					delete [] lpszUrl;

					// Break out of switch
					break;

				} // End of a button window command
				case IDM_FILE_EXIT:
				{
					// A file exit command

					// Destroy main window
					DestroyWindow( hWndMain );

					// Break out of switch
					break;

				} // End of a file exit command
				case IDM_HELP_ABOUT:
				{
					// A help about command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about command
				default:
				{
					// Default command

					// See if command message is from edit window
					if( IsEditWindow( ( HWND )lParam ) )
					{
						// Command message is from edit window

						// Handle command message from edit window
						if( !( EditWindowHandleCommandMessage( wParam, lParam, &EditWindowChangeFunction ) ) )
						{
							// Command message was not handled from edit window

							// Call default procedure
							lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

						} // End of command message was not handled from edit window

					} // End of command message is from edit window
					else
					{
						// Command message is not from list view window

						// Call default procedure
						lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

					} // End of command message is not from list view window

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

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

					// Break out of switch
					break;

				} // End of default system command

			}; // End of selection for system command

			// Break out of switch
			break;

		} // End of a system command message
		case WM_NOTIFY:
		{
			// A notify message
			LPNMHDR lpNmHdr;

			// Get notify message handler
			lpNmHdr = ( LPNMHDR )lParam;

			// See if notify message is from list view window
			if( IsListViewWindow( lpNmHdr->hwndFrom ) )
			{
				// Notify message is from list view window

				// Handle notify message from list view window
				if( !( ListViewWindowHandleNotifyMessage( wParam, lParam, &StatusBarWindowSetText ) ) )
				{
					// Notify message was not handled from list view window

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

				} // End of notify message was not handled from list view window

			} // End of notify message is from list view window
			else
			{
				// Notify message is not from list view window

				// Call default procedure
				lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

			} // End of notify message is not from list view window

			// Break out of switch
			break;

		} // End of a notify message
		case WM_CONTEXTMENU:
		{
			// A context menu message
			HMENU hMenu;

			// Load context menu
			hMenu = LoadMenu( NULL, MAKEINTRESOURCE( IDR_CONTEXT_MENU ) );

			// Show context menu
			TrackPopupMenu( GetSubMenu( hMenu, 0 ), ( TPM_LEFTALIGN | TPM_LEFTBUTTON ), LOWORD( lParam ), HIWORD( lParam ), 0, hWndMain, NULL );

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

			// Call default window procedure
			lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lr;

} // End of function MainWindowProcedure

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow )
{
	MSG msg;

	// Allocate global memory
	g_lpszParentUrl				= new char[ STRING_LENGTH + sizeof( char ) ];
	g_lpszRequiredTagName		= new char[ STRING_LENGTH + sizeof( char ) ];
	g_lpszRequiredAttributeName	= new char[ STRING_LENGTH + sizeof( char ) ];
	g_lpszAttributeMustContain	= new char[ STRING_LENGTH + sizeof( char ) ];

	// Clear global strings
	g_lpszParentUrl[ 0 ]				= ( char )NULL;
	g_lpszRequiredTagName[ 0 ]			= ( char )NULL;
	g_lpszRequiredAttributeName[ 0 ]	= ( char )NULL;
	g_lpszAttributeMustContain[ 0 ]		= ( char )NULL;

	// Clear message structure
	ZeroMemory( &msg, sizeof( msg ) );

	// Connect to internet
	if( InternetConnect() )
	{
		// Successfully connected to internet
		WNDCLASSEX wcMain;

		// Clear window class structure
		ZeroMemory( &wcMain, sizeof( wcMain ) );

		// Initialise window class structure
		wcMain.cbSize			= sizeof( WNDCLASSEX );
		wcMain.lpfnWndProc		= MainWindowProcedure;
		wcMain.hInstance		= hInstance;
		wcMain.lpszClassName	= MAIN_WINDOW_CLASS_NAME;
		wcMain.style			= MAIN_WINDOW_CLASS_STYLE;
		wcMain.hIcon			= MAIN_WINDOW_CLASS_ICON;
		wcMain.hCursor			= MAIN_WINDOW_CLASS_CURSOR;
		wcMain.hbrBackground	= MAIN_WINDOW_CLASS_BACKGROUND;
		wcMain.lpszMenuName		= MAIN_WINDOW_CLASS_MENU_NAME;
		wcMain.hIconSm			= MAIN_WINDOW_CLASS_ICON_SMALL;

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
				LPWSTR *lpszArgumentList;
				int nArgumentCount;

				// Get system menu
				hMenuSystem = GetSystemMenu( hWndMain, FALSE );

				// Add separator item to system menu
				InsertMenu( hMenuSystem, SYSTEM_MENU_SEPARATOR_ITEM_POSITION, ( MF_BYPOSITION | MF_SEPARATOR ), 0, NULL );

				// Add about item to system menu
				InsertMenu( hMenuSystem, SYSTEM_MENU_ABOUT_ITEM_POSITION, MF_BYPOSITION, IDM_HELP_ABOUT, SYSTEM_MENU_ABOUT_ITEM_TEXT );

				// Get argument list
				lpszArgumentList = CommandLineToArgvW( GetCommandLineW(), &nArgumentCount );

				// Initialise global data
				lstrcpy( g_lpszRequiredTagName,			( LPTSTR )DEFAULT_REQUIRED_TAG_NAME );
				lstrcpy( g_lpszRequiredAttributeName,	( LPTSTR )DEFAULT_REQUIRED_ATTRIBUTE_NAME );
				lstrcpy( g_lpszAttributeMustContain,	( LPTSTR )DEFAULT_ATTRIBUTE_MUST_CONTAIN );

				// Ensure that argument list was got
				if( lpszArgumentList )
				{
					// Successfully got argument list
					int nSizeNeeded;
					int nWideArgumentLength;

					// Allocate string memory
					LPTSTR lpszArgument = new char[ STRING_LENGTH + sizeof( char ) ];

					// See if tag name argument is valid
					if( nArgumentCount > INTERNET_TAG_NAME_ARGUMENT )
					{
						// Tag name argument is valid

						// Get wide argument length
						nWideArgumentLength = lstrlenW( lpszArgumentList[ INTERNET_TAG_NAME_ARGUMENT ] );

						// Get size required for argument
						nSizeNeeded = WideCharToMultiByte( CP_UTF8, 0, lpszArgumentList[ INTERNET_TAG_NAME_ARGUMENT ], nWideArgumentLength, NULL, 0, NULL, NULL );

						// Convert argument to ansi
						WideCharToMultiByte( CP_UTF8, 0, lpszArgumentList[ INTERNET_TAG_NAME_ARGUMENT ], nWideArgumentLength, lpszArgument, nSizeNeeded, NULL, NULL );

						// Terminate argument
						lpszArgument[ nSizeNeeded ] = ( char )NULL;

						// Copy argument into required tag name
						lstrcpy( g_lpszRequiredTagName, lpszArgument );

						// See if attribute name argument is valid
						if( nArgumentCount > INTERNET_ATTRIBUTE_NAME_ARGUMENT )
						{
							// Attribute name argument is valid

							// Get wide argument length
							nWideArgumentLength = lstrlenW( lpszArgumentList[ INTERNET_ATTRIBUTE_NAME_ARGUMENT ] );

							// Get size required for argument
							nSizeNeeded = WideCharToMultiByte( CP_UTF8, 0, lpszArgumentList[ INTERNET_ATTRIBUTE_NAME_ARGUMENT ], nWideArgumentLength, NULL, 0, NULL, NULL );

							// Convert argument to ansi
							WideCharToMultiByte( CP_UTF8, 0, lpszArgumentList[ INTERNET_ATTRIBUTE_NAME_ARGUMENT ], nWideArgumentLength, lpszArgument, nSizeNeeded, NULL, NULL );

							// Terminate argument
							lpszArgument[ nSizeNeeded ] = ( char )NULL;

							// Copy argument into required attribute
							lstrcpy( g_lpszRequiredAttributeName, lpszArgument );

							// See if must contain argument is valid
							if( nArgumentCount > INTERNET_ATTRIBUTE_MUST_CONTAIN_ARGUMENT )
							{
								// Attribute must contain argument is valid

								// Get wide argument length
								nWideArgumentLength = lstrlenW( lpszArgumentList[ INTERNET_ATTRIBUTE_MUST_CONTAIN_ARGUMENT ] );

								// Get size required for argument
								nSizeNeeded = WideCharToMultiByte( CP_UTF8, 0, lpszArgumentList[ INTERNET_ATTRIBUTE_MUST_CONTAIN_ARGUMENT ], nWideArgumentLength, NULL, 0, NULL, NULL );

								// Convert argument to ansi
								WideCharToMultiByte( CP_UTF8, 0, lpszArgumentList[ INTERNET_ATTRIBUTE_MUST_CONTAIN_ARGUMENT ], nWideArgumentLength, lpszArgument, nSizeNeeded, NULL, NULL );

								// Terminate argument
								lpszArgument[ nSizeNeeded ] = ( char )NULL;

								// Copy argument into required attribute
								lstrcpy( g_lpszAttributeMustContain, lpszArgument );

							} // End of attribute must contain argument is valid

						} // End of attribute name argument is valid

					} // End of tag name argument is valid

					// Free string memory
					delete [] lpszArgument;

				} // End of successfully got argument list

				// Show main window
				ShowWindow( hWndMain, nCmdShow );

				// Update main window
				UpdateWindow( hWndMain );

				// Message loop
				while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
				{
					// Translate message
					TranslateMessage( &msg );

					// Dispatch message
					DispatchMessage( &msg );

				}; // End of message loop

			} // End of successfully main created window
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

	// Free global memory
	delete [] g_lpszParentUrl;
	delete [] g_lpszRequiredTagName;
	delete [] g_lpszRequiredAttributeName;

	return msg.wParam;

} // End of function WinMain
