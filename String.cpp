// String.cpp

#include "String.h"

int FindTextInString( LPCTSTR lpszToSearch, LPCTSTR lpszToFind )
{
	int nResult = -1;

	LPTSTR lpszToSearchLower;
	LPTSTR lpszToFindLower;
	LPTSTR lpszFound;
	DWORD dwToSearchLength;
	DWORD dwToFindLength;

	// Store string lengths
	dwToSearchLength	= lstrlen( lpszToSearch );
	dwToFindLength		= lstrlen( lpszToFind );

	// Allocate string memory
	lpszToSearchLower	= new char[ dwToSearchLength	+ sizeof( char ) ];
	lpszToFindLower		= new char[ dwToFindLength		+ sizeof( char ) ];

	// Copy strings
	lstrcpy( lpszToSearchLower, lpszToSearch );
	lstrcpy( lpszToFindLower, lpszToFind );

	// Convert strings to lower case
	CharLower( lpszToSearchLower );
	CharLower( lpszToFindLower );

	// Find text
	lpszFound = strstr( lpszToSearchLower, lpszToFindLower );

	// Ensure that text was found
	if( lpszFound )
	{
		// Successfully found text

		// Update return value
		nResult = ( lpszFound - lpszToSearchLower );

	} // End of successfully found text

	return nResult;

} // End of function FindTextInString

int GetNextNonSpace( LPCTSTR lpszToSearch, int nOffset )
{
	int nResult = nOffset;

	// Find next non-space
	while( ( lpszToSearch[ nOffset ] == ASCII_SPACE_CHARACTER ) || ( lpszToSearch[ nOffset ] == ASCII_TAB_CHARACTER ) )
	{
		// Update return value
		nResult ++;

	}; // End of loop to find next non-space

	return nResult;

} // End of function GetNextNonSpace
