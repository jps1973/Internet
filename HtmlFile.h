// HtmlFile.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define HTML_FILE_MAXIMUM_TAG_LENGTH											STRING_LENGTH

#define HTML_FILE_START_OF_TAG_CHARACTER										ASCII_LESS_THAN_CHARACTER
#define HTML_FILE_END_OF_TAG_CHARACTER											ASCII_GREATER_THAN_CHARACTER

#define HTML_FILE_END_OF_TAG_NAME_CHARACTERS									" >"
#define HTML_FILE_EDGE_OF_ATTRIBUTE_VALUE_CHARACTERS							"\"'"

#define HTML_FILE_ANCHOR_TAG_NAME												"a"
#define HTML_FILE_ANCHOR_TAG_ATTRIBUTE											"href"

#define HTML_FILE_SUCCESSFULLY_PROCESSED_STATUS_MESSAGE_FORMAT_STRING			"Successfully processed %d items in %s"
#define HTML_FILE_OPENING_STATUS_MESSAGE_FORMAT_STRING							"Opening %s ..."
#define HTML_FILE_SUCCESSFULLY_OPENED_STATUS_MESSAGE_FORMAT_STRING				"Successfully opened %s"
#define HTML_FILE_UNABLE_TO_OPEN_STATUS_MESSAGE_FORMAT_STRING					"Unable to open %s"

void HtmlFileClose();

BOOL HtmlFileDisplay( HWND hWndParent, UINT uType = ( MB_OK | MB_ICONINFORMATION ) );

BOOL HtmlFileOpen( LPCTSTR lpszFileName );

int HtmlFileProcessItems( LPCTSTR lpszRequiredTagName, LPCTSTR lpszRequiredAttributeName, BOOL( *lpProcessFunction )( LPCTSTR lpszAttributeValue ) );
