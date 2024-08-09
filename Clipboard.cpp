// Clipboard.cpp

#include "Clipboard.h"

BOOL ClipboardGetText( LPTSTR lpszClipboardText, DWORD dwMacimumTextLength )
{
	BOOL bResult = FALSE;

	// Open clipboard
	if( OpenClipboard( NULL ) )
	{
		// Successfully opened clipboard
		HANDLE hClipboard;

		// Get cliboard data
		hClipboard = GetClipboardData( CF_TEXT );

		// Ensure that clipboard data was got
		if( hClipboard )
		{
			// Successfully got clipboard data

			// Allocate string memory
			LPTSTR lpszClipboard = static_cast<LPTSTR>( GlobalLock( hClipboard ) );

			// Ensure that string memory was allocated
			if( lpszClipboard )
			{
				// Successfully allocated string memory

				// Update clipboard text
				lstrcpyn( lpszClipboardText, lpszClipboard, dwMacimumTextLength );

				// Update return value
				bResult = TRUE;

				// Unlock global memory
				GlobalUnlock( hClipboard );

			} // End of successfully allocated string memory

		} // End of successfully got clipboard data

		// Close clipboard
		CloseClipboard();

	} // End of successfully opened clipboard

	return bResult;

} // End of function ClipboardGetText