// FileClass.cpp

#include "FileClass.h"

File::File()
{
	// Initialise member variables
	::ZeroMemory( &m_hFile, sizeof( m_hFile ) );

} // End of function File::File
 
File::~File()
{
	// Clear member variables
	::ZeroMemory( &m_hFile, sizeof( m_hFile ) );

} // End of function File::~File

File& File::operator = ( HANDLE hFile )
{
	// Update member variables
	m_hFile = hFile;

	return *this;

} // End of function File::operator =

BOOL File::operator == ( HANDLE hFile )
{
	BOOL bResult = FALSE;

	// See if item equals member item
	if( hFile == m_hFile )
	{
		// Item equals member item

		// Update return value
		bResult = TRUE;

	} // End of item equals member item

	return bResult;

} // End of function File::operator ==

BOOL File::operator != ( HANDLE hFile )
{
	BOOL bResult = FALSE;

	// See if item is different to member item
	if( hFile != m_hFile )
	{
		// Item is different to member item

		// Update return value
		bResult = TRUE;

	} // End of item is different to member item

	return bResult;

} // End of function File::operator !=

File::operator HANDLE()
{
	return m_hFile;

} // End of function File::operator HANDLE()

BOOL File::Close()
{
	// Close file
	return ::CloseHandle( m_hFile );

} // End of function File::Close

BOOL File::Create( LPCTSTR lpszFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile )
{
	BOOL bResult = FALSE;

	// Create file
	m_hFile = ::CreateFile( lpszFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile );

	// Ensure that file was created
	if( m_hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully created file

		// Update return value
		bResult = TRUE;

	} // End of successfully created file

	return bResult;

} // End of function File::Create

BOOL File::CreateAppend( LPCTSTR lpszFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile )
{
	// Create file
	return Create( lpszFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile );

} // End of function File::CreateAppend

BOOL File::CreateRead( LPCTSTR lpszFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile )
{
	// Create file
	return Create( lpszFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile );

} // End of function File::CreateRead

BOOL File::CreateWrite( LPCTSTR lpszFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile )
{
	// Create file
	return Create( lpszFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile );

} // End of function File::CreateWrite

DWORD File::GetSize( LPDWORD lpFileSizeHigh )
{
	// Get file size
	return ::GetFileSize( m_hFile, lpFileSizeHigh );

} // End of function File::GetSize

BOOL File::Read( LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped )
{
	// Read from file
	return ::ReadFile( m_hFile, lpBuffer, dwNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped );

} // End of function File::Read

BOOL File::Write( LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped )
{
	// Write to file
	return ::WriteFile( m_hFile, lpBuffer, dwNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped );

} // End of function File::Write

/*
File::
{
} // End of function File::
*/