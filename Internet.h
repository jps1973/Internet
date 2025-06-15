// Internet.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include <wininet.h>

#include "Ascii.h"
#include "Common.h"

#define INTERNET_USER_AGENT														"InetURL/1.0"

#define INTERNET_DOWNLOAD_BUFFER_LENGTH											1024

#define INTERNET_DOWNLOADING_STATUS_MESSAGE_FORMAT_STRING						"Downloading %s to %s (%d bytes)"

#define INTERNET_UNABLE_TO_DOWNLOAD_FILE_STATUS_MESSAGE							"[Unable to download file]"

#define INTERNET_UNABLE_TO_CONNECT_TO_INTERNET_ERROR_MESSAGE					"Unable to connect to internet"

BOOL InternetClose();

BOOL InternetConnect();

BOOL InternetDownloadFile( LPCTSTR lpszUrl, LPTSTR lpszLocalFilePath, BOOL( *lpStatusFunction )( LPCTSTR lpszStatusMessage ) );
