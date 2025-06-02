// HtmlFile.cpp

#include "HtmlFile.h"

// Global variables
LPTSTR g_lpszFileText;

int HtmlFileDisplayText( LPCTSTR lpszTitle, HWND hWndParent )
{
	// Display file text
	return MessageBox( hWndParent, g_lpszFileText, lpszTitle, ( MB_OK | MB_ICONINFORMATION ) );

} // End of function HtmlFileDisplayText

void HtmlFileFreeMemory()
{
	// Free string memory
	delete [] g_lpszFileText;

} // End of function HtmlFileFreeMemory

BOOL HtmlFileLoad( LPCTSTR lpszFileName )
{
	BOOL bResult = FALSE;

	HANDLE hFile;

	// Open file
	hFile = CreateFile( lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );

	// Ensure that file was opened
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully opened file
		DWORD dwFileSize;

		// Get file size
		dwFileSize = GetFileSize( hFile, NULL );

		// Ensure that file size was got
		if( dwFileSize != INVALID_FILE_SIZE )
		{
			// Successfully got file size

			// Allocate string memory
			g_lpszFileText = new char[ dwFileSize + sizeof( char ) ];

			// Read file text
			if( ReadFile( hFile, g_lpszFileText, dwFileSize, NULL, NULL ) )
			{
				// Successfully read file text

				// Terminate file text
				g_lpszFileText[ dwFileSize ] = ( char )NULL;

				// Update return value
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

} // End of function HtmlFileLoad

int HtmlFileProcessTags( int( *lpTagFunction )( LPCTSTR lpszTag ) )
{
	int nResult = 0;

	LPTSTR lpszStartOfTag;
	LPTSTR lpszEndOfTag;
	DWORD dwTagLength;
	DWORD dwMaximumTagLength = STRING_LENGTH;

	// Allocate string memory
	LPTSTR lpszTag = new char[ dwMaximumTagLength + sizeof( char ) ];

	// Find start of first tag
	lpszStartOfTag = strchr( g_lpszFileText, HTML_FILE_START_OF_TAG_CHARACTER );

	// Loop through all tags
	while( lpszStartOfTag )
	{
		// Find end of tag
		lpszEndOfTag = strchr( lpszStartOfTag, HTML_FILE_END_OF_TAG_CHARACTER );

		// Ensure that end of tag was found
		if( lpszEndOfTag )
		{
			// Successfully found end of tag

			// Calculate tag length
			dwTagLength = ( ( lpszEndOfTag - lpszStartOfTag ) + ( sizeof( char ) + sizeof( char ) ) );

			// Ensure that tag length is not greater than maximum
			if( dwTagLength > dwMaximumTagLength )
			{
				// Tag length is greater than maximum

				// Free string memory
				delete [] lpszTag;

				// Update maximum tag length
				dwMaximumTagLength = dwTagLength;

				// Re-allocate string memory
				lpszTag = new char[ dwMaximumTagLength + sizeof( char ) ];

			} // End of tag length is greater than maximum

			// Store tag
			lstrcpyn( lpszTag, lpszStartOfTag, dwTagLength );

			// Call tag function
			( *lpTagFunction )( lpszTag );

			// Update return value
			nResult ++;

			// Find start of next tag
			lpszStartOfTag = strchr( lpszEndOfTag, HTML_FILE_START_OF_TAG_CHARACTER );

		} // End of successfully found end of tag
		else
		{
			// Unable to find end of tag

			// Force exit from loop
			lpszStartOfTag = NULL;

		} // End of unable to find end of tag

	}; // End of loop through all tags

	// Free string memory
	delete [] lpszTag;

	return nResult;

} // End of function HtmlFileProcessTags
