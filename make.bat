cls

del *.exe

windres Resource.rc -o Resource.o

g++ -mwindows -m64 -static -Wall -Wextra InternetApp.cpp ButtonWindow.cpp Clipboard.cpp EditWindow.cpp Internet.cpp InternetFile.cpp StatusBarWindow.cpp TreeViewWindow.cpp Resource.o -o Internet.exe -lwininet
