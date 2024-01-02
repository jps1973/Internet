// HtmlFile.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

void HtmlFileFreeMemory();

BOOL HtmlFileRead( LPCTSTR lpszFilePath );
