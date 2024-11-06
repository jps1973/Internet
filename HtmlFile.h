// HtmlFile.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define HTML_FILE_OPENING_STATUS_MESSAGE_FORMAT_STRING							"Opening %s ..."
#define HTML_FILE_SUCCESSFULLY_OPENED_STATUS_MESSAGE_FORMAT_STRING				"Successfully opened %s"
#define HTML_FILE_UNABLE_TO_OPEN_STATUS_MESSAGE_FORMAT_STRING					"Unable to open %s"

void HtmlFileClose();

BOOL HtmlFileDisplay( HWND hWndParent, UINT uType = ( MB_OK | MB_ICONINFORMATION ) );

BOOL HtmlFileOpen( LPCTSTR lpszFileName );
