// HtmlFile.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define HTML_FILE_START_OF_TAG_CHARACTER										'<'
#define HTML_FILE_END_OF_TAG_CHARACTER											'>'

#define HTML_FILE_END_OF_TAG_NAME_CHARACTERS									" >"

#define HTML_FILE_COMMENT_PREFIX												"<!--"
#define HTML_FILE_COMMENT_SUFFIX												"-->"

#define HTML_FILE_ANCHOR_TAG_NAME												"a"
#define HTML_FILE_ANCHOR_TAG_HEADER												"Anchor"
#define HTML_FILE_ANCHOR_TAG_ATTRIBUTE_NAME										"href"

#define HTML_FILE_UNKNOWN_TAG_HEADER											"Unknown"

void HtmlFileFreeMemory();

BOOL HtmlFileGetAttributeValue( LPCTSTR lpszTag, LPCTSTR lpszAttributeName, LPTSTR lpszAttributeValue );

BOOL HtmlFileGetTagName( LPCTSTR lpszTag, LPTSTR lpszTagName );

int HtmlFileProcessTags( void( lpTagFunction )( LPTSTR lpszTag ) );

BOOL HtmlFileRead( LPCTSTR lpszFilePath );
