#ifndef WINDOW_H
#define WINDOW_H
#include <X11/Xlib.h>

struct modwm_State;
struct modwm_Window;

/*struct modwm_FrameStyle {
    int bar_width;
    int border_width;
    int color1;
    int color2;
    char position[5];/* /* NTIMC  */
//};
/*struct modwm_Frame {
    Window f;
    Window bar;
    struct modwm_Window* window;
    struct modwm_FrameStyle *style;
};*/

struct modwm_Window {
    Window region;
    struct modwm_Frame *frame;
};

/*struct modwm_FrameStyle* make_default_FrameStyle();
struct modwm_Frame* window_create_frame(struct modwm_Window *w, 
                         struct modwm_FrameStyle *fstyle,
                         struct modwm_State *state);
void window_destroy_frame(struct modwm_Window *w, struct modwm_State *state);
int window_make_bar(struct modwm_Frame* frame, struct modwm_State* state);*/
struct modwm_Window* window_register(Window region,
                            struct modwm_State *state);

#endif /* WINDOW_H */
