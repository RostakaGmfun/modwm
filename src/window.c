#include <window.h>
#include <stdlib.h>
#include <string.h>

#include <modwm.h>
#include <log.h>

struct modwm_FrameStyle* make_default_FrameStyle() {
    struct modwm_FrameStyle* fstyle = NULL;
    //log("asd\n");
    fstyle = malloc(sizeof(struct modwm_FrameStyle));
    fstyle->width = 8; /* 8px border default */
    /* Here you go! 50 shades of gray:D (Well, actually two only) */
    fstyle->color1 = 0x494949; /* warm gray */
    fstyle->color2 = 0x797979; /* more lighter shade of gray */
    /* However, they'are going to be interpolated */
    /* So, at 8px border you'll have 8 shades of gray. */
    /* That's the progress! */
    strcpy(fstyle->position, "NTIMC");

    return fstyle;
}

struct modwm_Frame* window_create_frame(struct modwm_Window* w, 
                                        struct modwm_FrameStyle *fstyle,
                                        struct  modwm_State *state) {
    XWindowAttributes attribs;
    struct modwm_Frame* frame;

    if(!w||!fstyle||!state)
        return NULL;

    log("ff\n");
    /*get window size and poistion*/
    XGetWindowAttributes(state->root->dpy,w->region,&attribs);

    log("aass\n");

    /* allocate memory for frame structure */
    frame = malloc(sizeof(struct modwm_Frame));
    frame->f = XCreateSimpleWindow(state->root->dpy,
                                   state->root->root,
                                    attribs.x, attribs.y,
                                    attribs.width,
                                    attribs.height,
                                    fstyle->width,
                                    fstyle->color1,
                                    fstyle->color2);
    if(frame->f==BadWindow) {
        log_err("modwm_create_frame(): "
        "error creating frame window!\n");
        free(frame);
        return NULL;
    }

    log("Craeted window of size %ix%i at position %i %i\n",attribs.width, attribs.height,
        attribs.x, attribs.y);
    XReparentWindow(state->root->dpy, w->region, frame->f,0,0);
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

struct modwm_Window* window_register(Window region, 
                         struct modwm_State *state) {
    struct modwm_Window* win = NULL;
    
    if(region==BadWindow)
        return NULL;
    win = malloc(sizeof(struct modwm_Window));
    win->region = region;
    win->frame = NULL;
    modwm_add_window(state, win);
    return win;
}

