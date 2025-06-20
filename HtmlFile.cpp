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

BOOL HtmlFileGetAttributeUrl( LPCTSTR lpszParentUrl, LPCTSTR lpszTag, LPCTSTR lpszAttributeName, LPTSTR lpszAttributeUrl )
{
	BOOL bResult = FALSE;

	// Allocate string memory
	LPTSTR lpszAttributeValue = new char[ STRING_LENGTH + sizeof( char ) ];

	// Get attribute value
	if( HtmlFileGetAttributeValue( lpszTag, lpszAttributeName, lpszAttributeValue ) )
	{
		// Successfully got attribute value
		LPTSTR lpszQuestionMark;

		// Find any question marks in attribute value
		lpszQuestionMark = strchr( lpszAttributeValue, ASCII_QUESTION_MARK_CHARACTER );

		// See if there are any question marks in attribute value
		if( lpszQuestionMark )
		{
			// There are question marks in attribute value

			// Terminate attribute value at first question mark
			lpszQuestionMark[ 0 ] = ( char )NULL;

		} // End of there are question marks in attribute value

		// Ensure that attribute value is not empty
		if( lpszAttributeValue[ 0 ] )
		{
			// Attribute value is not empty

			// See if attribute value is an absolute url
			if( strstr( lpszAttributeValue, HTML_FILE_ABSOLUTE_URL_IDENTIFIER ) )
			{
				// Attribute value is an absolute url

				// Update attribute url
				lstrcpy( lpszAttributeUrl, lpszAttributeValue );

			} // End of attribute value is an absolute url
			else
			{
				// Attribute value is not an absolute url

				// Copy parent url into attribute url
				lstrcpy( lpszAttributeUrl, lpszParentUrl );

				// See if attribute value begins with a forward slash
				if( lpszAttributeValue[ 0 ] == ASCII_FORWARD_SLASH_CHARACTER )
				{
					// Attribute value begins with a forward slash

					// Append attribute value (after forward slash character) onto  attribute url
					lstrcat( lpszAttributeUrl, ( lpszAttributeValue + sizeof( char ) ) );

				} // End of attribute value begins with a forward slash
				else
				{
					// Attribute value does not begin with a forward slash

					// Append attribute value onto  attribute url
					lstrcat( lpszAttributeUrl, lpszAttributeValue );

				} // End of attribute value does not begin with a forward slash

			} // End of attribute value is not an absolute url

			// Update return value
			bResult = TRUE;

		} // End of attribute value is not empty

	} // End of successfully got attribute value

	// Free string memory
	delete [] lpszAttributeValue;

	return bResult;

} // End of function HtmlFileGetAttributeUrl

BOOL HtmlFileGetAttributeValue( LPCTSTR lpszTag, LPCTSTR lpszAttributeName, LPTSTR lpszAttributeValue )
{
	BOOL bResult = FALSE;

	int nOffset;

	// Find attribute name in tag
	nOffset = FindTextInString( lpszTag, lpszAttributeName );

	// Ensure that attribute name was found in tag
	if( nOffset >= 0 )
	{
		// Successfully found attribute name in tag

		// Find first non-space character after attribute name
		nOffset = GetNextNonSpace( lpszTag, ( nOffset + lstrlen( lpszAttributeName ) ) );

		// Ensure that first non-space character after attribute name is an equals sign
		if( lpszTag[ nOffset ] == ASCII_EQUALS_CHARACTER )
		{
			// First non-space character after attribute name is an equals sign

			// Find first non-space character after equals sign
			nOffset = GetNextNonSpace( lpszTag, ( nOffset + sizeof( char ) ) );

			// Ensure that first non-space character after equals sign is an apostrophe or inverted comma
			if( ( lpszTag[ nOffset ] == ASCII_APOSTROPHE_CHARACTER ) || ( lpszTag[ nOffset ] == ASCII_INVERTED_COMMA_CHARACTER ) )
			{
				// First non-space character after equals sign is an apostrophe or inverted comma
				char cAttributeDelimiter;
				LPTSTR lpszEndOfAttribute;

				// Store attribute delimiter
				cAttributeDelimiter = lpszTag[ nOffset ];

				// Store attribute
				lstrcpy( lpszAttributeValue, ( lpszTag + nOffset + sizeof( char ) ) );

				// Find end of attribute
				lpszEndOfAttribute = strchr( lpszAttributeValue, cAttributeDelimiter );

				// Ensure that end of attribute was found
				if( lpszEndOfAttribute )
				{
					// Successfully found end of attribute

					// Terminate attribute
					lpszEndOfAttribute[ 0 ] = ( char )NULL;

					// Update return value
					bResult = TRUE;

				} // End of successfully found end of attribute

			} // End of first non-space character after equals sign is an apostrophe or inverted comma

		} // End of first non-space character after attribute name is an equals sign

	} // End of successfully found attribute name in tag

	return bResult;

} // End of function HtmlFileGetAttributeValue

BOOL HtmlFileGetTagName( LPCTSTR lpszTag, LPTSTR lpszTagName )
{
	BOOL bResult = FALSE;

	int nEndOfTagName;

	// Find end of tag name
	nEndOfTagName = strcspn( lpszTag, HTML_FILE_END_OF_TAG_NAME_CHARACTERS );

	// Ensure that end of tag name was found
	if( nEndOfTagName )
	{
		// Successfully found end of tag name

		// <script>
		// 01234567
		//        |

		// Store tag name
		strncpy( lpszTagName, ( lpszTag + sizeof( HTML_FILE_START_OF_TAG_CHARACTER ) ), ( nEndOfTagName - sizeof( HTML_FILE_START_OF_TAG_CHARACTER ) ) );

		// Terminate tag name
		lpszTagName[ nEndOfTagName - sizeof( HTML_FILE_START_OF_TAG_CHARACTER ) ] = ( char )NULL;

		// Update return value
		bResult = TRUE;

	} // End of successfully found end of tag name

	return bResult;

} // End of function HtmlFileLoad

BOOL HtmlFileIsTagName( LPCTSTR lpszTag, LPCTSTR lpszRequiredTagName )
{
	BOOL bResult = FALSE;

	// Allocate string memory
	LPTSTR lpszTagName = new char[ STRING_LENGTH + sizeof( char ) ];

	// Get tag name
	if( HtmlFileGetTagName( lpszTag, lpszTagName ) )
	{
		// Successfully got tag name

		// See if tag has the required name
		if( lstrcmpi( lpszTagName, lpszRequiredTagName ) == 0 )
		{
			// Tag has the required name

			// Update return value
			bResult = TRUE;

		} // End of tag has the required name

	} // End of successfully got tag name

	return bResult;

} // End of function HtmlFileIsTagName

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

int HtmlFileProcessTags( LPCTSTR lpszRequiredTagName, LPCTSTR lpszRequiredAttributeName, LPCTSTR lpszAttributeMustContain, int( *lpTagFunction )( LPCTSTR lpszTag, LPCTSTR lpszRequiredTagName, LPCTSTR lpszRequiredAttributeName, LPCTSTR lpszAttributeMustContain ) )
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
			if( ( *lpTagFunction )( lpszTag, lpszRequiredTagName, lpszRequiredAttributeName, lpszAttributeMustContain ) >= 0 )
			{
				// Successfully called tag function

				// Update return value
				nResult ++;

			} // End of successfully called tag function

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
