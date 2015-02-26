#include <window.h>
#include <stdlib.h>
#include <string.h>

#include <modwm.h>
#include <log.h>


struct modwm_Window* window_register(struct modwm_State *state,
                                                 Window region) {
    struct modwm_Window* win = NULL;
    
    if(region==BadWindow)
        return NULL;
    win = malloc(sizeof(struct modwm_Window));
    win->region = region;
    win->frame = NULL;
    modwm_add_window(state, win);
    return win;
}

void window_unregister(struct modwm_State *state,
                       struct modwm_Window *window) {

    if(!window||!state)
        return;
    modwm_remove_window(state, window);
}

