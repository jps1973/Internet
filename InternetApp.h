// InternetApp.h

#pragma once

#include "Ascii.h"
#include "Common.h"

#include "Resource.h"

#include "HtmlFile.h"
#include "Internet.h"

#include "ButtonWindow.h"
#include "EditWindow.h"
#include "ListViewWindow.h"
#include "StatusBarWindow.h"

#define MAIN_WINDOW_CLASS_NAME													"Main Internet Window Class"

#define MAIN_WINDOW_CLASS_STYLE													0
#define MAIN_WINDOW_CLASS_ICON_NAME												IDI_APPLICATION
#define MAIN_WINDOW_CLASS_ICON													LoadIcon( NULL, MAIN_WINDOW_CLASS_ICON_NAME )
#define MAIN_WINDOW_CLASS_CURSOR												LoadCursor( NULL, IDC_ARROW )
#define MAIN_WINDOW_CLASS_BACKGROUND											( HBRUSH )( COLOR_APPWORKSPACE + 1 )
#define MAIN_WINDOW_CLASS_MENU_NAME												MAKEINTRESOURCE( IDR_MAIN_MENU )
#define MAIN_WINDOW_CLASS_ICON_SMALL											LoadIcon( NULL, MAIN_WINDOW_CLASS_ICON_NAME )

#define MAIN_WINDOW_EXTENDED_STYLE												( WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES )
#define MAIN_WINDOW_STYLE														WS_OVERLAPPEDWINDOW
#define MAIN_WINDOW_TEXT														"Internet"

#define MAIN_WINDOW_MINIMUM_WIDTH												320
#define MAIN_WINDOW_MINIMUM_HEIGHT												200

#define SYSTEM_MENU_SEPARATOR_ITEM_POSITION										5
#define SYSTEM_MENU_ABOUT_ITEM_POSITION											6
#define SYSTEM_MENU_ABOUT_ITEM_TEXT												"About"

#define DEFAULT_URL																"https://news.sky.com"

#define DEFAULT_REQUIRED_TAG_NAME												HTML_FILE_IMAGE_TAG_NAME
#define DEFAULT_REQUIRED_ATTRIBUTE_NAME											HTML_FILE_IMAGE_TAG_SOURCE_ATTRIBUTE_NAME
#define DEFAULT_ATTRIBUTE_MUST_CONTAIN											EMPTY_STRING

#define ABOUT_MESSAGE_TEXT														"Internet\r\n"						\
																				"\r\n"								\
																				"Written by Jim Smith\r\n"			\
																				"\r\n"								\
																				"June 2025"

#define UNABLE_TO_REGISTER_MAIN_WINDOW_CLASS_ERROR_MESSAGE						"Unable to Register Main Window Class"
#define UNABLE_TO_CREATE_MAIN_WINDOW_ERROR_MESSAGE								"Unable to Create Main Window"

typedef enum
{
	INTERNET_EXECUTABLE_FILE_NAME_ARGUMENT = 0,
	INTERNET_TAG_NAME_ARGUMENT,
	INTERNET_ATTRIBUTE_NAME_ARGUMENT,
	INTERNET_ATTRIBUTE_MUST_CONTAIN_ARGUMENT,

	INTERNET_NUMBER_OF_ARGUMENTS

} INTERNET_ARGUMENTS;
