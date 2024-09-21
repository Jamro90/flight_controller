#ifndef _TERMINAL_CONTROLL_
#define _TERMINAL_CONTROLL_

#include <sys/signal.h>
#include <sys/ioctl.h>

#define ESC "\033["
#define ESC_CL "\x1b[38;2;%d;%d;%dm"
#define ESC_BK "\x1b[48;2;%d;%d;%dm"

#define HEX_2_RGB(hex) ((hex >> 16 ) & 0xFF), ((hex >> 8) & 0xFF), ((hex >> 0) & 0xFF)

// color theme from:
// https://lospec.com/palette-list/zx-spectrum-adjusted
#define CL_BLACK            0x000000
#define CL_DARK_BLUE        0x0022c7
#define CL_BLUE             0x002bfb
#define CL_DARK_RED         0xd62816
#define CL_RED              0xff331c
#define CL_VIOLET           0xd433c7
#define CL_PINK             0xff40fc
#define CL_DARK_GREEN       0x00c525
#define CL_GREEN            0x00f92f
#define CL_GOLD             0xccc82a
#define CL_YELLOW           0xfffc36

void clearScreen(void);
void setCursor(int row, int col);
struct winsize terminalSize(int *rows, int *cols);

#endif // _TERMINAL_CONTROLL_

#ifndef _TERMINAL_CONTROLL_IMPLEMENTATION
#define _TERMINAL_CONTROLL_IMPLEMENTATION

void clearScreen(void) {
  fprintf(stdout, ESC "1;J");
}

void setCursor(int row, int col) {
  fprintf(stdout, ESC"%d;%dH", col, row);
}

struct winsize terminalSize(int *rows, int *cols) {
  struct winsize winSize;
  ioctl(0, TIOCGWINSZ, &winSize);
  *cols = winSize.ws_col;
  *rows = winSize.ws_row;
  return winSize;
}

#endif // _TERMINAL_CONTROLL_IMPLEMENTATION
