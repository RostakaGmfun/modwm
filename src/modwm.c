#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <modwm.h>
#include <log.h>
#include <event_process.h>
#include <window.h>
#include <frame.h>

/*Special X11 error handler used to check for window manager existance */
static int handle_wm_existance(Display *dpy, XErrorEvent *err_ev);
/* Default modwm X11 error handler */
static int default_err_handle(Display *dpy, XErrorEvent *err_ev);
/* detects and reparent all existing windows (called in modwm_init() only once */
static int detect_reparent_windows();
/* Reads properties and atoms of windows */
static int read_windows();

static struct modwm_State *state = NULL;

int modwm_init() {
    
    state = malloc(sizeof(struct modwm_State));
    state->root = malloc(sizeof(struct modwm_Root));
    state->root->dpy = XOpenDisplay(NULL);
    if(!state->root->dpy) {
         log_err("%s %s:Error opening display\n",__FILE__,__LINE__);
         return 1;
    }
    state->root->root = DefaultRootWindow(state->root->dpy);
    modwm_init_windowList(state);
    XSetErrorHandler(handle_wm_existance);
    XSelectInput(state->root->dpy, state->root->root, 
        SubstructureNotifyMask| SubstructureRedirectMask |
        ButtonPress );
    XSync(state->root->dpy, true);
    XSetErrorHandler(default_err_handle);

    state->frame_style = make_default_FrameStyle();
    if(detect_reparent_windows()) {
        log_err("Failed to reparent windows\n");
        return 1;
    }

    return 0;
}

void modwm_run() {
    if(!state) {
    log_err("Unexpected error occured: state=NULL. Exiting\n");
    return;
    }
    while(1) {
        XEvent ev;
        XNextEvent(state->root->dpy, &ev);
        print_xevent(&ev);

        switch(ev.type) {
            case CreateNotify:
            on_create_notify(state, ev);
            break;
            case DestroyNotify:
            on_destroy_notify(state, ev);
            break;
            case MapNotify:
            on_map_notify(state, ev);
            break;
            case UnmapNotify:
            on_unmap_notify(state, ev);
            break;
            case ConfigureRequest:
            on_configure_request(state, ev);
            break;
            case MapRequest:
            on_map_request(state, ev);
            break;
            case ButtonPress:
            on_button_press(state, ev);
            break;
            default:
            break;
        }
    }
}

void modwm_cleanup() {
    
}

int handle_wm_existance(Display *dpy, XErrorEvent *err_ev) {
    if(err_ev)
        if(err_ev->error_code == BadAccess) {
            log_err("Only one window manager can run at the" 
            " same time\n"
            "Please disable currently runnning wm"
            " and run modwm again.\n");
            exit(-1);
        }
    return 0;
}

int detect_reparent_windows() {
    Window *children_return, 
            root_return, parent_return;
    unsigned int num_children = 0;
    char *window_name = NULL;

    struct modwm_Window* window;
    struct modwm_FrameStyle *fstyle;
    struct modwm_Frame *frame;
    
    if(!state) {
        log_err("reparent_windows(): state=NULL. Exiting\n");
        return 1;
    }
    XGrabServer(state->root->dpy);
    XQueryTree(state->root->dpy,
               state->root->root,
               &root_return,
               &parent_return,
               &children_return,
               &num_children);
    if(root_return!=state->root->root) {
        log_err("reparent_windows: wrong root returned.\n");
        XUngrabServer(state->root->dpy);
        return 1;
    }
    XUngrabServer(state->root->dpy);
    log("Number of windows: %i\n",num_children);
    for(int i = 0;i<num_children;i++) {
        window = modwm_register_window(state, children_return[i]);
        frame = modwm_frame_window(state, window, state->frame_style);
        window->frame = frame;
    }

    return 0;
}

int default_err_handle(Display *dpy, XErrorEvent *err_ev) {
    if(err_ev) {
        char err_text[200];
        XGetErrorText(state->root->dpy, err_ev->error_code,
        err_text, 199);
        log_err("%s",err_text);

        return 0;
    }
    return 0;
}

void modwm_init_windowList(struct modwm_State* state) {
    if(!state)
        return;
    static const size_t init_winlist_size = 4; 
    state->win_list = malloc(sizeof(struct modwm_WindowList));
    state->win_list->windows = malloc(sizeof(struct modwm_Window*)*
                                            init_winlist_size);
    state->win_list->num_windows = 0;
    state->win_list->list_size = init_winlist_size;
}

int modwm_add_window(struct modwm_State *state,
                         struct modwm_Window *w) {
    if(!state||!w)
        return 1;
    if(state->win_list->num_windows == 
       state->win_list->list_size) {
        state->win_list->list_size*=2;
        state->win_list->windows = realloc(state->win_list->windows,
                                    sizeof(struct modwm_WindowList)*
                                    state->win_list->list_size);
    }
    state->win_list->windows[state->win_list->num_windows++] = w;
    return 0;
}

void modwm_remove_window(struct modwm_State *state, 
                        struct modwm_Window *window) {
    struct modwm_WindowList *wl = NULL;
    if(!state||!window)
        return;
    wl = state->win_list;
    for(int i = 0; i<wl->num_windows;i++) {
        if(wl->windows[i]==window) {
            /* To remove element from array,
             * the last element is copied to 
             * the place of element to be removed
             * and array size is decremented */
            wl->windows[i] = wl->windows[wl->num_windows];
            wl->windows[wl->num_windows] = NULL;
            wl->num_windows--;
            break;
        }
    }
}

struct modwm_Window* modwm_find_window(struct modwm_State *state,
                                       Window win) {
    if(!state||win==BadWindow)
        return NULL;
    struct modwm_WindowList *wl = state->win_list;
    for(int i = 0;i<wl->num_windows;i++) {
        if(wl->windows[i]) { /* just to protect ourselves */
            if(wl->windows[i]->region==win)
                return wl->windows[i];
        }
    }
    return NULL;
}

struct modwm_Window* modwm_get_by_frame(struct modwm_State *state,
                                        Window f) {
    if(!state||f==BadWindow)
        return NULL;
   struct modwm_WindowList *wl = state->win_list;
    for(int i = 0;i<wl->num_windows;i++) {
        if(wl->windows[i]) {
            if(wl->windows[i]->frame)
                if(wl->windows[i]->frame->f==f) {
                    return wl->windows[i];
                }
        }
    }
    return NULL;
}

struct modwm_Window* modwm_register_window(struct modwm_State *state,
                                           Window win) {
    if(!state||win==BadWindow)
        return NULL;
    char *window_name = NULL;
    struct modwm_Window *window = window_register(state, win);
    XFetchName(state->root->dpy,win, &window_name);
    log("Registered Window:%s\n",window_name);

    return window;
}

struct modwm_Frame* modwm_frame_window(struct modwm_State *state,
                                          struct modwm_Window *window,
                                          struct modwm_FrameStyle *fstyle) {
    if(!state||!window||!fstyle)
        return NULL;
    struct modwm_Frame *frame = frame_create(state, window, fstyle);

    return frame;
}

void modwm_unframe_window(struct modwm_State *state,
                          struct modwm_Window *win) {
    if(!state|!win)
        return;
    frame_destroy(state, win);
    return;
}

void modwm_unregister_window(struct modwm_State *state,
                             struct modwm_Window *win) {
    if(!state||!win)
        return;
    window_unregister(state, win);
    return;
}

