// HtmlFile.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"
#include "String.h"

#define HTML_FILE_START_OF_TAG_CHARACTER										ASCII_LESS_THAN_CHARACTER
#define HTML_FILE_END_OF_TAG_CHARACTER											ASCII_GREATER_THAN_CHARACTER

#define HTML_FILE_END_OF_TAG_NAME_CHARACTERS									" >"

#define HTML_FILE_IMAGE_TAG_NAME												"img"
#define HTML_FILE_IMAGE_TAG_SOURCE_ATTRIBUTE_NAME								"src"

#define HTML_FILE_ABSOLUTE_URL_IDENTIFIER										"://"

#define HTML_FILE_PROCESS_TAGS_STATUS_MESSAGE_FORMAT_STRING						"%s (%d items)"

int HtmlFileDisplayText( LPCTSTR lpszTitle, HWND hWndParent = NULL );

void HtmlFileFreeMemory();

BOOL HtmlFileGetAttributeUrl( LPCTSTR lpszParentUrl, LPCTSTR lpszTag, LPCTSTR lpszAttributeName, LPTSTR lpszAttributeUrl );

BOOL HtmlFileGetAttributeValue( LPCTSTR lpszTag, LPCTSTR lpszAttributeName, LPTSTR lpszAttributeValue );

BOOL HtmlFileGetTagName( LPCTSTR lpszTag, LPTSTR lpszTagName );

BOOL HtmlFileIsTagName( LPCTSTR lpszTag, LPCTSTR lpszRequiredTagName );

BOOL HtmlFileLoad( LPCTSTR lpszFileName );

int HtmlFileProcessTags( LPCTSTR lpszRequiredTagName, LPCTSTR lpszRequiredAttributeName, LPCTSTR lpszAttributeMustContain, int( *lpTagFunction )( LPCTSTR lpszTag, LPCTSTR lpszRequiredTagName, LPCTSTR lpszRequiredAttributeName, LPCTSTR lpszAttributeMustContain ) );

