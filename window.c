#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#define MH 768
#define MW 1366

typedef struct {
  u_int16_t px;
  u_int16_t py;
  u_int16_t height;
  u_int16_t width;
  u_int8_t border;
} Property;

Property pp;

typedef struct {
  Window c;
  Window m;
} Buttons;

typedef struct {
  int scr;
  Display *dpy;
  Window root;
  Window child;
  Buttons btns;
} bd26;

bd26 wm;

void init_wm() {
  wm.dpy = XOpenDisplay(NULL);
  if (!wm.dpy) {
    err(1, "Can't Create Connection\n");
    return;
  }
  wm.scr = DefaultScreen(wm.dpy);
  wm.root = RootWindow(wm.dpy, wm.scr);

  pp.border = 5;
  pp.px = 200;
  pp.py = 200;
  pp.height = 300;
  pp.width = 300;
}

void close_window() { XCloseDisplay(wm.dpy); }

void handle_close_button() {
  close_window();
  exit(0);
}
void handle_mx_button() {
  XWindowAttributes xwa;
  XGetWindowAttributes(wm.dpy, wm.child, &xwa);

  if (xwa.height != MH || xwa.width != MW)
    // XResizeWindow(wm.dpy, wm.child, MW, MH);
    XMoveResizeWindow(wm.dpy, wm.child, 0, 0, MW, MH);
  if (xwa.height == MH || xwa.width == MW)
    // XResizeWindow(wm.dpy, wm.child, pp.width, pp.height);
    XMoveResizeWindow(wm.dpy, wm.child, pp.px, pp.py, pp.height, pp.width);
  XFlush(wm.dpy);
}

void run_wm() {
  wm.child = XCreateSimpleWindow(
      wm.dpy, wm.root, pp.px, pp.py, pp.width, pp.height, pp.border,
      BlackPixel(wm.dpy, wm.scr), WhitePixel(wm.dpy, wm.scr));
  wm.btns.m = XCreateSimpleWindow(wm.dpy, wm.child, 0, 0, 40, 40, 0,
                                  BlackPixel(wm.dpy, wm.scr),
                                  BlackPixel(wm.dpy, wm.scr));
  wm.btns.c = XCreateSimpleWindow(wm.dpy, wm.child, 50, 0, 20, 20, 0,
                                  BlackPixel(wm.dpy, wm.scr),
                                  BlackPixel(wm.dpy, wm.scr));

  XMapWindow(wm.dpy, wm.child);
  XMapWindow(wm.dpy, wm.btns.c);
  XMapWindow(wm.dpy, wm.btns.m);

  XSelectInput(wm.dpy, wm.btns.m, ButtonPressMask);
  XSelectInput(wm.dpy, wm.btns.c, ButtonPressMask);
  XFlush(wm.dpy);
  while (1) {
    XEvent e;
    XNextEvent(wm.dpy, &e);
    if (e.type == ButtonPress) {
      XButtonEvent *newev = (XButtonEvent *)&e;

      if (newev->window == wm.btns.c)
        handle_close_button();
      if (newev->window == wm.btns.m)
        handle_mx_button();
    }
  }
}

int main() {
  init_wm();
  run_wm();
  close_window();
  return 0;
}
