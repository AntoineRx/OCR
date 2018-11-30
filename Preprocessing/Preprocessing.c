#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "image.h"
#include <stdlib.h>
#include <math.h>

void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

void SDL_FreeSurface(SDL_Surface *surface);

void lightness(int n)
{
	n = n*20;
	SDL_Surface* image_surface;
    	SDL_Surface* screen_surface;
	image_surface = load_image("my_image.jpg");
	screen_surface = display_image(image_surface);
	int width = image_surface->w;
	int height = image_surface->h;
	for(int i = 0; i < height; i++)
	{	
		for(int j = 0; j < width; j++)
		{
			Uint32 pixel = get_pixel(image_surface, j, i);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			int average = (0.3*r + 0.59*g + 0.11*b);
			if(average + n > 255)
			{
				r=g=b= 255;
			}
			else if(average + n < 0)
			{	
				r=g=b=0;
			}
			else
			{
				r=g=b= average +n;
			}
			Uint32 pixel2 = SDL_MapRGB(image_surface->format, r, g, b);
			put_pixel(image_surface, j, i, pixel2);
		}
	}
	update_surface(screen_surface, image_surface);
	SDL_FreeSurface(image_surface);
	SDL_FreeSurface(screen_surface);
	SDL_SaveBMP(screen_surface, "luminosity.out");
}

Image* Preprocessing(int p, char name[])
{
	SDL_Surface* image_surface;
    	SDL_Surface* screen_surface;
	image_surface = load_image(name);
	screen_surface = display_image(image_surface);
	int width = image_surface->w;
	int height = image_surface->h;
	for(int i = 0; i < height; i++)
	{	
		for(int j = 0; j < width; j++)
		{
			Uint32 pixel = get_pixel(image_surface, j, i);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			int average = (r + g + b)/3;
			if(average <= (255*p/100))
			{		
				r = g = b = 0;
			}
			if(average > (255*p/100))
			{
				r = g = b = 255;
			}
			Uint32 pixel2 = SDL_MapRGB(image_surface->format, r, g, b);
			put_pixel(image_surface, j, i, pixel2);
		}
	}
	update_surface(screen_surface, image_surface);
	SDL_FreeSurface(image_surface);
	SDL_FreeSurface(screen_surface);
	SDL_SaveBMP(screen_surface, "blackandwhite.out");
	image_surface = load_image("blackandwhite.out");
	Image* Image2 = Image_create(height, width);
  	Image_fill(Image2, image_surface); 
  	return Image2;
	
}

int main(int argc, char* argv[])
{

    if(argc == 2)
    {
	
	Image* img = Preprocessing(75, argv[1]);
	printf("width = %zu\n", img->width);
	printf("height = %zu\n", img->height);
	Image_print(img);
	for(size_t i = 0; i < (img->width) * ( img->height);i++)
	{
		printf("%i ", img->pixels[i]);
	}	
    }
    else
    {
	errx(0, "plop");
    }
    
    return 0;
}

/*int main()
{
	
	Image* img = Image_create(2,2);
	img->pixels[0]= 0;
	img->pixels[1]= 1;
	img->pixels[2]= 1;
	img->pixels[3]= 0;
	Image_buildFromArray(img, "cac.out");
	
	return 0;
}*/
