#include "io.h"

#ifdef _WIN32
    BOOL WINAPI __T(DWORD ctrl) {
        return TRUE;
    }
#endif

char input(DESC in) {
#ifdef _WIN32
    char c;
    DWORD bytes;
    ReadFile(in, &c, 1, &bytes, NULL);
    if(c == '\r') return '\n';
    return c;
#else
    char c;
    read(fileno(in), &c, 1);
    return c;
#endif
}

int wtc(char c, DESC out) {
#ifdef _WIN32
    DWORD len = 0;
    WriteFile(out, &c, 1, &len, 0);
    #ifdef CARRIAGE_AFTER_NL
        if(c == '\n') wtc('\r', out);
    #endif
    return len;
#else
    #ifdef BSP_IS_127
        if((int)c == 127) c = '\b';
    #endif
    write(fileno(out), &c, 1);
    return 0;
#endif
}

int wts(char *s, DESC out) {
    while(*s) {
        wtc(*s, out);
        s++;
    }
    return 0;
}

int cwtc(char c, int x, int y, DESC out) {
    cpos(x, y, out);
    wtc(c, out);
}

int cwts(char *s, int x, int y, DESC out) {
    cpos(x, y, out);
    wts(s, out);
}

int clear(DESC out) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(out, &csbi);
    COORD tl = {0, 0};
    DWORD len;
    FillConsoleOutputCharacterA(out, ' ', csbi.dwSize.X * csbi.dwSize.Y, tl, &len);
    FillConsoleOutputAttribute(out, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        csbi.dwSize.X * csbi.dwSize.Y, tl, &len);
    SetConsoleCursorPosition(out, tl);
#else
    wts("\033[H\033[J", out);
#endif
}

int cursorshowstate(int state, DESC out) {
#ifdef _WIN32
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(out, &info);
    info.bVisible = state;
    SetConsoleCursorInfo(out, &info);
#else
    switch(state) {
    case 0: wts("\033[?25l", out); break;
    case 1: wts("\033[?25h", out); break;
    }
#endif
    return 0;
}

int cpos(int x, int y, DESC out) {
#ifdef _WIN32
    COORD pos = {x, y};
    SetConsoleCursorPosition(out, pos);
#else
    char xn[12], yn[12];
    sprintf(xn, "%d;", y); sprintf(yn, "%df", x);
    char code[27] = "\33[";
    strcat(code, xn);
    strcat(code, yn);
    wts(code, out);
#endif
    return 0;
}

int consz(int *ret, DESC out) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(out, &csbi);
    ret[0] = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    ret[1] = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    struct winsize wsz;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &wsz);
    ret[0] = wsz.ws_col; ret[1] = wsz.ws_row;
#endif
    return 0;
}


