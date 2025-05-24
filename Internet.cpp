// Internet.cpp

#include "Internet.h"

// Global variables
HINTERNET g_hInternet;

BOOL InternetClose()
{
	// Close internet
	return InternetCloseHandle( g_hInternet );

} // End of function InternetClose

BOOL InternetConnect()
{
	BOOL bResult = FALSE;

	// Open internet
	g_hInternet = InternetOpen( INTERNET_USER_AGENT, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );

	// Ensure that internet was opened
	if( g_hInternet )
	{
		// Successfully opened internet

		// Update return value
		bResult = TRUE;

	} // End of successfully opened internet

	return bResult;

} // End of function InternetConnect

BOOL InternetDownloadFile( LPCTSTR lpszUrl, LPTSTR lpszLocalFilePath, BOOL( *lpStatusFunction )( LPCTSTR lpszStatusMessage ) )
{
	BOOL bResult = FALSE;

	// Get current folder into local file path
	if( GetCurrentDirectory( STRING_LENGTH, lpszLocalFilePath ) )
	{
		// Successfully got current folder into local file path
		HINTERNET hInternetFile;
		LPTSTR lpszLastForwardSlash;

		// Allocate string memory
		LPTSTR lpszShortUrl = new char[ STRING_LENGTH ];

		// Ensure that local file path ends with a back slash
		if( lpszLocalFilePath[ lstrlen( lpszLocalFilePath ) - sizeof( ASCII_BACK_SLASH_CHARACTER ) ] != ASCII_BACK_SLASH_CHARACTER )
		{
			// Local file path does not end with a back slash

			// Append a back slash onto local file path
			lstrcat( lpszLocalFilePath, ASCII_BACK_SLASH_STRING );

		} // End of local file path does not end with a back slash

		// Copy url
		lstrcpy( lpszShortUrl, lpszUrl );

		// Remove forward slash characters from end of short url
		while( lpszShortUrl[ lstrlen( lpszShortUrl ) - sizeof( char ) ] == ASCII_FORWARD_SLASH_CHARACTER )
		{
			// Remove forward slash character from end of short url
			lpszShortUrl[ lstrlen( lpszShortUrl ) - sizeof( char ) ] = ( char )NULL;

		}; // End of loop to remove forward slash characters from end of short url

		// Find last forward slash in url
		lpszLastForwardSlash = strrchr( lpszShortUrl, ASCII_FORWARD_SLASH_CHARACTER );

		// Ensure that last forward was found slash in url
		if( lpszLastForwardSlash )
		{
			// Successfully found last forward slash in url

			// Append file name onto local file path
			lstrcat( lpszLocalFilePath, ( lpszLastForwardSlash + sizeof( ASCII_FORWARD_SLASH_CHARACTER ) ) );

		} // End of successfully found last forward slash in url
		else
		{
			// Unable to find last forward slash in url

			// Append entire url onto local file path
			lstrcat( lpszLocalFilePath, lpszShortUrl );

		} // End of unable to find last forward slash in url

		// Open internet file
		hInternetFile = InternetOpenUrl( g_hInternet, lpszShortUrl, NULL, 0, 0, 0 );

		// Ensure that internet file was opened
		if( hInternetFile )
		{
			// Successfully opened internet file
			HANDLE hLocalFile;

			// Open local file
			hLocalFile = CreateFile( lpszLocalFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

			// Ensure that local file was opened
			if( hLocalFile != INVALID_HANDLE_VALUE )
			{
				// Successfully opened local file
				DWORD dwRead;
				DWORD dwWritten;
				DWORD dwTotalBytes = 0;
				CHAR cBuffer[ INTERNET_DOWNLOAD_BUFFER_LENGTH ];

				// Allocate string memory
				LPTSTR lpszStatusMessage = new char[ STRING_LENGTH + sizeof( char ) ];

				// Read internet file
				while( InternetReadFile( hInternetFile, cBuffer, INTERNET_DOWNLOAD_BUFFER_LENGTH, &dwRead ) )
				{
					// Ensure that data was read into buffer
					if( dwRead == 0 )
					{
						// No data was read into buffer

						// Break out of loop
						break;

					} // End of no data was read into buffer

					// Terminate buffer
					cBuffer[ dwRead ] = ( char )NULL;

					// Write buffer into local file
					WriteFile( hLocalFile, cBuffer, dwRead, &dwWritten, NULL );

					// Update total bytes
					dwTotalBytes += dwWritten;

					// Format status message
					wsprintf( lpszStatusMessage, INTERNET_DOWNLOADING_STATUS_MESSAGE_FORMAT_STRING, lpszUrl, lpszLocalFilePath, dwTotalBytes );

					// Show status message
					( *lpStatusFunction )( lpszStatusMessage );

				} // End of loop to read internet file

				// Update return value
				bResult = TRUE;

				// Close file
				CloseHandle( hLocalFile );

			} // End of successfully opened local file

			// Close internet file
			InternetCloseHandle( hInternetFile );

		} // End of successfully opened internet file

		// Free string memory
		delete [] lpszShortUrl;

	} // End of successfully got current folder into local file path

	return bResult;

} // End of function InternetDownloadFile
