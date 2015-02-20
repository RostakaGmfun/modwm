#include <stdio.h>
#include <stdarg.h>
#include "log.h"

/* Taken from Xlib programming Manual ( chapter 8 ) */

static char *event_names[] = {
   "",
   "",
   "KeyPress",
   "KeyRelease",
   "ButtonPress",
   "ButtonRelease",
   "MotionNotify",
   "EnterNotify",
   "LeaveNotify",
   "FocusIn",
   "FocusOut",
   "KeymapNotify",
   "Expose",
   "GraphicsExpose",
   "NoExpose",
   "VisibilityNotify",
   "CreateNotify",
   "DestroyNotify",
   "UnmapNotify",
   "MapNotify",
   "MapRequest",
   "ReparentNotify",
   "ConfigureNotify",
   "ConfigureRequest",
   "GravityNotify",
   "ResizeRequest",
   "CirculateNotify",
   "CirculateRequest",
   "PropertyNotify",
   "SelectionClear",
   "SelectionRequest",
   "SelectionNotify",
   "ColormapNotify",
   "ClientMessage",
   "MappingNotify"
};

typedef struct {
    int use_fout;
    const char* log_fname;
} log_Props;

void log(const char *format, ...) {
    va_list args;
    va_start(args,format);
    vprintf(format,args);
    va_end(args);
}

void log_err(const char *format, ...) {
    va_list args;
    va_start(args,format);
    vfprintf(stderr,format,args);
    va_end(args);
}

void print_xevent(XEvent* ev) {
    if(!ev)
        return;
    //if(ev->type==DestroyNotify)
        log("XEvent: %s\n",event_names[ev->type]);
}
