// Internet.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Resource.h"

#include "Common.h"

#include "FileClass.h"
#include "InternetClass.h"
#include "TreeViewWindow.h"

#define MAIN_WINDOW_CLASS_NAME													"Main Internet Window Class"

#define MAIN_WINDOW_CLASS_STYLE													0
#define MAIN_WINDOW_CLASS_ICON													LoadIcon( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDI_APPLICATION_ICON ) )
#define MAIN_WINDOW_CLASS_CURSOR												LoadCursor( NULL, IDC_ARROW )
#define MAIN_WINDOW_CLASS_BACKGROUND											( HBRUSH )( COLOR_APPWORKSPACE + 1 )
#define MAIN_WINDOW_CLASS_MENU_NAME												MAKEINTRESOURCE( IDR_MAIN_MENU )
#define MAIN_WINDOW_CLASS_ICON_SMALL											LoadIcon( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDI_APPLICATION_ICON ) )

#define MAIN_WINDOW_EXTENDED_STYLE												( WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES )
#define MAIN_WINDOW_STYLE														WS_OVERLAPPEDWINDOW
#define MAIN_WINDOW_TEXT														"Internet"

#define MAIN_WINDOW_MINIMUM_WIDTH												320
#define MAIN_WINDOW_MINIMUM_HEIGHT												200

#define LIST_BOX_WINDOW_EXTENDED_STYLE											0
#define LIST_BOX_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY )
#define LIST_BOX_WINDOW_TEXT													NULL

#define EDIT_WINDOW_EXTENDED_STYLE												0
#define EDIT_WINDOW_STYLE														( WS_CHILD | WS_VISIBLE | WS_BORDER )
#define EDIT_WINDOW_DEFAULT_URL													"http://www.google.com"

#define BUTTON_WINDOW_EXTENDED_STYLE											0
#define BUTTON_WINDOW_STYLE														( WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT | BS_VCENTER )
#define BUTTON_WINDOW_TEXT														"Go"
#define BUTTON_WINDOW_ID														WM_USER

#define BUTTON_WINDOW_WIDTH														30
#define BUTTON_WINDOW_HEIGHT													25

#define STATUS_BAR_WINDOW_EXTENDED_STYLE										0
#define STATUS_BAR_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP )
#define STATUS_BAR_WINDOW_TEXT													NULL

#define SYSTEM_MENU_SEPARATOR_ITEM_POSITION										5
#define SYSTEM_MENU_ABOUT_ITEM_POSITION											6
#define SYSTEM_MENU_ABOUT_ITEM_TEXT												"About"

#define ABOUT_MESSAGE_TEXT														"Internet\r\n"						\
																				"\r\n"								\
																				"Written by Jim Smith\r\n"			\
																				"\r\n"								\
																				"July 2022"

#define DOWNLOADING_STATUS_MESSAGE_FORMAT_STRING								"Downloading %s..."
#define SUCCESSFULLY_DOWNLOADED_FILE_STATUS_MESSAGE_FORMAT_STRING				"Successfully downloaded %s to %s (%d tags)"
#define UNABLE_TO_GET_LOCAL_FILE_SIZE_STATUS_MESSAGE_FORMAT_STRING				"Unable to get sixe of local file, %s"
#define UNABLE_TO_OPEN_LOCAL_FILE_STATUS_MESSAGE_FORMAT_STRING					"Unable to open local file, %s"
#define UNABLE_TO_DOWNLOAD_FILE_STATUS_MESSAGE_FORMAT_STRING					"Unable to download %s"

#define UNABLE_TO_REGISTER_WINDOW_CLASS_ERROR_MESSAGE							"Unable to Register Window Class"
#define UNABLE_TO_CREATE_WINDOW_ERROR_MESSAGE									"Unable to Create Window"