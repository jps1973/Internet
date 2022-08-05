// FileClass.h

#pragma once

#include <windows.h>

#include "Common.h"

class File
{
public:
	File();
	~File();

	File& operator = ( HANDLE hFile );

	BOOL operator == ( HANDLE hFile );

	BOOL operator != ( HANDLE hFile );

	operator HANDLE();

	BOOL Close();

	BOOL Create( LPCTSTR lpszFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile );

	BOOL CreateAppend( LPCTSTR lpszFileName, DWORD dwDesiredAccess = FILE_APPEND_DATA, DWORD dwShareMode = 0, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, DWORD dwCreationDisposition = OPEN_ALWAYS, DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL, HANDLE hTemplateFile = NULL );

	BOOL CreateRead( LPCTSTR lpszFileName, DWORD dwDesiredAccess = GENERIC_READ, DWORD dwShareMode = FILE_SHARE_READ, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, DWORD dwCreationDisposition = OPEN_EXISTING, DWORD dwFlagsAndAttributes = 0, HANDLE hTemplateFile = NULL );

	BOOL CreateWrite( LPCTSTR lpszFileName, DWORD dwDesiredAccess = GENERIC_WRITE, DWORD dwShareMode = 0, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, DWORD dwCreationDisposition = CREATE_ALWAYS, DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL, HANDLE hTemplateFile = NULL );

	DWORD GetSize( LPDWORD lpFileSizeHigh = NULL );

	BOOL Read( LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead = NULL, LPOVERLAPPED lpOverlapped = NULL );

	BOOL Write( LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten = NULL, LPOVERLAPPED lpOverlapped = NULL );

protected:
	HANDLE m_hFile;

}; // End of class File
