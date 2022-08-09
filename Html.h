// Html.h

#pragma once

#include <windows.h>

#include "Common.h"

#define HTML_TAG_PREFIX															'<'
#define HTML_TAG_SUFFIX															'>'
#define HTML_TAG_NAME_SUFFIXES													" >"
#define HTML_TAG_TYPE_ANCHOR													"a"
#define HTML_TAG_TYPE_IMAGE														"img"
#define HTML_TAG_ATTRIBUTE_HREF													"href"
#define HTML_TAG_ATTRIBUTE_SOURCE												"src"
#define HTML_ABSOLUTE_URL_IDENTIFIER											"://"

void HtmlGetAbsoluteUrl( LPCTSTR lpszOriginalUrl, LPCTSTR lpszParentUrl, LPTSTR lpszAbsoluteUrl );

BOOL HtmlGetAttribute( LPCTSTR lpszTag, LPCTSTR lpszRequiredAttribute, LPTSTR lpszAttribute );
