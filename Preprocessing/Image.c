struct Image {
	int width;
	int height;
	int pixels[][];
};


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

void Image_fill (Image* img)
{
  int (*array_2D)[img->width] = (double(*)[img->width]) img->values;

  for(size_t height=0; height < img->height; height++)
  {
    for(size_t width=0; width < img->width; width++)
    {
      array_2D[height][width] = (double)width; // whatever value that makes sense
    }
  }
}

void Image_print (const Image* img)
{
  int (*array_2D)[img->width] = (double(*)[img->width]) img->values;

  for(size_t height=0; height < img->height; height++)
  {
    for(size_t width=0; width < img->width; width++)
    {
      printf("%.5f ", array_2D[height][width]); 
    }
    printf("\n");
  }
}

int main (void)
{
  int h = 3;
  int w = 4;

  Image* Image = Image_create(h, w);
  Image_fill(Image); // fill with some garbage value
  Image_print(Image);
  Image_destroy(Image);
}
