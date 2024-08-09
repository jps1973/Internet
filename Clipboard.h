// Clipboard.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

BOOL ClipboardGetText( LPTSTR lpszClipboardText, DWORD dwMacimumTextLength = STRING_LENGTH );
