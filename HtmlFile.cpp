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

int HtmlFileProcessTags( void( lpTagFunction )( LPTSTR lpszTag ) )
{
	int nResult = 0;

	DWORD dwTagLength;
	DWORD dwMaximumTagLength = STRING_LENGTH;

	LPTSTR lpszStartOfTag;
	LPTSTR lpszEndOfTag;
	LPTSTR lpszEndOfComment;

	// Allocate string memory
	LPTSTR lpszTag = new char[ dwMaximumTagLength + sizeof( char ) ];

	// Find start of first tag
	lpszStartOfTag = strchr( g_lpszFileText, HTML_FILE_START_OF_TAG_CHARACTER );

	// Loop through all tags
	while( lpszStartOfTag )
	{
		// See if tag is a comment
		if( memcmp( lpszStartOfTag, HTML_FILE_COMMENT_PREFIX, lstrlen( HTML_FILE_COMMENT_PREFIX ) ) == 0 )
		{
			// Tag is a comment
	
			//Find end of comment
			lpszEndOfComment = strstr( lpszStartOfTag, HTML_FILE_COMMENT_SUFFIX );

			// Ensure that end of comment was found
			if( lpszEndOfComment )
			{
				// Successfully found end of comment

				// 0123456789
				//  <!-- -->
				//  |    | |
				//  1    6 8

				// Update end of tag
				lpszEndOfTag = ( ( lpszEndOfComment + lstrlen( HTML_FILE_COMMENT_SUFFIX ) ) );

			} // End of successfully found end of comment
			else
			{
				// Unable to find end of comment

				// Force exit from loop
				lpszStartOfTag = NULL;

			} // End of unable to find end of comment

		} // End of tag is a comment
		else
		{
			// Tag is not a comment

			// Find end of tag
			lpszEndOfTag = strchr( lpszStartOfTag, HTML_FILE_END_OF_TAG_CHARACTER );

		} // End of tag is not a comment

		// Ensure that end of tag was found
		if( lpszEndOfTag )
		{
			// Successfully found end of tag

			// 01234567890
			//   <qwer>
			//   |    |
			//   2    7

			// Calculate tag length
			dwTagLength = ( ( lpszEndOfTag - lpszStartOfTag ) + sizeof( char ) );

			// Ensure that tag is not empty
			if( dwTagLength > 0 )
			{
				// Tag is not empty

				// Ensure that tag string is long enough
				if( dwTagLength > dwMaximumTagLength )
				{
					// Tag string is not long enough

					// Free string memory
					delete [] lpszTag;

					// Update maximum tag length
					dwMaximumTagLength = dwTagLength;

					// Re-allocate string memory
					lpszTag = new char[ dwMaximumTagLength + sizeof( char ) ];

				} // End of tag string is not long enough

				// Store tag
				lstrcpyn( lpszTag, lpszStartOfTag, ( dwTagLength + sizeof( char ) ) );

				// Process tag
				( *lpTagFunction )( lpszTag );

				// Update return value
				nResult ++;

			} // End of tag is not empty

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

				// Update return value
				bResult = TRUE;

			} // End of successfully read file text

		} // End of successfully got file size

		// Close file
		CloseHandle( g_hFile );

	} // End of successfully opened file

	return bResult;

} // End of function HtmlFileRead