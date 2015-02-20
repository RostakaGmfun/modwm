#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <modwm.h>
#include <log.h>
#include <event_process.h>
#include <window.h>

static int handle_wm_existance(Display *dpy, XErrorEvent *err_ev);
static int default_err_handle(Display *dpy, XErrorEvent *err_ev);

static int reparent_windows();

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
    if(reparent_windows()) {
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

int reparent_windows() {
    Window *children_return, root_return, parent_return;
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
        XFetchName(state->root->dpy,children_return[i], &window_name);
        log("Window #%i, Name:%s\n",i,window_name);
        window = window_register(children_return[i], state);
        //log("ass\n");
        fstyle =  make_default_FrameStyle();
        //log("asdd\n");
        frame = window_create_frame(window, fstyle, state);
        log("ssdd\n");
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

int modwm_add_window(struct modwm_State* state,
                     struct modwm_Window* w) {
    if(!state||!w)
        return 1;
    //log("asd\n");
    if(state->win_list->num_windows == 
       state->win_list->list_size) {
        //log("asd\n");
        state->win_list->list_size*=2;
        //log("asd\n");
        state->win_list->windows = realloc(state->win_list->windows,
                                    sizeof(struct modwm_WindowList)*
                                    state->win_list->list_size);
    }
    //log("asd\n");
    state->win_list->windows[state->win_list->num_windows++] = w;
    return 0;
}


