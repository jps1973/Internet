// InternetFile.cpp

#include "InternetFile.h"

// Global variables
static LPTSTR g_lpszFileText;
static DWORD g_dwFileSize;

BOOL InternetFileLoad( LPCTSTR lpszFileName )
{
	BOOL bResult = FALSE;

	HANDLE hFile;

	// Open file
	hFile = CreateFile( lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL) ;

	// Ensure that hile was opened
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

				// Terminate file text
				g_lpszFileText[ g_dwFileSize ] = ( char )NULL;

				// Update return value
				bResult = TRUE;

			} // End of successfully read file text

		} // End of successfully got file size

		// Close file
		CloseHandle( hFile );

	} // End of successfully opened file

	return bResult;

} // End of function InternetFileLoad

int InternetFileProcessTags( BOOL( *lpTagFunction )( LPCTSTR lpszTag ) )
{
	int nResult = 0;

	LPTSTR lpszStartOfTag;
	LPTSTR lpszEndOfTag;
	DWORD dwMaximumTagLength = STRING_LENGTH;
	DWORD dwTagLength;

	// Allocate string memory
	LPTSTR lpszTag = new char[ dwMaximumTagLength + sizeof( char ) ];

	// Find start of first tag
	lpszStartOfTag = strchr( g_lpszFileText, INTERNET_FILE_START_OF_TAG_CHARACTER );

	// Loop through all tags
	while( lpszStartOfTag )
	{
		// See if tag is a comment
		if( memcmp( lpszStartOfTag, INTERNET_FILE_COMMENT_PREFIX, lstrlen( INTERNET_FILE_COMMENT_PREFIX ) ) == 0 )
		{
			// Tag is a comment

			// Find end of tag
			lpszEndOfTag = strstr( lpszStartOfTag, INTERNET_FILE_COMMENT_SUFFIX );

			// Ensure that end of tag was found
			if( lpszEndOfTag )
			{
				// Successfully found end of tag

				// Calculate tag length
				dwTagLength = ( ( lpszEndOfTag - lpszStartOfTag ) + lstrlen( INTERNET_FILE_COMMENT_SUFFIX ) );

				// Ensure that tag is long enough
				if( dwTagLength > dwMaximumTagLength )
				{
					// Tag is not long enough

					// Free string memory
					delete [] lpszTag;

					// Update maximum tag length
					dwMaximumTagLength = dwTagLength;

					// Re-allocate string memory
					lpszTag = new char[ dwMaximumTagLength + sizeof( char ) ];

				} // End of tag is not long enough

				// Store tag
				lstrcpyn( lpszTag, lpszStartOfTag, ( dwTagLength + sizeof( char ) ) );

				// Process tag
				if( ( *lpTagFunction )( lpszTag ) )
				{
					// Successfully processed tag

					// Update return value
					nResult ++;

				} // End of successfully processed tag

				// Find start of next tag
				lpszStartOfTag = strchr( lpszEndOfTag, INTERNET_FILE_START_OF_TAG_CHARACTER );

			} // End of successfully found end of tag
			else
			{
				// Unable to find end of tag

				// Force exit from loop
				lpszStartOfTag = NULL;

			} // End of unable to find end of tag

		} // End of tag is a comment
		else
		{
			// Tag is not a comment

			// Find end of tag
			lpszEndOfTag = strchr( lpszStartOfTag, INTERNET_FILE_END_OF_TAG_CHARACTER );

			// Ensure that end of tag was found
			if( lpszEndOfTag )
			{
				// Successfully found end of tag

				// Calculate tag length
				dwTagLength = ( ( lpszEndOfTag - lpszStartOfTag ) + sizeof( char ) );

				// Ensure that tag is long enough
				if( dwTagLength > dwMaximumTagLength )
				{
					// Tag is not long enough

					// Free string memory
					delete [] lpszTag;

					// Update maximum tag length
					dwMaximumTagLength = dwTagLength;

					// Re-allocate string memory
					lpszTag = new char[ dwMaximumTagLength + sizeof( char ) ];

				} // End of tag is not long enough

				// Store tag
				lstrcpyn( lpszTag, lpszStartOfTag, ( dwTagLength + sizeof( char ) ) );

				// Process tag
				if( ( *lpTagFunction )( lpszTag ) )
				{
					// Successfully processed tag

					// Update return value
					nResult ++;

				} // End of successfully processed tag

				// Find start of next tag
				lpszStartOfTag = strchr( lpszEndOfTag, INTERNET_FILE_START_OF_TAG_CHARACTER );

			} // End of successfully found end of tag
			else
			{
				// Unable to find end of tag

				// Force exit from loop
				lpszStartOfTag = NULL;

			} // End of unable to find end of tag

		} // End of tag is not a comment

	}; // End of loop through all tags

	// Free string memory
	delete [] lpszTag;

	return nResult;

} // End of function InternetFileProcessTags