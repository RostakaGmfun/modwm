#include <event_process.h>

void on_create_notify(struct modwm_State *state, XEvent ev) {

}

void on_map_notify(struct modwm_State *state, XEvent ev) {

}

void on_configure_request(struct modwm_State *state, XEvent ev) {
    
}

void on_unmap_notify(struct modwm_State *state, XEvent ev) {

}

void on_destroy_notify(struct modwm_State *state, XEvent ev) {

}

void on_button_press(struct modwm_State *state, XEvent ev) {

}

void on_map_request(struct modwm_State *state, XEvent ev) {
    XMapRaised(state->root->dpy, ev.xmaprequest.window);
}

