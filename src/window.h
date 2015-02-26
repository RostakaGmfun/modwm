#ifndef WINDOW_H
#define WINDOW_H
#include <X11/Xlib.h>

struct modwm_State;
struct modwm_Window;

struct modwm_Window {
    Window region;
    struct modwm_Frame *frame;
};

/* Creates modwm_Window structure from X11 Window and adds window
 * to modwm_State window list */
struct modwm_Window* window_register(struct modwm_State *state,
                                     Window region);
/* Removes window from window list and frees window structure */
void window_unregister(struct modwm_State *state,
                       struct modwm_Window *window);
#endif /* WINDOW_H */
