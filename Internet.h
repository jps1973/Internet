// Internet.h
//
// Note that the following text must be added to the end of the g++ command (in the make.bat file) for this to work:
//
// -lwininet

#pragma once

#include <windows.h>
#include <commctrl.h>
#include <wininet.h>

#include "Ascii.h"
#include "Common.h"

#define INTERNET_USER_AGENT														"InetURL/1.0"

#define INTERNET_DOWNLOAD_BUFFER_LENGTH											1024

#define INTERNET_UNABLE_TO_OPEN_ERROR_MESSAGE									"Unable to open internet"

#define INTERNET_UNABLE_DOWNLOAD_FILE_FORMAT_STRING								"Unable to Download %s"

BOOL InternetDownloadFile( LPCTSTR lpszUrl );
