// InternetApp.cpp

#include "InternetApp.h"

// Global variables
LPTSTR g_lpszParentUrl;

void TagFunction( LPTSTR lpszTag )
{
	// Allocate string memory
	LPTSTR lpszTagName = new char[ lstrlen( lpszTag ) ];

	// Get tag name
	if( HtmlFileGetTagName( lpszTag, lpszTagName ) )
	{
		// Successfully got tag name

		// Ensure that this is not an end tag
		if( lpszTagName[ 0 ] != ASCII_FORWARD_SLASH_CHARACTER )
		{
			// This is not an end tag
			HTREEITEM htiParentItem;

			// Allocate string memory
			LPTSTR lpszParentItemName	= new char[ STRING_LENGTH ];
			LPTSTR lpszItemText			= new char[ STRING_LENGTH ];

			// See if this is an anchor tag
			if( lstrcmpi( lpszTagName, HTML_FILE_ANCHOR_TAG_NAME ) == 0 )
			{
				// This is an anchor tag

				// Update parent item name
				lstrcpy( lpszParentItemName, HTML_FILE_ANCHOR_TAG_HEADER );

				// Update item text
				HtmlFileGetAttributeValue( lpszTag, g_lpszParentUrl, HTML_FILE_ANCHOR_TAG_ATTRIBUTE_NAME, lpszItemText );

			} // End of this is an anchor tag
			else if( lstrcmpi( lpszTagName, HTML_FILE_IMAGE_TAG_NAME ) == 0 )
			{
				// This is an image tag

				// Update parent item name
				lstrcpy( lpszParentItemName, HTML_FILE_IMAGE_TAG_HEADER );

				// Update item text
				HtmlFileGetAttributeValue( lpszTag, g_lpszParentUrl, HTML_FILE_IMAGE_TAG_ATTRIBUTE_NAME, lpszItemText );

			} // End of this is an image tag
			else
			{
				// This is an unknown tag

				// Update parent item name
				lstrcpy( lpszParentItemName, HTML_FILE_UNKNOWN_TAG_HEADER );

				// Update item text
				lstrcpy( lpszItemText, lpszTag );

			} // End of this is an unknown tag

			// See if parent item already exists on tree view window
			htiParentItem = TreeViewWindowFindItem( lpszParentItemName, TVI_ROOT );

			// See if parent item was found on tree view window
			if( htiParentItem == NULL )
			{
				// Parent item was not found on tree view window
			
				// Add parent item to tree view window
				htiParentItem = TreeViewWindowAddString( lpszParentItemName );

			} // End of parent item was not found on tree view window

			// Ensure that parent item tag is valid
			if( htiParentItem )
			{
				// parent item tag is valid

					// Add tag to tree view window
					TreeViewWindowAddString( lpszItemText, htiParentItem );

			} // End of parent item tag is valid

			// Free string memory
			delete [] lpszParentItemName;
			delete [] lpszItemText;

		} // End of this is not an end tag

	} // End of successfully got tag name

	// Free string memory
	delete [] lpszTagName;

} // End of function TagFunction

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

void TreeViewWindowDoubleClickFunction( LPCTSTR lpszItemText )
{
	// See if item is an absolute url
	if( strstr( lpszItemText, HTML_FILE_ABSOLUTE_IDENTIFIER ) )
	{
		// Item is an absolute url

		// Allocate string memory
		LPTSTR lpszLocalFilePath = new char[ STRING_LENGTH ];
		LPTSTR lpszStatusMessage = new char[ STRING_LENGTH ];

		// Format status message
		wsprintf( lpszStatusMessage, DOWNLOADING_STATUS_MESSAGE_FORMAT_STRING, lpszItemText );

		// Show status message on status bar window
		StatusBarWindowSetText( lpszStatusMessage );

		// Download item
		if( InternetDownloadFile( lpszItemText, lpszLocalFilePath ) )
		{
			// Successfully downloaded item

			// Format status message
			wsprintf( lpszStatusMessage, DOWNLOADED_STATUS_MESSAGE_FORMAT_STRING, lpszItemText, lpszLocalFilePath );

		} // End of successfully downloaded item
		else
		{
			// Unable to download item

			// Format status message
			wsprintf( lpszStatusMessage, UNABLE_TO_DOWNLOAD_STATUS_MESSAGE_FORMAT_STRING, lpszItemText );

		} // End of unable to download item

		// Show status message on status bar window
		StatusBarWindowSetText( lpszStatusMessage );

		// Free string memory
		delete [] lpszLocalFilePath;
		delete [] lpszStatusMessage;

	} // End of item is an absolute url
	else
	{
		// Item is an not absolute url

		// Display item text
		MessageBox( NULL, lpszItemText, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

	} // End of item is not an absolute url

} // End of function TreeViewWindowDoubleClickFunction

void TreeViewWindowSelectionChangedFunction( LPCTSTR lpszItemText )
{
	// Show item text on status bar window
	StatusBarWindowSetText( lpszItemText );

} // End of function TreeViewWindowSelectionChangedFunction

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

LRESULT CALLBACK MainWndProc( HWND hWndMain, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	LRESULT lr = 0;

	// Select message
	switch( uMessage )
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
			if( EditWindowCreate( hWndMain, hInstance ) )
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

					// Create tree view window
					if( TreeViewWindowCreate( hWndMain, hInstance ) )
					{
						// Successfully created tree view window

						// Set tree view window font
						TreeViewWindowSetFont( hFont );

						// Create status bar window
						if( StatusBarWindowCreate( hWndMain, hInstance ) )
						{
							// Successfully created status bar window

							// Set status bar window font
							StatusBarWindowSetFont( hFont );

							// Select edit window text
							EditWindowSelect();

						} // End of successfully created status bar window

					} // End of successfully created tree view window

				} // End of successfully created tree view window

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
			int nTreeViewWindowHeight;
			int nEditWindowWidth;
			int nButtonWindowLeft;
			int nTreeViewWindowTop;

			// Store client width and height
			nClientWidth	= ( int )LOWORD( lParam );
			nClientHeight	= ( int )HIWORD( lParam );

			// Size status bar window
			StatusBarWindowSize();

			// Get status window size
			StatusBarWindowGetRect( &rcStatus );

			// Calculate window sizes
			nStatusWindowHeight		= ( rcStatus.bottom - rcStatus.top );
			nTreeViewWindowHeight	= ( nClientHeight - ( BUTTON_WINDOW_HEIGHT + nStatusWindowHeight ) + WINDOW_BORDER_HEIGHT );
			nEditWindowWidth		= ( ( nClientWidth - BUTTON_WINDOW_WIDTH ) + WINDOW_BORDER_WIDTH );

			// Calculate window positions
			nButtonWindowLeft		= ( nEditWindowWidth - WINDOW_BORDER_WIDTH );
			nTreeViewWindowTop		= ( BUTTON_WINDOW_HEIGHT - WINDOW_BORDER_HEIGHT );

			// Move control windoww
			EditWindowMove( 0, 0, nEditWindowWidth, BUTTON_WINDOW_HEIGHT );
			ButtonWindowMove( nButtonWindowLeft, 0, BUTTON_WINDOW_WIDTH, BUTTON_WINDOW_HEIGHT );
			TreeViewWindowMove( 0, nTreeViewWindowTop, nClientWidth, nTreeViewWindowHeight, TRUE );

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
		case WM_DROPFILES:
		{
			// A drop files message
			UINT uFileCount;
			HDROP hDrop;
			UINT uWhichFile;
			UINT uFileSize;

			// Allocate string memory
			LPTSTR lpszFilePath = new char[ STRING_LENGTH ];

			// Get drop handle
			hDrop = ( HDROP )wParam;

			// Count dropped files
			uFileCount = DragQueryFile( hDrop, ( UINT )-1, NULL, 0 );

			// Loop through dropped files
			for( uWhichFile = 0; uWhichFile < uFileCount; uWhichFile ++ )
			{
				// Get size of dropped file
				uFileSize = DragQueryFile( hDrop, uWhichFile, NULL, 0 );

				// Ensure that file size is valid
				if( uFileSize != 0 )
				{
					// File size is valid

					// Get file path
					if( DragQueryFile( hDrop, uWhichFile, lpszFilePath, ( uFileSize + sizeof( char ) ) ) )
					{
						// Successfully got file path

						// Add file path to tree view window
						TreeViewWindowAddString( lpszFilePath );

					} // End of successfully got file path

				} // End of file size is valid

			}; // End of loop through dropped files

			// Free string memory
			delete [] lpszFilePath;

			// Break out of switch
			break;

		} // End of a drop files message
		case WM_COMMAND:
		{
			// A command message

			// Select command
			switch( LOWORD( wParam ) )
			{
				case BUTTON_WINDOW_ID:
				{
					// A button window command

					// Get url from edit window
					if( EditWindowGetText( g_lpszParentUrl ) )
					{
						// Successfully got url from edit window

						// Allocate string memory
						LPTSTR lpszLocalFilePath = new char[ STRING_LENGTH ];

						// Download file
						if( InternetDownloadFile( g_lpszParentUrl, lpszLocalFilePath ) )
						{
							// Successfully downloaded file

							// Read html file
							if( HtmlFileRead( lpszLocalFilePath ) )
							{
								// Successfully read html file

								// Process tags
								HtmlFileProcessTags( &TagFunction );

								// Free memory associated with html file
								HtmlFileFreeMemory();

							} // End of successfully read html file

						} // End of successfully downloaded file
						else
						{
							// Unable to donload file

							// Allocate string memory
							LPTSTR lpszErrorMessage = new char[ STRING_LENGTH ];

							// Format error message
							wsprintf( lpszErrorMessage, INTERNET_UNABLE_TO_DOWNLOAD_FILE_ERROR_MESSAGE_FORMAT_STRING, g_lpszParentUrl );

							// Display error message
							MessageBox( NULL, lpszErrorMessage, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

							// Free string memory
							delete [] lpszErrorMessage;

						} // End of unable to connect to internet

					} // End of successfully got url from edit window

					// Break out of switch
					break;

				} // End of a button window command
				case IDM_FILE_EXIT:
				{
					// A file exit command

					// Destroy window
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
						if( !( EditWindowHandleCommandMessage( wParam, lParam, &EditWindowUpdateFunction ) ) )
						{
							// Command message was not handled from edit window

							// Call default procedure
							lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

						} // End of command message was not handled from edit window

					} // End of command message is from edit window
					else
					{
						// Command message is not from edit window

						// Call default procedure
						lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

					} // End of command message is not from edit window

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
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

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

			// See if notify message is from tree view window
			if( IsTreeViewWindow( lpNmHdr->hwndFrom ) )
			{
				// Notify message is from tree view window

				if( !( TreeViewWindowHandleNotifyMessage( wParam, lParam, &TreeViewWindowDoubleClickFunction, &TreeViewWindowSelectionChangedFunction ) ) )
				{
					// Notify message was not handled from tree view window

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

				} // End of notify message was not handled from tree view window

			} // End of notify message is from tree view window
			else
			{
				// Notify message is not from tree view window

				// Call default procedure
				lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			} // End of notify message is not from tree view window

			// Break out of switch
			break;

		} // End of a notify message
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

			// Call default window procedure
			lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lr;

} // End of function MainWndProc

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow )
{
	MSG msg;

	// Clear message structure
	ZeroMemory( &msg, sizeof( msg ) );

	// Allocate global memory
	g_lpszParentUrl = new char[ STRING_LENGTH ];

	// Clear parent url
	g_lpszParentUrl[ 0 ] = ( char )NULL;

	// Connect to internet
	if( InternetConnect() )
	{
		// Successfully connected to internet
		WNDCLASSEX wcMain;

		// Clear window class structure
		ZeroMemory( &wcMain, sizeof( wcMain ) );

		// Initialise main window class structure
		wcMain.cbSize			= sizeof( WNDCLASSEX );
		wcMain.lpszClassName	= MAIN_WINDOW_CLASS_NAME;
		wcMain.lpfnWndProc		= MainWndProc;
		wcMain.hInstance		= hInstance;
		wcMain.style			= MAIN_WINDOW_CLASS_STYLE;
		wcMain.hIcon			= MAIN_WINDOW_CLASS_ICON;
		wcMain.hCursor			= MAIN_WINDOW_CLASS_CURSOR;
		wcMain.hbrBackground	= MAIN_WINDOW_CLASS_BACKGROUND;
		wcMain.lpszMenuName		= MAIN_WINDOW_CLASS_MENU_NAME;
		wcMain.hIconSm			= MAIN_WINDOW_CLASS_ICON;

		// Register main window class
		if( RegisterClassEx( &wcMain ) )
		{
			// Successfully registered main window class
			HWND hWndMain;

			// Create main window
			hWndMain = CreateWindowEx( MAIN_WINDOW_EXTENDED_STYLE, MAIN_WINDOW_CLASS_NAME, MAIN_WINDOW_TEXT, MAIN_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

			// Ensure that main window was created
			if( hWndMain )
			{
				// Successfully created main window
				HMENU hMenuSystem;
				LPWSTR *lpszArgumentList;
				int nArgumentCount;

				// Get system menu
				hMenuSystem = GetSystemMenu( hWndMain, FALSE );

				// Add separator to system menu
				InsertMenu( hMenuSystem, SYSTEM_MENU_SEPARATOR_ITEM_POSITION, ( MF_BYPOSITION | MF_SEPARATOR ), 0, NULL );

				// Add about item to system menu
				InsertMenu( hMenuSystem, SYSTEM_MENU_ABOUT_ITEM_POSITION, MF_BYPOSITION, IDM_HELP_ABOUT, SYSTEM_MENU_ABOUT_ITEM_TEXT );

				// Get argument list
				lpszArgumentList = CommandLineToArgvW( GetCommandLineW(), &nArgumentCount );

				// Ensure that argument list was got
				if( lpszArgumentList )
				{
					// Successfully got argument list
					int nWhichArgument;
					int nSizeNeeded;
					int nWideArgumentLength;

					// Allocate string memory
					LPTSTR lpszArgument = new char[ STRING_LENGTH ];

					// Loop through arguments
					for( nWhichArgument = 1; nWhichArgument < nArgumentCount; nWhichArgument ++ )
					{
						// Get wide argument length
						nWideArgumentLength = lstrlenW( lpszArgumentList[ nWhichArgument ] );

						// Get size required for argument
						nSizeNeeded = WideCharToMultiByte( CP_UTF8, 0, lpszArgumentList[ nWhichArgument ], nWideArgumentLength, NULL, 0, NULL, NULL );

						// Convert argument to ansi
						WideCharToMultiByte( CP_UTF8, 0, lpszArgumentList[ nWhichArgument ], nWideArgumentLength, lpszArgument, nSizeNeeded, NULL, NULL );

						// Terminate argument
						lpszArgument[ nSizeNeeded ] = ( char )NULL;

						// Add argument to tree view window
						TreeViewWindowAddString( lpszArgument );

					}; // End of loop through arguments

					// Free string memory
					delete [] lpszArgument;

				} // End of successfully got argument list

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
		MessageBox( NULL, INTERNET_UNABLE_TO_CONNECT_INTERNET_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

	} // End of unable to connect to internet

	// Free global memory
	delete [] g_lpszParentUrl;

	return msg.wParam;

} // End of function WinMain
