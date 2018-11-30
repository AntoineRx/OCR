# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: Preprocessing


Preprocessing: pixel_operations.o image.o Preprocessing.o

Preprocessing.o: pixel_operations.h image.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} Preprocessing

# END
