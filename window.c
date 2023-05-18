#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <err.h>

#define H 200
#define W 200
#define X 300
#define Y 200
#define B 3

static Display *dpy;
static unsigned int scr;
static Window root;
static Window child;


void handelKeypress(KeySym k){
  static int pressD = 0;

  if (k == XK_q){
    printf("Destroying the Window :3");
    XDestroyWindow(dpy, child);
    XCloseDisplay(dpy);
    exit(0);
  }

  if (k == XK_d){
    pressD = 1;
  }
  else if (pressD && k == XK_h){
    XWindowAttributes child_attr;
    XGetWindowAttributes(dpy, child, &child_attr);

    int nH = child_attr.height - 30;
    XResizeWindow(dpy, child, child_attr.width, nH);
    XFlush(dpy);
    pressD = 0;
  }
  else if (k == XK_b){
    pressD = 0;
    XWindowAttributes child_attr;
    XGetWindowAttributes(dpy, child, &child_attr);

    int nW = child_attr.width + 30;
    
    XResizeWindow(dpy, child, nW, child_attr.height);
    XFlush(dpy);
  }

  else if (k == XK_h){
    pressD = 0;
    XWindowAttributes child_attr;
    XGetWindowAttributes(dpy, child, &child_attr);

    int nH = child_attr.height + 30;

    XResizeWindow(dpy, child, child_attr.width, nH);
    XFlush(dpy);
  }

}

int main(){

  dpy = XOpenDisplay(NULL);

  if (!dpy) {
    err(1, "Can't create connection");
    return 1;
  }

  scr = DefaultScreen(dpy);
  root = RootWindow(dpy, scr);
  child = XCreateSimpleWindow(dpy, root, X, Y, W, H, B, BlackPixel(dpy, scr), WhitePixel(dpy, scr));

  XSelectInput(dpy, child, KeyPressMask); 
  XMapWindow(dpy, child);
  XFlush(dpy);

  while (1) {
    XEvent ev;
    XNextEvent(dpy, &ev);

    if (ev.type == KeyPress){
      char buf[1];
      KeySym key;
      XLookupString(&ev.xkey, buf, sizeof(buf), &key, NULL);
      handelKeypress(key);
    }
  }

}
