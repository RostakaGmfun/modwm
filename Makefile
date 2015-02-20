CC=gcc
CFLAGS=-std=c99 -c -Wall -I/home/rostyslav/Projects/modix/modwm/src/
LDFLAGS=-lX11
SOURCES=main.c src/modwm.c src/log.c src/error.c src/event_process.c src/window.c
OBJECTS=$(SOURCES:.c=.o)
DEPS=src/modwm.h src/log.h src/error.h src/event_process.h src/window.h
EXECUTABLE=bin/modwm

all: $(SOURCES) $(HEADERS) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) ;  $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o: ; $(CC) $(CFLAGS) $< -o $@

clean: ; for o in $(OBJECTS) rm o done
