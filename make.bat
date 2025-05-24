cls

del *.exe

windres Resource.rc -o Resource.o

g++ -mwindows -m64 -static -Wall -Wextra ^
 ^
 InternetApp.cpp ^
 ^
 HtmlFile.cpp ^
 Internet.cpp ^
 ^
 ButtonWindow.cpp ^
 EditWindow.cpp ^
 ListBoxWindow.cpp ^
 StatusBarWindow.cpp ^
 ^
 Resource.o ^
 ^
 -o InternetApp.exe -lwininet
