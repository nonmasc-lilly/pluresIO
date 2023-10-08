#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <termios.h>
#endif

#ifndef IO_H

#ifdef _WIN32
    #define CARRIAGE_AFTER_NL
    BOOL WINAPI __T(DWORD ctrl);
    #define GETSTDOUT GetStdHandle(STD_OUTPUT_HANDLE)
    #define GETSTDIN  GetStdHandle(STD_INPUT_HANDLE)
    #define DESC HANDLE
    #define SETUP \
        SetConsoleMode(GETSTDIN, \
            ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));
    #define EXIT \
        SetConsoleMode(GETSTDIN, (ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));\
        ExitProcess(0)
    #define ONWIN32(x) x
#else
    #define BSP_IS_127
    #define GETSTDOUT stdout
    #define GETSTDIN stdin
    #define DESC FILE*
    #define SETUP \
        struct termios _OLDT;\
        do {struct termios _NEWT;\
        tcgetattr(STDIN_FILENO, &_OLDT);\
        _NEWT = _OLDT;\
        _NEWT.c_lflag &= ~(ICANON | ECHO);\
        tcsetattr(STDIN_FILENO, TCSANOW, &_NEWT);} while(0)
    #define EXIT \
        tcsetattr(STDIN_FILENO, TCSANOW, &_OLDT); exit(0)
    #define ONWIN32(x)
#endif

char input(DESC in);
int wtc(char c, DESC out);
int wts(char *s, DESC out);
int cwtc(char c, int x, int y, DESC out);
int cwts(char *s, int x, int y, DESC out);
int clear(DESC out);
int cputc(char c, int x, int y, DESC out);
int cursorshowstate(int state, DESC out);
int cpos(int x, int y, DESC out);
int consz(int *ret, DESC out);

#endif
