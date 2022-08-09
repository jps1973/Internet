// Html.cpp

#include "Html.h"

void HtmlGetAbsoluteUrl( LPCTSTR lpszOriginalUrl, LPCTSTR lpszParentUrl, LPTSTR lpszAbsoluteUrl )
{
	// See if original url is already absolute
	if( strstr( lpszOriginalUrl, HTML_ABSOLUTE_URL_IDENTIFIER ) )
	{
		// Original url is already absolute

		// Copy original url into absolute url
		lstrcpy( lpszAbsoluteUrl, lpszOriginalUrl );

	} // End of original url is already absolute
	else
	{
		// Original url is not already absolute

		// Copy parent url into absolute url
		lstrcpy( lpszAbsoluteUrl, lpszParentUrl );

		// See if original url begins with a forward slash character
		if( lpszOriginalUrl[ 0 ] == ASCII_FORWARD_SLASH_CHARACTER )
		{
			// Original url begins with a forward slash character

			// Append original url (after forward slash character) onto absolute url
			lstrcat( lpszAbsoluteUrl, ( lpszOriginalUrl + sizeof( ASCII_FORWARD_SLASH_CHARACTER ) ) );

		} // End of original url begins with a forward slash character
		else
		{
			// Original url does not begin with a forward slash character

			// Append original url onto absolute url
			lstrcat( lpszAbsoluteUrl, lpszOriginalUrl );

		} // End of original url does not begin with a forward slash character

	} // End of original url is not already absolute

} // End of function HtmlGetAbsoluteUrl

BOOL HtmlGetAttribute( LPCTSTR lpszTag, LPCTSTR lpszRequiredAttribute, LPTSTR lpszAttribute )
{
	BOOL bResult = FALSE;

	LPTSTR lpszFind;

	// Find required attribute in tag
	lpszFind = strstr( lpszTag, lpszRequiredAttribute );

	// Ensure that required attribute was found in tag
	if( lpszFind )
	{
		// Successfully found required attribute in tag

		// Find equals sign after required attribute
		lpszFind = strchr( lpszFind, ASCII_EQUALS_CHARACTER );

		// Ensure that equals sign was found after required attribute
		if( lpszFind )
		{
			// Successfully found equals sign after required attribute
			DWORD dwAttributeBufferLength;
			LPTSTR lpszStartOfAttribute;
			LPTSTR lpszEndOfAttribute;

			// Clear start and end of attribute
			lpszStartOfAttribute	= NULL;
			lpszEndOfAttribute		= NULL;

			// Find first non space character after equals sign
			for( lpszFind ++; lpszFind[ 0 ] <= ASCII_SPACE_CHARACTER; lpszFind ++ );

			// See if attribute begins with a comma or an inverted comma
			if( ( lpszFind[ 0 ] == ASCII_COMMA_CHARACTER ) || ( lpszFind[ 0 ] == ASCII_INVERTED_COMMA_CHARACTER ) )
			{
				// Attribute begins with a comma or an inverted comma

				// Set start of attribute
				lpszStartOfAttribute = ( lpszFind + sizeof( char ) );

				// Find end of attribute
				lpszEndOfAttribute = strchr( lpszStartOfAttribute, lpszFind[ 0 ] );

			} // End of attribute begins with a comma or an inverted comma
			else
			{
				// Attribute does not begin with a comma or an inverted comma

				// Set start of attribute
				lpszStartOfAttribute = lpszFind;

				// Find end of attribute
				lpszEndOfAttribute = strpbrk( lpszStartOfAttribute, HTML_TAG_NAME_SUFFIXES );

			} // End of attribute does not begin with a comma or an inverted comma

			// Ensure that start and end of attribute are valid
			if( lpszStartOfAttribute && lpszEndOfAttribute )
			{
				// Start and end of attribute are valid

				// Calculate attribute buffer length
				dwAttributeBufferLength = ( ( lpszEndOfAttribute - lpszStartOfAttribute ) + sizeof( char ) );

				// Get attribute
				lstrcpyn( lpszAttribute, lpszStartOfAttribute, dwAttributeBufferLength );

				// Update return value
				bResult = TRUE;

			} // End of start and end of attribute are valid

		} // End of successfully found equals sign after required attribute

	} // End of successfully found required attribute in tag

	return bResult;

} // End of function HtmlGetAttribute

