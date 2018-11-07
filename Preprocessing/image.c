#include <err.h>
#include "image.h"
#include "pixel_operations.h"

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
