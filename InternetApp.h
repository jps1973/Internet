// InternetApp.h

#pragma once

#include <windows.h>

#include "Resource.h"

#include "Ascii.h"
#include "Common.h"

#include "ButtonWindow.h"
#include "EditWindow.h"
#include "StatusBarWindow.h"
#include "TreeViewWindow.h"

#include "Internet.h"
#include "InternetFile.h"

#define MAIN_WINDOW_CLASS_NAME													"Main Internet Window Class"

#define MAIN_WINDOW_CLASS_STYLE													0
#define MAIN_WINDOW_CLASS_ICON_NAME												IDI_APPLICATION
#define MAIN_WINDOW_CLASS_ICON													LoadIcon( NULL, MAIN_WINDOW_CLASS_ICON_NAME )
#define MAIN_WINDOW_CLASS_CURSOR												LoadCursor( NULL, IDC_ARROW )
#define MAIN_WINDOW_CLASS_BACKGROUND											( HBRUSH )( COLOR_WINDOW + 1 )
#define MAIN_WINDOW_CLASS_MENU_NAME												NULL

#define MAIN_WINDOW_EXTENDED_STYLE												WS_EX_CLIENTEDGE
#define MAIN_WINDOW_TITLE														"Jim's Internet Browser"
#define MAIN_WINDOW_STYLE														WS_OVERLAPPEDWINDOW

#define MAIN_WINDOW_MINIMUM_WIDTH												500
#define MAIN_WINDOW_MINIMUM_HEIGHT												400

#define SYSTEM_MENU_SEPARATOR_ITEM_POSITION										5
#define SYSTEM_MENU_ABOUT_ITEM_POSITION											6
#define SYSTEM_MENU_ABOUT_ITEM_TEXT												"About"

#define ABOUT_MESSAGE_TEXT														"Jim's Internet Browser\r\n"		\
																				"\r\n"								\
																				"Written by Jim Smith\r\n"			\
																				"\r\n"								\
																				"August 2024"

#define DOWNLOADING_INTERNET_FILE_STATUS_MESSAGE_FORMAT_STRING					"Downloading internet file %s ..."
#define LOADING_LOCAL_FILE_STATUS_MESSAGE_FORMAT_STRING							"Loading local file %s ..."
#define PROCESSING_TAGS_IN_LOCAL_FILE_STATUS_MESSAGE_FORMAT_STRING				"Processing tags in local file %s ..."

#define UNABLE_TO_CREATE_MAIN_WINDOW_ERROR_MESSAGE								"Unable to create main window"
#define UNABLE_TO_REGISTER_MAIN_WINDOW_CLASS_ERROR_MESSAGE						"Unable to register main window class"
