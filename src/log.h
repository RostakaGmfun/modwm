#ifndef LOG_H
#define LOG_H
#include <X11/Xlib.h>

void log(const char *format, ...);
void log_err(const char *format, ...);
void print_xevent(XEvent *ev);

#endif /* LOG_H */
