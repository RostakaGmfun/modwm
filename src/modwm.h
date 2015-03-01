#ifndef MODWM_H
#define MODWM_H
#include <X11/Xlib.h>
#include <window.h>

struct modwm_Window;
struct modwm_Frame;
struct modwm_FrameStyle;

struct modwm_Root {
    Window root;
    Display* dpy;
};

struct modwm_WindowList {
    struct modwm_Window** windows;
    size_t num_windows;
    size_t list_size;
};

struct modwm_State {
    struct modwm_Root* root;
    struct modwm_WindowList* win_list;
    struct modwm_FrameStyle *frame_style;
};


/*GENERAL ROUTINES */
int modwm_init();
void modwm_run();
void modwm_cleanup();

/* WINDOW PROCEDURES */
struct modwm_Window* modwm_register_window(struct modwm_State *state,
                                           Window win);
/* Reparents window and creates frame around it(border and titlebar) */
struct modwm_Frame* modwm_frame_window(struct modwm_State *state,
                                          struct modwm_Window *window,
                                          struct modwm_FrameStyle *fstyle);

void modwm_unregister_window(struct modwm_State *state,
                             struct modwm_Window *win);

void modwm_unframe_window(struct modwm_State *state,
                          struct modwm_Window *win);
/* STATE HANDLING */
/* Initialises window list array */
void modwm_init_windowList(struct modwm_State* state);

/* Adds window to the window list */
int modwm_add_window(struct modwm_State* state,
                     struct modwm_Window* w);

/* Removes window from the window list  */
void modwm_remove_window(struct modwm_State* state,
                         struct modwm_Window* w);

/* Find window by its Window ID */      
struct modwm_Window* modwm_get_window(struct modwm_State *state,
                                      Window win);
struct modwm_Window* modwm_get_by_frame(struct modwm_State *state,
                                        Window frame);

#endif /* MODWM_H */
