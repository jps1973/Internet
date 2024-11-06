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

int HtmlFileProcessItems( LPCTSTR lpszRequiredTagName, LPCTSTR lpszRequiredAttributeName, LPCTSTR lpszParentUrl, BOOL( *lpProcessFunction )( LPCTSTR lpszAttributeValue ) )
{
	int nResult = 0;

	LPSTR lpStartOfTag;
	LPSTR lpEndOfTag;
	LPSTR lpEndOfTagName;
	LPSTR lpStartOfAttribute;
	LPSTR lpStartOfAttributeValue;
	LPSTR lpEndOfAttributeValue;
	DWORD dwTagLength;

	// Allocate string memory
	LPTSTR lpszTag						= new char[ HTML_FILE_MAXIMUM_TAG_LENGTH + sizeof( char ) ];
	LPTSTR lpszTagName					= new char[ HTML_FILE_MAXIMUM_TAG_LENGTH + sizeof( char ) ];
	LPTSTR lpszRelativeAttributeValue	= new char[ HTML_FILE_MAXIMUM_TAG_LENGTH + sizeof( char ) ];
	LPTSTR lpszAbsoluteAttributeValue	= new char[ HTML_FILE_MAXIMUM_TAG_LENGTH + sizeof( char ) ];

	// Find start of first tag
	lpStartOfTag = strchr( g_lpszFileText, HTML_FILE_START_OF_TAG_CHARACTER );

	// Loop through all tags
	while( lpStartOfTag )
	{
		// Find end of tag
		lpEndOfTag = strchr( lpStartOfTag, HTML_FILE_END_OF_TAG_CHARACTER );

		// Ensure that end of tag was found
		if( lpEndOfTag )
		{
			// Successfully found end of tag

			// Calculate tag length
			dwTagLength = ( ( lpEndOfTag - lpStartOfTag ) + sizeof( char ) );

			// Ensure that lag length is valid
			if( dwTagLength > HTML_FILE_MAXIMUM_TAG_LENGTH )
			{
				// Tag length is too big

				// Limit tag length to maximum value
				dwTagLength = HTML_FILE_MAXIMUM_TAG_LENGTH;

			} // End of tag length is too big

			// Store tag
			lstrcpyn( lpszTag, lpStartOfTag, ( dwTagLength + sizeof( char ) ) );

			// Set start of tag name
			lstrcpy( lpszTagName, ( lpszTag + sizeof( char ) ) );

			// Find end of tag name
			lpEndOfTagName = strpbrk( lpszTagName, HTML_FILE_END_OF_TAG_NAME_CHARACTERS );

			// Ensure that end of tag name was found
			if( lpEndOfTagName )
			{
				// Successfully found end of tag name

				// Terminate tag name
				lpEndOfTagName[ 0 ] = ( char )NULL;

				// See if tag has the required name
				if( lstrcmpi( lpszTagName, lpszRequiredTagName ) == 0 )
				{
					// Tag has the required name

					// Find attribute in tag
					lpStartOfAttribute = strstr( lpszTag, lpszRequiredAttributeName );

					// Ensure that attribute was found in tag
					if( lpStartOfAttribute )
					{
						// Successfully found attribute in tag

						// Find start of atttribute value
						lpStartOfAttributeValue = strpbrk( lpStartOfAttribute, HTML_FILE_EDGE_OF_ATTRIBUTE_VALUE_CHARACTERS );

						// Ensure that start of atttribute value was found
						if( lpStartOfAttributeValue )
						{
							// Successfully found start of atttribute value

							// Store relative attribute value
							lstrcpy( lpszRelativeAttributeValue, ( lpStartOfAttributeValue + sizeof( char ) ) );

							// Find end of relative attribute value
							lpEndOfAttributeValue = strpbrk( lpszRelativeAttributeValue, HTML_FILE_EDGE_OF_ATTRIBUTE_VALUE_CHARACTERS );

							// See if end of attribute value was found
							if( lpEndOfAttributeValue )
							{
								// Successfully found end of attribute value

								// Terminate attribute value
								lpEndOfAttributeValue[ 0 ] = ( char )NULL;
								
								// See if attribute value is absolute
								if( strstr( lpszRelativeAttributeValue, HTML_FILE_ABSOLUTE_URL_IDENTIFIER ) )
								{
									// Attribute value is absolute

									// Copy attribute into absolute attribute
									lstrcpy( lpszAbsoluteAttributeValue, lpszRelativeAttributeValue );

								} // End of attribute value is absolute
								else
								{
									// Attribute value is not absolute

									// Copy parent url into absolute attribute
									lstrcpy( lpszAbsoluteAttributeValue, lpszParentUrl );

									// Ensure that absolute attribute value ends with a forward slash
									if( lpszAbsoluteAttributeValue[ lstrlen( lpszAbsoluteAttributeValue ) - sizeof( char ) ] != ASCII_FORWARD_SLASH_CHARACTER )
									{
										// Absolute attribute value does not end with a forward slash

										// Append a forward slash onto absolute attribute value
										lstrcat( lpszAbsoluteAttributeValue, ASCII_FORWARD_SLASH_STRING );

									} // End of absolute attribute value does not end with a forward slash
									
									// See if relative attribute value begins with a forward slash
									if( lpszRelativeAttributeValue[ 0 ] == ASCII_FORWARD_SLASH_CHARACTER )
									{
										// Relative attribute value begins with a forward slash

										// Append relative attribute (after forward slash) onto absolute attribute value
										lstrcat( lpszAbsoluteAttributeValue, ( lpszRelativeAttributeValue + sizeof( char ) ) );

									} // End of relative attribute value begins with a forward slash
									else
									{
										// Relative attribute value does not begin with a forward slash

										// Append relative attribute onto absolute attribute value
										lstrcat( lpszAbsoluteAttributeValue, lpszRelativeAttributeValue );

									} // End of relative attribute value begins does not begin a forward slash

								} // End of attribute value is not absolute

								// Process item
								if( ( *lpProcessFunction )( lpszAbsoluteAttributeValue ) )
								{
									// Successfully processed item

									// Update return value
									nResult ++;

								} // End of successfully processed item

							} // End of successfully found end of attribute value

						} // End of successfully found start of atttribute value

					} // End of successfully found attribute in tag

				} // End of tag has the required name

			} // End of successfully found end of tag name

			// Find start of next tag
			lpStartOfTag = strchr( lpEndOfTag, HTML_FILE_START_OF_TAG_CHARACTER );

		} // End of successfully found end of tag
		else
		{
			// Unable to find end of tag

			// Force exit from loop
			lpStartOfTag = NULL;

		} // End of unable to find end of tag

	}; // End of loop through all tags

	// Free string memory
	delete [] lpszTag;
	delete [] lpszTagName;
	delete [] lpszRelativeAttributeValue;
	delete [] lpszAbsoluteAttributeValue;

	return nResult;

} // End of function HtmlFileProcessItems
