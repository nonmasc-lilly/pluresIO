# pluresIO
just some simple IO tools for linux and windows that I keep having to reimplment over and over

A simple example program for reference
```C
#include "io.h"

int main() {
  START;
  DESC _in = GETSTDIN,
       _out = GETSTDOUT;
  wts("Hello World! [tab to exit]\n", _out);
  int _break = 0;
  while(!_break) {
    char c = input(_in);
    switch(c) {
    case '\t': _break=1; break;
    default: wtc(c, _out); break;
    }
  }
  wtc('\n', _out);

  EXIT;
}
```
