// InternetFile.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define INTERNET_FILE_START_OF_TAG_CHARACTER									ASCII_LESS_THAN_CHARACTER
#define INTERNET_FILE_END_OF_TAG_CHARACTER										ASCII_GREATER_THAN_CHARACTER

#define INTERNET_FILE_COMMENT_PREFIX											"<!--"
#define INTERNET_FILE_COMMENT_SUFFIX											"-->"

#define INTERNET_FILE_END_OF_TAG_NAME_CHARACTERS								" >"

#define INTERNET_FILE_MAXIMUM_TAG_NAME_LENGTH									( STRING_LENGTH - sizeof( char ) )

#define INTERNET_FILE_ANCHOR_TAG_NAME											"a"
#define INTERNET_FILE_ANCHOR_TAG_HEADER											"Anchor"
#define INTERNET_FILE_ANCHOR_TAG_ATTRIBUTE_NAME									"href"

#define INTERNET_FILE_IMAGE_TAG_NAME											"img"
#define INTERNET_FILE_IMAGE_TAG_HEADER											"Image"
#define INTERNET_FILE_IMAGE_TAG_ATTRIBUTE_NAME									"src"

#define INTERNET_FILE_UNKNOWN_TAG_HEADER										"Unknown"

#define INTERNET_FILE_ABSOLUTE_IDENTIFIER										"://"

#define INTERNET_FILE_PROCESS_TAGS_FORMAT_STRING								"%s (%d tags)"

#define INTERNET_FILE_UNABLE_TO_LOAD_ERROR_MESSAGE_FORMAT_STRING				"Unable to load %s"

BOOL InternetFileGetAttributeValue( LPCTSTR lpszTag, LPCTSTR lpszParentUrl, LPCTSTR lpszAttributeName, LPTSTR lpszAttributeValue );

BOOL InternetFileGetTagName( LPCTSTR lpszTag, LPTSTR lpszTagName );

BOOL InternetFileLoad( LPCTSTR lpszFileName );

int InternetFileProcessTags( BOOL( *lpTagFunction )( LPCTSTR lpszTag ) );
