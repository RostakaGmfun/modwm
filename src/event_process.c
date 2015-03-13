#include <event_process.h>
#include <modwm.h>
#include <log.h>
#include <frame.h>

void on_create_notify(struct modwm_State *state, XEvent ev) {

}

void on_map_notify(struct modwm_State *state, XEvent ev) {

}

void on_configure_request(struct modwm_State *state, XEvent ev) {
    
}

void on_unmap_notify(struct modwm_State *state, XEvent ev) {
}

void on_destroy_notify(struct modwm_State *state, XEvent ev) {
    struct modwm_Window *w = NULL;
    char *window_name = NULL;
    if(ev.xdestroywindow.event==BadWindow)
        return;
    w = modwm_get_by_frame(state, ev.xdestroywindow.event);
   // if(!w) /* check if window registered */
    //    return;
    XFetchName(state->root->dpy,ev.xdestroywindow.event, &window_name);
    log("Window Name:%s\n",window_name);
    modwm_unframe_window(state, w);
    XDestroyWindow(state->root->dpy, ev.xdestroywindow.event);
    modwm_unregister_window(state, w);
}

void on_button_press(struct modwm_State *state, XEvent ev) {
    struct modwm_Window *win = NULL;
    char *window_name = NULL;
 
    win = modwm_get_by_frame(state, ev.xbutton.window);
    if(!win) { /* it is not frame - try checking if it is client window */
        win = modwm_find_window(state, ev.xbutton.window);
            if(!win)
                return;
            else
                XRaiseWindow(state->root->dpy, win->frame->f); 
    }
    else
        XRaiseWindow(state->root->dpy, ev.xbutton.window);
    /*XSetInputFocus(state->root->dpy, ev.xbutton.window,
                                    RevertToPointerRoot, CurrentTime);*/
}

void on_map_request(struct modwm_State *state, XEvent ev) {
    struct modwm_Window *win;
    struct modwm_Frame *frame;
    if(!modwm_find_window(state, ev.xmaprequest.window)) {
        win = modwm_register_window(state, ev.xmaprequest.window);
        frame = modwm_frame_window(state, win, state->frame_style);
        win->frame = frame;
    }
    XMapRaised(state->root->dpy, ev.xmaprequest.window);
}

