// HtmlFile.cpp

#include "HtmlFile.h"

// Global variables
HANDLE g_hFile;
LPTSTR g_lpszFileText;

void HtmlFileFreeMemory()
{
	// Free string memory
	delete [] g_lpszFileText;

} // End of function HtmlFileFreeMemory

BOOL HtmlFileRead( LPCTSTR lpszFilePath )
{
	BOOL bResult = FALSE;

	// Open file
	g_hFile = CreateFile( lpszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );

	// Ensure that file was opened
	if( g_hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully opened file
		DWORD dwFileSize;

		// Get file size
		dwFileSize = GetFileSize( g_hFile, NULL );

		// Ensure that file size was got
		if( dwFileSize != INVALID_FILE_SIZE )
		{
			// Successfully got file size
			DWORD dwRead;

			// Allocate string memory
			g_lpszFileText = new char[ dwFileSize + sizeof( char ) ];

			// Read file text
			if( ReadFile( g_hFile, g_lpszFileText, dwFileSize, &dwRead, NULL ) )
			{
				// Successfully read file text

				// Terminate file text
				g_lpszFileText[dwFileSize] = ( char )NULL;

				// Display file text
				MessageBox( NULL, g_lpszFileText, lpszFilePath, ( MB_OK | MB_ICONINFORMATION ) );

				// Update return value
				bResult = TRUE;

			} // End of successfully read file text

		} // End of successfully got file size

		// Close file
		CloseHandle( g_hFile );

	} // End of successfully opened file

	return bResult;

} // End of function HtmlFileRead