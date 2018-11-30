#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "image.h"
#include <stdlib.h>
#include <math.h>

Image* Image_create (size_t height, size_t width)
{
  // using calloc since it conveniently fills everything with zeroes
  Image* img = calloc(1, sizeof *img + sizeof(int[height][width]) );
  img->height = height;
  img->width = width;
  // ...
  return img;
}

void Image_destroy (Image* img)
{
  free(img);
}

void Image_fill (Image* img, SDL_Surface* img_sdl)
{
  int (*array_2D)[img->width] = (int(*)[img->width]) img->pixels;

  for(size_t height=0; height < img->height; height++)
  {
    for(size_t width=0; width < img->width; width++)
    {
	Uint32 pixel = get_pixel(img_sdl, width, height);
	Uint8 r, g, b;
	SDL_GetRGB(pixel, img_sdl->format, &r, &g, &b);
	array_2D[height][width] = r == 0 ? 1 : 0;
	
    }
  }
}

void Image_print (const Image* img)
{
  int (*array_2D)[img->width] = (int(*)[img->width]) img->pixels;

  for(size_t height=0; height < img->height; height++)
  {
    for(size_t width=0; width < img->width; width++)
    {
      printf("%d ", array_2D[height][width]); 
    }
    printf("\n");
  }
}

void Image_buildFromArray(Image* array, char name[])
{	
	SDL_Surface *surface;
	size_t w = array->width;
	size_t h = array->height;
	int (*array_2D)[w] = (int(*)[w]) array->pixels;
	surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	for(size_t height=0; height < h; height++)
  	{
    		for(size_t width=0; width < w; width++)
    		{
			Uint8 r, g, b;
			if(array_2D[height][width] == 1)
			{
				r = g = b = 0;
			}
			else
			{
				r = g = b = 255;
			}
			Uint32 pixel = SDL_MapRGB(surface->format, r, g, b);
			put_pixel(surface, width, height, pixel);
   		}
        }
        SDL_SaveBMP(surface, name);
	
}
