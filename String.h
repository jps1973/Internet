// String.h

#pragma once

#include "Ascii.h"
#include "Common.h"

#include "Resource.h"

#define STRING_SPACE_CHARACTERS													" \t"

int FindTextInString( LPCTSTR lpszToSearch, LPCTSTR lpszToFind );

int GetNextNonSpace( LPCTSTR lpszToSearch, int nOffset = 0 );
