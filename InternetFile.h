// InternetFile.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define INTERNET_FILE_UNABLE_TO_LOAD_ERROR_MESSAGE_FORMAT_STRING				"Unable to load %s"

BOOL InternetFileLoad( LPCTSTR lpszFileName );
