#ifndef PREPROCESSING_H_
#define PREPROCESSING_H_

#include <stdlib.h>
#include <SDL.h>

void init_sdl();
SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
void SDL_FreeSurface(SDL_Surface *surface);
void lightness(int n);
Image* Preprocessing(int p, char name[]);

#endif
