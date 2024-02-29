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

BOOL HtmlFileGetAttributeValue( LPCTSTR lpszTag, LPCTSTR lpszParentUrl, LPCTSTR lpszAttributeName, LPTSTR lpszAttributeValue )
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
				if( strstr( lpszRelativeAttributeValue, HTML_FILE_ABSOLUTE_IDENTIFIER ) )
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

} // End of function HtmlFileGetAttributeValue

BOOL HtmlFileGetTagName( LPCTSTR lpszTag, LPTSTR lpszTagName )
{
	BOOL bResult;

	int nEndOfTagName;

	// copy tag name
	lstrcpy( lpszTagName, ( lpszTag + sizeof( char ) ) );

	// Find end of tag name
	nEndOfTagName = strcspn( lpszTagName, HTML_FILE_END_OF_TAG_NAME_CHARACTERS );

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

} // End of function HtmlFileGetTagName

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
