#include <modwm.h>
#include <log.h>

int main(int argc, char **argv) {
    if(modwm_init()) {
        log_err("Initialisation error.\n");
        return -1;
    }
    modwm_run();
    modwm_cleanup();
    return 0;
}
