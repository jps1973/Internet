// HtmlFile.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define HTML_FILE_START_OF_TAG_CHARACTER										ASCII_LESS_THAN_CHARACTER
#define HTML_FILE_END_OF_TAG_CHARACTER											ASCII_GREATER_THAN_CHARACTER

#define HTML_FILE_END_OF_TAG_NAME_CHARACTERS									" >"

#define HTML_FILE_IMAGE_TAG_NAME												"img"

#define HTML_FILE_PROCESS_TAGS_STATUS_MESSAGE_FORMAT_STRING						"%s (%d tags)"

int HtmlFileDisplayText( LPCTSTR lpszTitle, HWND hWndParent = NULL );

void HtmlFileFreeMemory();

BOOL HtmlFileGetTagName( LPCTSTR lpszTag, LPTSTR lpszTagName );

BOOL HtmlFileIsTagName( LPCTSTR lpszTag, LPCTSTR lpszRequiredTagName );

BOOL HtmlFileLoad( LPCTSTR lpszFileName );

int HtmlFileProcessTags( int( *lpTagFunction )( LPCTSTR lpszTag ) );

