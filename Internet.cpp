// Internet.cpp

#include "Internet.h"

// Global variables
HWND g_hWndButton;
HWND g_hWndEdit;
HWND g_hWndListBox;
HWND g_hWndStatusBar;
Internet g_internet;

int DisplayFile( LPCTSTR lpszFileText )
{
	int nResult = 0;

	LPTSTR lpszStartOfTag;
	LPTSTR lpszEndOfTag = ( LPTSTR )lpszFileText;
	DWORD dwMaxTagLength = STRING_LENGTH;
	DWORD dwTagLength;

	// Allocate string memory
	LPTSTR lpszTag = new char[ dwMaxTagLength + sizeof( char ) ];

	// Find start of first tag
	lpszStartOfTag = strchr( lpszFileText, ASCII_LESS_THAN_CHARACTER );

	// Loop through all tags
	while( lpszStartOfTag )
	{
		// Find end of tag
		lpszEndOfTag = strchr( lpszStartOfTag, ASCII_GREATER_THAN_CHARACTER );

		// Calculate length of tag
		dwTagLength = ( ( lpszEndOfTag - lpszStartOfTag ) + sizeof( char ) );

		// Ensure that tag length is less then maximum
		if( dwTagLength > dwMaxTagLength )
		{
			// Tag length is greater then maximum

			// Free string memory
			delete [] lpszTag;

			// Update maximum
			dwMaxTagLength = dwTagLength;

			// Re-allocate string memory
			lpszTag = new char[ dwMaxTagLength + sizeof( char ) ];

		} // End of tag length is greater then maximum

		// Store tag
		lstrcpyn( lpszTag, lpszStartOfTag, ( dwTagLength + sizeof( char ) ) );

		// Add tag to list box window
		SendMessage( g_hWndListBox, LB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszTag );

		// Find start of next tag
		lpszStartOfTag = strchr( lpszEndOfTag, ASCII_LESS_THAN_CHARACTER );

		// Update result
		nResult ++;

	}; // End of loop through all tags

	// Free string memory
	delete [] lpszTag;

	return nResult;

} // End of function DisplayFile

BOOL DownloadFile( LPCTSTR lpszUrl )
{
	BOOL bResult = FALSE;

	// Allocate string memory
	LPTSTR lpszLocalFilePath	= new char[ STRING_LENGTH ];
	LPTSTR lpszStatus			= new char[ STRING_LENGTH ];

	// Format status message
	wsprintf( lpszStatus, DOWNLOADING_STATUS_MESSAGE_FORMAT_STRING, lpszUrl );

	// Show status message on status bar window
	SendMessage( g_hWndStatusBar, SB_SETTEXT, ( WPARAM )NULL, ( LPARAM )lpszStatus );

	// Download file
	if( g_internet.Download( lpszUrl, lpszLocalFilePath ) )
	{
		// Successfully downloaded file
		File localFile;

		// Open local file
		if( localFile.CreateRead( lpszLocalFilePath ) )
		{
			// Successfully opened local file
			DWORD dwLocalFileSize;

			// Get local file size
			dwLocalFileSize = localFile.GetSize();

			// Ensure that local file size was got
			if( dwLocalFileSize != INVALID_FILE_SIZE )
			{
				// Successfully got local file size

				// Allocate string memory
				LPTSTR lpszLocalFileText = new char[ dwLocalFileSize + sizeof( char ) ];

				// Read local file text
				if( localFile.Read( lpszLocalFileText, dwLocalFileSize ) )
				{
					// Successfully read local file text
					int nTagCount;

					// Terminate local file text
					lpszLocalFileText[ dwLocalFileSize ] = ( char )NULL;

					// Display local file
					nTagCount = DisplayFile( lpszLocalFileText );

					// Format status message
					wsprintf( lpszStatus, SUCCESSFULLY_DOWNLOADED_FILE_STATUS_MESSAGE_FORMAT_STRING, lpszUrl, lpszLocalFilePath, nTagCount );

					// Update return value
					bResult = TRUE;

				} // End of successfully read local file text

				// Free string memory
				delete [] lpszLocalFileText;

			} // End of successfully got local file size
			else
			{
				// Unable to get local file size

				// Format status message
				wsprintf( lpszStatus, UNABLE_TO_GET_LOCAL_FILE_SIZE_STATUS_MESSAGE_FORMAT_STRING, lpszLocalFilePath );

			} // End of unable to get local file size

			// Close local file
			localFile.Close();

		} // End of successfully opened local file
		else
		{
			// Unable to open local file

			// Format status message
			wsprintf( lpszStatus, UNABLE_TO_OPEN_LOCAL_FILE_STATUS_MESSAGE_FORMAT_STRING, lpszLocalFilePath );

		} // End of unable to open local file

	} // End of successfully downloaded file
	else
	{
		// Unable to download file

		// Format status message
		wsprintf( lpszStatus, UNABLE_TO_DOWNLOAD_FILE_STATUS_MESSAGE_FORMAT_STRING, lpszUrl );

	} // End of unable to download file

	// Show status message on status bar window
	SendMessage( g_hWndStatusBar, SB_SETTEXT, ( WPARAM )NULL, ( LPARAM )lpszStatus );

	// Free string memory
	delete [] lpszLocalFilePath;
	delete [] lpszStatus;

	return bResult;

} // End of function DownloadFile

int ShowAboutMessage( HWND hWnd )
{
	int nResult = 0;

	MSGBOXPARAMS mbp;

	// Clear message box parameter structure
	ZeroMemory( &mbp, sizeof( mbp ) );

	// Initialise message box parameter structure
	mbp.cbSize		= sizeof( MSGBOXPARAMS );
	mbp.hwndOwner	= hWnd;
	mbp.hInstance	= GetModuleHandle( NULL );
	mbp.lpszText	= ABOUT_MESSAGE_TEXT;
	mbp.lpszCaption	= ABOUT_MESSAGE_CAPTION;
	mbp.dwStyle		= ( MB_OK | MB_USERICON );
	mbp.lpszIcon	= MAKEINTRESOURCE( IDI_APPLICATION_ICON );

	// Show message box
	nResult = MessageBoxIndirect( &mbp );

	return nResult;

} // End of function ShowAboutMessage

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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

			// Allocate string memory
			LPTSTR lpszUrl = new char[ STRING_LENGTH ];

			// Get instance
			hInstance = ( ( LPCREATESTRUCT )lParam )->hInstance;

			// Get font
			hFont = ( HFONT )GetStockObject( DEFAULT_GUI_FONT );

			// Set default url
			lstrcpy( lpszUrl, EDIT_WINDOW_DEFAULT_URL );

			// Open clipboard
			if( OpenClipboard( NULL ) )
			{
				// Successfully opened clipboard
				HANDLE hClipboard;

				// Get clipboard data
				hClipboard = GetClipboardData( CF_TEXT );

				// Ensure that clipboard data was got
				if( hClipboard )
				{
					// Successfully got clipboard data
					LPTSTR lpszClipboard;

					// Get text from clipboard
					lpszClipboard = ( LPTSTR )GlobalLock( hClipboard );

					// Ensure that text was got from clipboard
					if( lpszClipboard )
					{
						// Successfully got text from clipboard

						// Update url
						lstrcpy( lpszUrl, lpszClipboard );

						// Release clipboard
						GlobalUnlock( hClipboard );

					} // End of successfully got text from clipboard

				} // End of successfully got clipboard data

				// Close clipboard
				CloseClipboard();

			} // End of successfully opened clipboard

			// Create edit window
			g_hWndEdit = CreateWindowEx( EDIT_WINDOW_EXTENDED_STYLE, WC_EDIT, lpszUrl, EDIT_WINDOW_STYLE, 0, 0, 0, 0, hWnd, ( HMENU )NULL, hInstance, NULL );

			// Ensure that edit window was created
			if( g_hWndEdit )
			{
				// Successfully created edit window

				// Set edit window font
				SendMessage( g_hWndEdit, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

				// Create button window
				g_hWndButton = CreateWindowEx( BUTTON_WINDOW_EXTENDED_STYLE, WC_BUTTON, BUTTON_WINDOW_TEXT, BUTTON_WINDOW_STYLE, 0, 0, 0, 0, hWnd, ( HMENU )BUTTON_WINDOW_ID, hInstance, NULL );

				// Ensure that button window was created
				if( g_hWndButton )
				{
					// Successfully created button window

					// Set button window font
					SendMessage( g_hWndButton, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

					// Create list box window
					g_hWndListBox = CreateWindowEx( LIST_BOX_WINDOW_EXTENDED_STYLE, WC_LISTBOX, LIST_BOX_WINDOW_TEXT, LIST_BOX_WINDOW_STYLE, 0, 0, 0, 0, hWnd, ( HMENU )NULL, hInstance, NULL );

					// Ensure that list box window was created
					if( g_hWndListBox )
					{
						// Successfully created list box window

						// Set list box window font
						SendMessage( g_hWndListBox, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

						// Create status bar window
						g_hWndStatusBar = CreateWindowEx( STATUS_BAR_WINDOW_EXTENDED_STYLE, STATUSCLASSNAME, STATUS_BAR_WINDOW_TEXT, STATUS_BAR_WINDOW_STYLE, 0, 0, 0, 0, hWnd, ( HMENU )NULL, hInstance, NULL );

						// Ensure that status bar window was created
						if( g_hWndStatusBar )
						{
							// Successfully created status bar window

							// Set status bar window font
							SendMessage( g_hWndStatusBar, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

							// Select edit window text
							SendMessage( g_hWndEdit, EM_SETSEL, ( WPARAM )0, ( LPARAM )-1 );

							// Focus on edit window
							SetFocus( g_hWndEdit );

						} // End of successfully created status bar window

					} // End of successfully created list box window

				} // End of successfully created button window

			} // End of successfully created edit window

			// Free string memory
			delete [] lpszUrl;

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
			int nListBoxWindowHeight;
			int nEditWindowWidth;

			// Store client width and height
			nClientWidth	= ( int )LOWORD( lParam );
			nClientHeight	= ( int )HIWORD( lParam );

			// Size status bar window
			SendMessage( g_hWndStatusBar, WM_SIZE, ( WPARAM )NULL, ( LPARAM )NULL );

			// Get status window size
			GetWindowRect( g_hWndStatusBar, &rcStatus );

			// Calculate window sizes
			nStatusWindowHeight		= ( rcStatus.bottom - rcStatus.top );
			nListBoxWindowHeight	= ( nClientHeight - ( BUTTON_WINDOW_HEIGHT + nStatusWindowHeight ) + WINDOW_BORDER_HEIGHT );
			nEditWindowWidth		= ( ( nClientWidth - BUTTON_WINDOW_WIDTH ) + WINDOW_BORDER_WIDTH );

			// Move control windows
			MoveWindow( g_hWndEdit, 0, 0, nEditWindowWidth, BUTTON_WINDOW_HEIGHT, TRUE );
			MoveWindow( g_hWndButton, ( nEditWindowWidth - WINDOW_BORDER_WIDTH ), 0, BUTTON_WINDOW_WIDTH, BUTTON_WINDOW_HEIGHT, TRUE );
			MoveWindow( g_hWndListBox, 0, ( BUTTON_WINDOW_HEIGHT - WINDOW_BORDER_HEIGHT ), nClientWidth, nListBoxWindowHeight, TRUE );

			// Break out of switch
			break;

		} // End of a size message
		case WM_ACTIVATE:
		{
			// An activate message

			// Focus on edit window
			SetFocus( g_hWndEdit );

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

						// Add file path to list box window
						SendMessage( g_hWndListBox, LB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszFilePath );

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
					// A Button window command

					// Allocate string memory
					LPTSTR lpszUrl = new char[ STRING_LENGTH ];

					// Get url from edit window
					if( SendMessage( g_hWndEdit, WM_GETTEXT, ( WPARAM )STRING_LENGTH, ( LPARAM )lpszUrl ) )
					{
						// Successfully got url from edit window

						// Download file
						DownloadFile( lpszUrl );

					} // End of successfully got url from edit window

					// Free string memory
					delete [] lpszUrl;

					// Break out of switch
					break;

				} // End of a button window command
				case IDM_FILE_EXIT:
				{
					// A file exit command

					// Destroy window
					DestroyWindow( hWnd );

					// Break out of switch
					break;

				} // End of a file exit command
				case IDM_HELP_ABOUT:
				{
					// A help about command

					// Show about message
					ShowAboutMessage( hWnd );

					// Break out of switch
					break;

				} // End of a help about command
				default:
				{
					// Default command

					// See if command message is from edit window
					if( ( HWND )lParam == g_hWndEdit )
					{
						// Command message is from edit window

						// Select edit window notification code
						switch( HIWORD( wParam ) )
						{
							case EN_UPDATE:
							{
								// An edit window update command
								int nTextLength;

								// Get edit window text length
								nTextLength = SendMessage( g_hWndEdit, WM_GETTEXTLENGTH, ( WPARAM )NULL, ( LPARAM )NULL );

								// See if edit window contains text
								if( nTextLength > 0 )
								{
									// Edit window contains text

									// Enable button window
									EnableWindow( g_hWndButton, TRUE );

								} // End of edit window contains text
								else
								{
									// Edit window is empty

									// Disable button window
									EnableWindow( g_hWndButton, FALSE );

								} // End of edit window is empty

								// Break out of switch
								break;

							} // End of an edit window update command
							default:
							{
								// Default edit window notification code

								// Call default procedure
								lr = DefWindowProc( hWnd, uMsg, wParam, lParam );

								// Break out of switch
								break;

							} // End of default edit window notification code

						}; // End of selection for edit window notification code

					} // End of command message is from edit window
					else if( ( HWND )lParam == g_hWndListBox )
					{
						// Command message is from list box window

						// Select list box window notification code
						switch( HIWORD( wParam ) )
						{
							case LBN_DBLCLK:
							{
								// A list box window double click notification code
								int nSelectedItem;

								// Allocate string memory
								LPTSTR lpszSelected = new char[ STRING_LENGTH ];

								// Get selected item
								nSelectedItem = SendMessage( g_hWndListBox, LB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

								// Get selected item text
								if( SendMessage( g_hWndListBox, LB_GETTEXT, ( WPARAM )nSelectedItem, ( LPARAM )lpszSelected ) )
								{
									// Successfully got selected item text

									// Display selected item text
									MessageBox( hWnd, lpszSelected, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

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
								LPTSTR lpszSelected = new char[ STRING_LENGTH ];

								// Get selected item
								nSelectedItem = SendMessage( g_hWndListBox, LB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

								// Get selected item text
								if( SendMessage( g_hWndListBox, LB_GETTEXT, ( WPARAM )nSelectedItem, ( LPARAM )lpszSelected ) )
								{
									// Successfully got selected item text

									// Show selected item text on status bar window
									SendMessage( g_hWndStatusBar, SB_SETTEXT, ( WPARAM )NULL, ( LPARAM )lpszSelected );

								} // End of successfully got selected item text

								// Free string memory
								delete [] lpszSelected;

								// Break out of switch
								break;

							} // End of a list box window selection change notification code
							default:
							{
								// Default list box window notification code

								// Call default procedure
								lr = DefWindowProc( hWnd, uMsg, wParam, lParam );

								// Break out of switch
								break;

							} // End of default list box window notification code

						}; // End of selection for list box window notification code

					} // End of command message is from list box window
					else
					{
						// Command message is not from list box window

						// Call default procedure
						lr = DefWindowProc( hWnd, uMsg, wParam, lParam );

					} // End of command message is not from list box window

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
					ShowAboutMessage( hWnd );

					// Break out of switch
					break;

				} // End of a help about system command
				default:
				{
					// Default system command

					// Call default procedure
					lr = DefWindowProc( hWnd, uMsg, wParam, lParam );

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

			// Call default handler
			lr = DefWindowProc( hWnd, uMsg, wParam, lParam );

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
			TrackPopupMenu( GetSubMenu( hMenu, 0 ), ( TPM_LEFTALIGN | TPM_LEFTBUTTON ), LOWORD( lParam ), HIWORD( lParam ), 0, hWnd, NULL );

			// Break out of switch
			break;

		} // End of a context menu message
		case WM_CLOSE:
		{
			// A close message

			// Destroy window
			DestroyWindow( hWnd );

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
			lr = DefWindowProc( hWnd, uMsg, wParam, lParam );

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lr;

} // End of function WndProc

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow )
{
	MSG msg;

	// Clear message structure
	ZeroMemory( &msg, sizeof( msg ) );

	// Open internet
	if( g_internet.Open() )
	{
		// Successfully opened internet
		WNDCLASSEX wc;

		// Clear window class structure
		ZeroMemory( &wc, sizeof( wc ) );

		// Initialise window class structure
		wc.cbSize			= sizeof( WNDCLASSEX );
		wc.lpfnWndProc		= WndProc;
		wc.hInstance		= hInstance;
		wc.lpszClassName	= MAIN_WINDOW_CLASS_NAME;
		wc.style			= MAIN_WINDOW_CLASS_STYLE;
		wc.hIcon			= MAIN_WINDOW_CLASS_ICON;
		wc.hCursor			= MAIN_WINDOW_CLASS_CURSOR;
		wc.hbrBackground	= MAIN_WINDOW_CLASS_BACKGROUND;
		wc.lpszMenuName		= MAIN_WINDOW_CLASS_MENU_NAME;
		wc.hIconSm			= MAIN_WINDOW_CLASS_ICON_SMALL;

		// Register window class
		if( RegisterClassEx( &wc ) )
		{
			// Successfully registered window class
			HWND hWnd;

			// Create window
			hWnd = CreateWindowEx( MAIN_WINDOW_EXTENDED_STYLE, MAIN_WINDOW_CLASS_NAME, MAIN_WINDOW_TEXT, MAIN_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL );

			// Ensure that window was created
			if( hWnd )
			{
				// Successfully created window
				HMENU hMenuSystem;

				// Get system menu
				hMenuSystem = GetSystemMenu( hWnd, FALSE );

				// Add separator item to system menu
				InsertMenu( hMenuSystem, SYSTEM_MENU_SEPARATOR_ITEM_POSITION, ( MF_BYPOSITION | MF_SEPARATOR ), 0, NULL );

				// Add about item to system menu
				InsertMenu( hMenuSystem, SYSTEM_MENU_ABOUT_ITEM_POSITION, MF_BYPOSITION, IDM_HELP_ABOUT, SYSTEM_MENU_ABOUT_ITEM_TEXT );

				// Show window
				ShowWindow( hWnd, nCmdShow );

				// Update window
				UpdateWindow( hWnd );

				// Message loop
				while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
				{
					// Translate message
					TranslateMessage( &msg );

					// Dispatch message
					DispatchMessage( &msg );

				}; // End of message loop

			} // End of successfully created window
			else
			{
				// Unable to create window

				// Display error message
				MessageBox( NULL, UNABLE_TO_CREATE_WINDOW_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

			} // End of unable to create window

		} // End of successfully registered window class
		else
		{
			// Unable to register window class

			// Display error message
			MessageBox( NULL, UNABLE_TO_REGISTER_WINDOW_CLASS_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

		} // End of unable to register window class

		// Close internet
		g_internet.Close();

	} // End of successfully opened internet
	else
	{
		// Unable to open internet

		// Display error message
		MessageBox( NULL, INTERNET_CLASS_UNABLE_TO_OPEN_INTERNET_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

	} // End of unable to open internet

	return msg.wParam;

} // End of function WinMain