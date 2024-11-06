// HtmlFile.cpp

#include "HtmlFile.h"

// Global variables
static LPTSTR g_lpszFileText;
static LPTSTR g_lpszFileName;
static DWORD g_dwFileSize;

void HtmlFileClose()
{
	// Free string memory
	delete [] g_lpszFileText;
	delete [] g_lpszFileName;

} // End of function HtmlFileClose

BOOL HtmlFileDisplay( HWND hWndParent, UINT uType )
{
	// Display file information
	return MessageBox( hWndParent, g_lpszFileText, g_lpszFileName, uType );

} // End of function HtmlFileDisplay

BOOL HtmlFileOpen( LPCTSTR lpszFileName )
{
	BOOL bResult = FALSE;

	HANDLE hFile;

	// Open file
	hFile = CreateFile( lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );

	// Ensure that file was opened
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully opened file

		// Get file size
		g_dwFileSize = GetFileSize( hFile, NULL );

		// Ensure that file size was got
		if( g_dwFileSize != INVALID_FILE_SIZE )
		{
			// Successfully got file size

			// Allocate string memory
			g_lpszFileText = new char[ g_dwFileSize + sizeof( char ) ];

			// Read file text
			if( ReadFile( hFile, g_lpszFileText, g_dwFileSize, NULL, NULL ) )
			{
				// Successfully read file text

				// Allocate string memory
				g_lpszFileName = new char[ STRING_LENGTH ];

				// Terminate file text
				g_lpszFileText[ g_dwFileSize ] = ( char )NULL;

				// Store file name
				lstrcpy( g_lpszFileName, lpszFileName );

				// Update return valur
				bResult = TRUE;

			} // End of successfully read file text
			else
			{
				// Unable to read file text

				// Free string memory
				delete [] g_lpszFileText;

			} // End of unable to read file text

		} // End of successfully got file size

		// Close file
		CloseHandle( hFile );

	} // End of successfully opened file

	return bResult;

} // End of function HtmlFileOpen
