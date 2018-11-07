#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdlib.h>
#include <SDL.h>

typedef struct {
	size_t width;
	size_t height;
	int pixels[];
} Image;


Image* Image_create (size_t height, size_t width);
void Image_destroy (Image* img);
void Image_fill (Image* img, SDL_Surface* img_sdl);
void Image_print (const Image* img);

#endif
