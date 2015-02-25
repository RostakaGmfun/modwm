#include <window.h>
#include <stdlib.h>
#include <string.h>

#include <modwm.h>
#include <log.h>


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

