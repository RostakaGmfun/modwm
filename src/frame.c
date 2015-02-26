#include <stdlib.h>
#include <string.h>

#include <frame.h>
#include <modwm.h>
#include <log.h>

struct modwm_FrameStyle* make_default_FrameStyle() {
    struct modwm_FrameStyle* fstyle = NULL;

    fstyle = malloc(sizeof(struct modwm_FrameStyle));
    fstyle->bar_width = 24;
    fstyle->border_width=6;
    fstyle->color1 = 0x292929;
    fstyle->color2 = 0x393939;
    strcpy(fstyle->position, "NTIMC");

    return fstyle;
}

struct modwm_Frame* window_create_frame(struct modwm_State *state,
                                        struct modwm_Window* w, 
                                        struct modwm_FrameStyle *fstyle) {
    XWindowAttributes attribs;
    struct modwm_Frame* frame;

    if(!w||!fstyle||!state)
        return NULL;

    /*get window size and poistion*/
    XGetWindowAttributes(state->root->dpy,w->region,&attribs);

    /* allocate memory for frame structure */
    frame = malloc(sizeof(struct modwm_Frame));
    frame->f = XCreateSimpleWindow(state->root->dpy,
                                   state->root->root,
                                    attribs.x, attribs.y,
                                    attribs.width+fstyle->border_width*2,
                                    attribs.height+fstyle->bar_width+fstyle->border_width,
                                    0,
                                    fstyle->color1,
                                    fstyle->color2);
    if(frame->f==BadWindow) {
        log_err("modwm_create_frame(): "
        "error creating frame window!\n");
        free(frame);
        return NULL;
    }

    log("Created window of size %ix%i at position %i %i\n",attribs.width, attribs.height,
        attribs.x, attribs.y);
    XReparentWindow(state->root->dpy, w->region, frame->f,fstyle->border_width,fstyle->bar_width);
    XMapWindow(state->root->dpy, frame->f);
    //XRaiseWindow(state->root->dpy, w->region);
    frame->style = fstyle;
    frame->window = w;

    XSelectInput(state->root->dpy,frame->f,
                SubstructureRedirectMask|
                SubstructureNotifyMask);

    XAddToSaveSet(state->root->dpy, w->region);

    return frame;
}
