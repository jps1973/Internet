// HtmlFile.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define HTML_FILE_START_OF_TAG_CHARACTER										'<'
#define HTML_FILE_END_OF_TAG_CHARACTER											'>'

void HtmlFileFreeMemory();

int HtmlFileProcessTags( void( lpTagFunction )( LPTSTR lpszTag ) );

BOOL HtmlFileRead( LPCTSTR lpszFilePath );
