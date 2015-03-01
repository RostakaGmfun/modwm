#ifndef FRAME_H
#define FRAME_H
#include <window.h>

struct modwm_FrameStyle {
    int bar_width;
    int border_width;
    int color1;
    int color2;
    char position[5]; /* NTIMC  */
};
struct modwm_Frame {
    Window f;
    Window bar;
    struct modwm_Window* window;
    struct modwm_FrameStyle *style;
} ;


struct modwm_FrameStyle* make_default_FrameStyle();
struct modwm_Frame* frame_create(struct modwm_State *state,
                                        struct modwm_Window *w, 
                                        struct modwm_FrameStyle *fstyle);
void frame_destroy(struct modwm_State *state,
                          struct modwm_Window *w);
int frame_make_bar(struct modwm_State *state,
                    struct modwm_Frame* frame);

#endif /* FRAME_H */
