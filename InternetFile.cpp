// InternetFile.cpp

#include "InternetFile.h"

// Global variables
static LPTSTR g_lpszFileText;
static DWORD g_dwFileSize;

BOOL InternetFileGetAttributeValue( LPCTSTR lpszTag, LPCTSTR lpszParentUrl, LPCTSTR lpszAttributeName, LPTSTR lpszAttributeValue )
{
	BOOL bResult = FALSE;

	LPTSTR lpszFoundAttributeName;

	// Find attribute name in tag
	lpszFoundAttributeName = strstr( lpszTag, lpszAttributeName );

	// Ensure that attribute name was found in tag
	if( lpszAttributeName )
	{
		// Successfully found attribute name in tag
		LPTSTR lpszInvertedComma;

		// Find first inverted comma after attribute name
		lpszInvertedComma = strchr( lpszFoundAttributeName, ASCII_INVERTED_COMMA_CHARACTER );

		// Ensure that inverted comma was found
		if( lpszInvertedComma )
		{
			// Successfully found inverted comma

			// Allocate string memory
			LPTSTR lpszRelativeAttributeValue = new char[ STRING_LENGTH ];

			// Copy text into relative attribute value
			lstrcpy( lpszRelativeAttributeValue, ( lpszInvertedComma + sizeof( char ) ) );

			// Find first inverted comma in relative attribute value
			lpszInvertedComma = strchr( lpszRelativeAttributeValue, ASCII_INVERTED_COMMA_CHARACTER );

			// Ensure that inverted comma was found
			if( lpszInvertedComma )
			{
				// Successfully found inverted comma
				LPTSTR lpszQuestionMark;

				// Terminate relative attribute value
				lpszInvertedComma[ 0 ] = ( char )NULL;

				// See if relative attribute value is absolute
				if( strstr( lpszRelativeAttributeValue, INTERNET_FILE_ABSOLUTE_IDENTIFIER ) )
				{
					// Relative attribute value is absolute

					// Use relative value as attribute value
					lstrcpy( lpszAttributeValue, lpszRelativeAttributeValue );

				} // End of relative attribute value is absolute
				else
				{
					// Relative attribute value is not absolute

					// Copy parent url into attribute value
					lstrcpy( lpszAttributeValue, lpszParentUrl );

					// See if relative url begins with a forward slash character
					if( lpszRelativeAttributeValue[ 0 ] == ASCII_FORWARD_SLASH_CHARACTER )
					{
						// Relative url begins with a forward slash character

						// Append relative value (after forward slash character) onto attribute value
						lstrcat( lpszAttributeValue, ( lpszRelativeAttributeValue + sizeof( char ) ) );

					} // End of relative url begins with a forward slash character
					else
					{
						// Relative url does not begin with a forward slash character

						// Append relative value onto attribute value
						lstrcat( lpszAttributeValue, lpszRelativeAttributeValue );

					} // End of relative url does not begin with a forward slash character

				} // End of relative attribute value is not absolute

				// Find first question mark in attribute value
				lpszQuestionMark = strchr( lpszAttributeValue, ASCII_QUESTION_MARK_CHARACTER );

				// See if first question mark was found in attribute value
				if( lpszQuestionMark )
				{
					// Successfully found first question mark in attribute value

					// Terminate attribute value at first question mark
					lpszQuestionMark[ 0 ] = ( char )NULL;

				} // End of successfully found first question mark in attribute value

				// Update return value
				bResult = TRUE;

			} // End of successfully found inverted comma
			else
			{
				// Unable to find inverted comma

				// Clear attribute value
				lpszAttributeValue[ 0 ] = ( char )NULL;

			} // End of unable to find inverted comma

			// Free string memory
			delete [] lpszRelativeAttributeValue;

		} // End of successfully found inverted comma
		else
		{
			// Unable to find inverted comma

			// Clear attribute value
			lpszAttributeValue[ 0 ] = ( char )NULL;

		} // End of unable to find inverted comma

	} // End of successfully found attribute name in tag
	else
	{
		// Unable to find attribute name in tag

		// Clear attribute value
		lpszAttributeValue[ 0 ] = ( char )NULL;

	} // End of unable to find attribute name in tag

	return bResult;

} // End of function InternetFileGetAttributeValue

BOOL InternetFileGetTagName( LPCTSTR lpszTag, LPTSTR lpszTagName )
{
	BOOL bResult;

	int nEndOfTagName;

	// copy tag name
	lstrcpy( lpszTagName, ( lpszTag + sizeof( char ) ) );

	// Find end of tag name
	nEndOfTagName = strcspn( lpszTagName, INTERNET_FILE_END_OF_TAG_NAME_CHARACTERS );

	// Ensure that end of tag name was found
	if( nEndOfTagName < lstrlen( lpszTagName ) )
	{
		// Successfully found end of tag name

		// Terminate tag name
		lpszTagName[ nEndOfTagName ] = ( char )NULL;

		// Update return value
		bResult = TRUE;

	} // End of successfully found end of tag name
	else
	{
		// Unable to find end of tag name

		// Clear tag name
		lpszTagName[ 0 ] = ( char )NULL;

	} // End of unable to find end of tag name

	return bResult;

} // End of function InternetFileGetTagName

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