#include <event_process.h>
#include <modwm.h>
#include <log.h>

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
    w = modwm_get_by_frame(state, ev.xdestroywindow.window);
    log("asd2\n");
    if(!w) /* check if window registered */
        return;
    XFetchName(state->root->dpy,ev.xdestroywindow.window, &window_name);
    log("Window Name:%s\n",window_name);
    modwm_unframe_window(state, w);
    //XDestroyWindow(state->root->dpy, ev.xunmap.window);
    modwm_unregister_window(state, w);
}

void on_button_press(struct modwm_State *state, XEvent ev) {

}

void on_map_request(struct modwm_State *state, XEvent ev) {
    struct modwm_Window *win;
    struct modwm_Frame *frame;
    if(!modwm_find_window(state, ev.xmaprequest.window)) {
        win = modwm_register_window(state, ev.xmaprequest.window);
        frame = modwm_frame_window(state, win, state->frame_style);
    }
    XMapRaised(state->root->dpy, ev.xmaprequest.window);
}

