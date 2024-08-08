// InternetApp.cpp

#include "InternetApp.h"

// Global variables
LPTSTR g_lpszParentURL;

BOOL DisplayTagFunction( LPCTSTR lpszTag )
{
	BOOL bResult = FALSE;

	// Allocate string memory
	LPTSTR lpszTagName = new char[ STRING_LENGTH ];

	// Get tag name
	if( InternetFileGetTagName( lpszTag, lpszTagName ) )
	{
		// Successfully got tag name

		// Ensure that tag is not empty
		if( lpszTagName[ 0 ] )
		{
			// Tag is not empty

			// Ensure that tag is not an end tag
			if( lpszTagName[ 0 ] != INTERNET_FILE_END_TAG_PREFIX )
			{
				// Tag is not an end tag
				HTREEITEM htiTagName;

				// Allocate string memory
				LPTSTR lpszHeader = new char[ STRING_LENGTH ];

				// See if tag is an anchor
				if( lstrcmpi( lpszTagName, INTERNET_FILE_ANCHOR_TAG_NAME ) == 0 )
				{
					// Tag is an anchor

					// Update header
					lstrcpy( lpszHeader, INTERNET_FILE_ANCHOR_TAG_HEADER );

				} // End of tag is an anchor
				else if( lstrcmpi( lpszTagName, INTERNET_FILE_IMAGE_TAG_NAME ) == 0 )
				{
					// Tag is an image

					// Update header
					lstrcpy( lpszHeader, INTERNET_FILE_IMAGE_TAG_HEADER );

				} // End of tag is an image
				else
				{
					// Tag is of unknown type

					// Update header
					lstrcpy( lpszHeader, INTERNET_FILE_UNKNOWN_TAG_HEADER );

				} // End of tag is of unknown type

				// Insert tag name
				htiTagName = TreeViewWindowInsertUniqueItem( lpszHeader, TVI_ROOT, TVI_SORT );

				// Add tag to tree view window
				if( TreeViewWindowInsertItem( lpszTag, htiTagName, TVI_SORT ) )
				{ 
					// Successfully added tag to tree view window

					// Update return value
					bResult = TRUE;

				} // End of successfully added tag to tree view window

				// Free string memory
				delete [] lpszHeader;

			} // End of tag is not an end tag

		} // End of tag is not empty

	} // End of successfully got tag name

	// Free string memory
	delete [] lpszTagName;

	return bResult;
	
} // End of function DisplayTagFunction

BOOL DownloadFile( LPCTSTR lpszUrl, LPTSTR lpszLocalFilePath )
{
	BOOL bResult = FALSE;

	// Allocate string memory
	LPTSTR lpszStatusMessage = new char[ STRING_LENGTH ];

	// Format status message
	wsprintf( lpszStatusMessage, DOWNLOADING_INTERNET_FILE_STATUS_MESSAGE_FORMAT_STRING, lpszUrl );

	// Show status message on status bar window
	StatusBarWindowSetText( lpszStatusMessage );

	// Download file from url
	if( InternetDownloadFile( lpszUrl, lpszLocalFilePath ) )
	{
		// Successfully downloaded file from url

		// Format status message
		wsprintf( lpszStatusMessage, INTERNET_SUCESSFULLY_DOWNLOADED_FILE_FORMAT_STRING, lpszUrl, lpszLocalFilePath );

		// Update return value
		bResult = TRUE;

	} // End of successfully downloaded file from url
	else
	{
		// Unable to download file from url

		// Display status message
		MessageBox( NULL, lpszStatusMessage, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

	} // End of unable to download file from url

	// Show status message on status bar window
	StatusBarWindowSetText( lpszStatusMessage );

	// Free string memory
	delete [] lpszStatusMessage;

	return bResult;

} // End of function DownloadFile

BOOL TreeViewWindowDownloadFile( LPCTSTR lpszUrl )
{
	BOOL bResult = FALSE;

	// Allocate string memory
	LPTSTR lpszLocalFilePath = new char[ STRING_LENGTH ];

	// Download file from url
	if( DownloadFile( lpszUrl, lpszLocalFilePath ) )
	{
		// Successfully downloaded file from url

		// Allocate string memory
		LPTSTR lpszStatusMessage = new char[ STRING_LENGTH ];

		// Format status message
		wsprintf( lpszStatusMessage, LOADING_LOCAL_FILE_STATUS_MESSAGE_FORMAT_STRING, lpszLocalFilePath );

		// Show status message on status bar window
		StatusBarWindowSetText( lpszStatusMessage );

		// Load internet file
		if( InternetFileLoad( lpszLocalFilePath ) )
		{
			// Successfully loaded internet file
			int nTagCount;

			// Format status message
			wsprintf( lpszStatusMessage, PROCESSING_TAGS_IN_LOCAL_FILE_STATUS_MESSAGE_FORMAT_STRING, lpszLocalFilePath );

			// Show status message on status bar window
			StatusBarWindowSetText( lpszStatusMessage );

			// Process tags in internet file
			nTagCount = InternetFileProcessTags( &DisplayTagFunction );

			// Update tree view window
			TreeViewWindowUpdate();

			// Format status message
			wsprintf( lpszStatusMessage, INTERNET_FILE_PROCESS_TAGS_FORMAT_STRING, lpszUrl, nTagCount );

			// Show status message on status bar window
			StatusBarWindowSetText( lpszStatusMessage );

			// Update return value
			bResult = TRUE;

		} // End of successfully loaded internet file
		else
		{
			// Unable to load internet file

			// Format status message
			wsprintf( lpszStatusMessage, INTERNET_FILE_UNABLE_TO_LOAD_ERROR_MESSAGE_FORMAT_STRING, lpszLocalFilePath );

			// Display status message
			MessageBox( NULL, lpszStatusMessage, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

			// Show status message on status bar window
			StatusBarWindowSetText( lpszStatusMessage );

		} // End of unable to load internet file

		// Free string memory
		delete [] lpszStatusMessage;

	} // End of successfully downloaded file from url

	// Free string memory
	delete [] lpszLocalFilePath;

	return bResult;

} // End of function TreeViewWindowDownloadFile

BOOL ProcessTagDownload( LPCTSTR lpszTag, LPCTSTR lpszAttributeName )
{
	BOOL bResult = FALSE;

	// Allocate string memory
	LPTSTR lpszTargetURL = new char[ STRING_LENGTH ];

	// Get target url
	if( InternetFileGetAttributeValue( lpszTag, g_lpszParentURL, lpszAttributeName, lpszTargetURL ) )
	{
		// Successfully got target url

		// Allocate string memory
		LPTSTR lpszLocalFilePath = new char[ STRING_LENGTH ];

		// Download image
		bResult = DownloadFile( lpszTargetURL, lpszLocalFilePath );

		// Free string memory
		delete [] lpszLocalFilePath;

	} // End of successfully got target url
	else
	{
		// Unable to get target url

		// Allocate string memory
		LPTSTR lpszStatusMessage	= new char[ STRING_LENGTH ];

		// Format status message
		wsprintf( lpszStatusMessage, INTERNET_FILE_UNABLE_TO_GET_TARGET_URL_ERROR_MESSAGE_FORMAT_STRING, lpszTag );

		// Dusplay status message
		MessageBox( NULL, lpszStatusMessage, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

		// Show status message on status bar window
		StatusBarWindowSetText( lpszStatusMessage );

		// Free string memory
		delete [] lpszStatusMessage;

	} // End of unable to get target url

	// Free string memory
	delete [] lpszTargetURL;

	return bResult;

} // End of function ProcessTagDownload

BOOL ProcessTag( LPCTSTR lpszTag )
{
	BOOL bResult = FALSE;

	// Allocate string memory
	LPTSTR lpszTagName			= new char[ STRING_LENGTH ];
	LPTSTR lpszStatusMessage	= new char[ STRING_LENGTH ];

	// Get tag name
	if( InternetFileGetTagName( lpszTag, lpszTagName ) )
	{
		// Successfully got tag name

		// See if this is an anchor tag
		if( lstrcmpi( lpszTagName, INTERNET_FILE_ANCHOR_TAG_NAME ) == 0 )
		{
			// This is an anchor tag

			// Download item
			bResult = ProcessTagDownload( lpszTag, INTERNET_FILE_ANCHOR_TAG_ATTRIBUTE_NAME );

		} // End of this is an anchor tag
		if( lstrcmpi( lpszTagName, INTERNET_FILE_IMAGE_TAG_NAME ) == 0 )
		{
			// This is an image tag

			// Download image
			bResult = ProcessTagDownload( lpszTag, INTERNET_FILE_IMAGE_TAG_ATTRIBUTE_NAME );

		} // End of this is an image tag
		else
		{
			// This is an unknown tag

			// Display message
			MessageBox( NULL, lpszTag, INTERNET_FILE_UNKNOWN_TAG_TYPE_ERROR_MESSAGE_TITLE, ( MB_OK | MB_ICONINFORMATION ) );

		} // End of this is an unknown tag

	} // End of successfully got tag name
	else
	{
		// Unable to get tag name

		// Format status message
		wsprintf( lpszStatusMessage, INTERNET_FILE_UNABLE_TO_GET_TAG_NAME_ERROR_MESSAGE_FORMAT_STRING, lpszTag );

		// Show status message on status bar window
		StatusBarWindowSetText( lpszStatusMessage );

	} // End of unable to get tag name

	// Free string memory
	delete [] lpszTagName;
	delete [] lpszStatusMessage;

	return bResult;

} // End of function ProcessTag

void TreeViewWindowDoubleClickFunction( LPCTSTR lpszItemText )
{
	// Process tag
	ProcessTag( lpszItemText );

} // End of function TreeViewWindowDoubleClickFunction

void TreeViewWindowSelectionChangedFunction( LPCTSTR lpszItemText )
{
	// Show item text on status bar window
	StatusBarWindowSetText( lpszItemText );
	
} // End of function TreeViewWindowSelectionChangedFunction

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

// Main window procedure
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

					// Set button edit window font
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
			int nEditWindowWidth;
			int nButtonWindowLeft;
			int nStatusBarWindowHeight;
			int nTreeViewWindowTop;
			int nTreeViewWindowHeight;
			RECT rcStatusBar;

			// Store client width and height
			nClientWidth	= ( int )LOWORD( lParam );
			nClientHeight	= ( int )HIWORD( lParam );

			// Size status bar window
			StatusBarWindowSize();

			// Get status bar window size
			StatusBarWindowGetRect( &rcStatusBar );

			// Calculate control window sizes
			nStatusBarWindowHeight	= ( rcStatusBar.bottom - rcStatusBar.top );
			nEditWindowWidth		= ( nClientWidth - ( BUTTON_WINDOW_WIDTH - WINDOW_BORDER_WIDTH ) );
			nTreeViewWindowHeight	= ( nClientHeight - ( BUTTON_WINDOW_HEIGHT - WINDOW_BORDER_HEIGHT + nStatusBarWindowHeight ) );

			// Calculate control window positions
			nButtonWindowLeft	= ( nEditWindowWidth - WINDOW_BORDER_WIDTH );
			nTreeViewWindowTop	= ( BUTTON_WINDOW_HEIGHT - WINDOW_BORDER_HEIGHT );

			// Move control windows
			EditWindowMove( 0, 0, nEditWindowWidth, BUTTON_WINDOW_HEIGHT );
			ButtonWindowMove( nButtonWindowLeft, 0, BUTTON_WINDOW_WIDTH, BUTTON_WINDOW_HEIGHT );
			TreeViewWindowMove( 0, nTreeViewWindowTop, nClientWidth, nTreeViewWindowHeight );

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

						// Display file path
						MessageBox( hWndMain, lpszFilePath, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

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
					if( EditWindowGetText( g_lpszParentURL ) )
					{
						// Successfully got url from edit window

						// Download file from url
						TreeViewWindowDownloadFile( g_lpszParentURL );

					} // End of successfully got url from edit window

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
				case IDM_PROCESS_GROUP:
				{
					// A process group command
					int nItemCount;

					// Allocate string memory
					LPTSTR lpszStatusMessage = new char[ STRING_LENGTH ];

					// Process group
					nItemCount = TreeViewWindowProcessGroup( &ProcessTag );

					// Format status message
					wsprintf( lpszStatusMessage, TREE_VIEW_WINDOW_PROCESS_GROUP_STATUS_MESSAGE_FORMAT_STRING, nItemCount );

					// Show status message on status bar window
					StatusBarWindowSetText( lpszStatusMessage );

					// Free string memory
					delete [] lpszStatusMessage;

					// Break out of switch
					break;

				} // End of a process group command
				case IDM_PROCESS_TAG:
				{
					// A process tag command

					// Allocate string memory
					LPTSTR lpszItemText = new char[ STRING_LENGTH ];

					// Get item text
					if( TreeViewWindowGetItemText( lpszItemText ) )
					{
						// Successfully got item text

						// Process tag
						ProcessTag( lpszItemText );

					} // End of successfully got item text

					// Free string memory
					delete [] lpszItemText;

					// Break out of switch
					break;

				} // End of a process tag command
				default:
				{
					// Default command

					// Source window is ( HWND )lParam

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
					else
					{
						// Command is not from edit window

						// Call default procedure
						lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

					} // End of command is not from edit window

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

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
					// A help about command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about command
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

				// Handle notify message from tree view window
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
			HTREEITEM htiSelected;

			// Load 'group' context menu
			hMenuContext = LoadMenu( NULL, MAKEINTRESOURCE( IDR_CONTEXT_MENU ) );

			// Get selected tree item
			htiSelected = TreeViewWindowGetSelected();

			// Ensure that selected tree item was got
			if( htiSelected )
			{
				// Successfully got selected tree item

				// Allocate string memory
				LPTSTR lpszTopLevelItemText = new char[ STRING_LENGTH ];

				// Get top level item text
				TreeViewWindowGetTopLevelItemText( htiSelected, lpszTopLevelItemText );

				// See if top level item is unknown
				if( lstrcmpi( lpszTopLevelItemText, INTERNET_FILE_UNKNOWN_TAG_HEADER ) == 0 )
				{
					// Top level item is unknown

					// Disable tag and group menu items
					EnableMenuItem( hMenuContext, IDM_PROCESS_TAG, ( MF_BYCOMMAND | MF_GRAYED ) );
					EnableMenuItem( hMenuContext, IDM_PROCESS_GROUP, ( MF_BYCOMMAND | MF_GRAYED ) );

				} // End of top level item is unknown
				else
				{
					// Top level item is not unknown

					// See if selected tree item is a group
					if( TreeViewWindowIsGroup( htiSelected ) )
					{
						// Selected tree item is a group

						// Disable tag menu item
						EnableMenuItem( hMenuContext, IDM_PROCESS_TAG, ( MF_BYCOMMAND | MF_GRAYED ) );

					} // End of selected tree item is a group
					else
					{
						// Selected tree item is not a group

						// Disable group menu item
						EnableMenuItem( hMenuContext, IDM_PROCESS_GROUP, ( MF_BYCOMMAND | MF_GRAYED ) );

					} // End of selected tree item is not a group

				} // End of top level item is not unknown

				// Free string memory
				delete [] lpszTopLevelItemText;

			} // End of successfully got selected tree item
			else
			{
				// Unable to get selected tree item

				// Disable tag and group menu items
				EnableMenuItem( hMenuContext, IDM_PROCESS_TAG, ( MF_BYCOMMAND | MF_GRAYED ) );
				EnableMenuItem( hMenuContext, IDM_PROCESS_GROUP, ( MF_BYCOMMAND | MF_GRAYED ) );

			} // End of unable to get selected tree item

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

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPTSTR, int nCmdShow )
{
	MSG msg;

	WNDCLASSEX wcMain;

	// Allocate global memory
	g_lpszParentURL = new char[ STRING_LENGTH ];

	// Clear parent url
	g_lpszParentURL[ 0 ] = ( char )NULL;

	// Clear message structure
	ZeroMemory( &msg, sizeof( msg ) );

	// Clear main window class structure
	ZeroMemory( &wcMain, sizeof( wcMain ) );

	// Initialise main window class structure
	wcMain.cbSize			= sizeof( WNDCLASSEX );
	wcMain.lpfnWndProc		= MainWndProc;
	wcMain.hInstance		= hInstance;
	wcMain.lpszClassName	= MAIN_WINDOW_CLASS_NAME;
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
		hWndMain = CreateWindowEx( MAIN_WINDOW_EXTENDED_STYLE, MAIN_WINDOW_CLASS_NAME, MAIN_WINDOW_TITLE, MAIN_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT,  CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL );

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

					// Display argument
					MessageBox( hWndMain, lpszArgument, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

				}; // End of loop through arguments

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

	// Free global memory
	delete [] g_lpszParentURL;

	return msg.wParam;

} // End of function WinMain
