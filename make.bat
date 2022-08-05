cls

del *.exe

windres Resource.rc -o Resource.o

g++ -mwindows -m64 -static -Wall -Wextra Internet.cpp FileClass.cpp InternetClass.cpp Resource.o -o Internet.exe -lwininet
