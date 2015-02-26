#ifndef MODWM_H
#define MODWM_H
#include <X11/Xlib.h>
#include <window.h>

struct modwm_Window;
struct modwm_Frame;

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
};


/*GENERAL ROUTINES */
int modwm_init();
void modwm_run();
void modwm_cleanup();

/* WINDOW PROCEDURES */
/* Reparents window and creates frame around it(border and titlebar) */
struct modwm_Frame* modwm_reparent_window(struct modwm_State *state,
                                          struct modwm_Window *window,
                                          struct modwm_FrameStyle *fstyle);

/* STATE HANDLING */
/* Initialises window list array */
void modwm_init_windowList(struct modwm_State* state);

/* Adds window to the window list */
int modwm_add_window(struct modwm_State* state,
                     struct modwm_Window* w);

/* Removes window from the window list  */
void modwm_remove_window(struct modwm_State* state,
                         struct modwm_Window* w);



#endif /* MODWM_H */
