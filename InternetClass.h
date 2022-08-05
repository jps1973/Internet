// InternetClass.h
//
// Note that the following text must be added to the end of the g++ command for this to work:
//  -lwininet

#pragma once

#include <windows.h>
#include <wininet.h>

#include "Common.h"

#define INTERNET_CLASS_USER_AGENT												"InetURL/1.0"

#define INTERNET_CLASS_DOWNLOAD_BUFFER_LENGTH									1024

#define INTERNET_CLASS_UNABLE_TO_OPEN_INTERNET_ERROR_MESSAGE					"Unable to open internet"

class Internet
{
public:
	Internet();
	~Internet();

	Internet& operator = ( HINTERNET hInternet );

	BOOL operator == ( HINTERNET hInternet );

	BOOL operator != ( HINTERNET hInternet );

	operator HINTERNET();

	BOOL Close();

	BOOL Download( LPCTSTR lpszUrl, LPTSTR lpszLocalFilePath );

	BOOL Open( LPCSTR lpszAgent = INTERNET_CLASS_USER_AGENT, DWORD dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG, LPCTSTR lpszProxy = NULL, LPCTSTR lpszProxyBypass = NULL, DWORD dwFlags = 0 );

protected:
	HINTERNET m_hInternet;

}; // End of class Internet
