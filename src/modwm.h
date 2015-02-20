#ifndef MODWM_H
#define MODWM_H
#include <X11/Xlib.h>
#include <window.h>

struct modwm_Window;

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

/* STATE HANDLING */
void modwm_init_windowList(struct modwm_State* state);
int modwm_add_window(struct modwm_State* state,
                     struct modwm_Window* w);
void modwm_remove_window(struct modwm_State* state,
                         struct modwm_Window* w);



#endif /* MODWM_H */
