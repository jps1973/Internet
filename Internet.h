// Internet.h
//
// Note that the following text must be added to the end of the g++ command:
//  -lwininet
#pragma once

#include <windows.h>
#include <commctrl.h>
#include <wininet.h>

#include "Ascii.h"
#include "Common.h"

#define INTERNET_USER_AGENT														"InetURL/1.0"

#define INTERNET_DOWNLOAD_BUFFER_LENGTH											1024

#define INTERNET_DOWNLOADING_STATUS_MESSAGE_FORMAT_STRING						"Downloading %s ..."
#define INTERNET_SUCCESSFULLY_DOWNLOADED_STATUS_MESSAGE_FORMAT_STRING			"Successfully downloaded %s to %s"
#define INTERNET_UNABLE_TO_DOWNLOAD_STATUS_MESSAGE_FORMAT_STRING				"Unable to download %s"

#define INTERNET_UNABLE_TO_CONNECT_TO_INTERNET_ERROR_MESSAGE					"Unable to connect to internet"

BOOL InternetConnect( LPCSTR lpszAgent = INTERNET_USER_AGENT, DWORD dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG, LPCSTR lpszProxy = NULL, LPCSTR lpszProxyBypass = NULL, DWORD dwFlags = 0 );

BOOL InternetClose();

BOOL InternetDownloadFile( LPCTSTR lpszUrl, LPTSTR lpszLocalFilePath );
