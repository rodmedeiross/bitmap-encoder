#include <stdio.h>
#include <stdlib.h>

typedef unsigned int t_color;

typedef struct Bitmap {
  int width;
  int height;
  t_color *pixels;
} Bitmap;

Bitmap alloc_bitmap(int width, int height) {
  t_color *pixels = malloc(width * height * sizeof(t_color));
  struct Bitmap bitmap = {.width = width, .height = height, .pixels = pixels};
  return bitmap;
}

void draw_pixel(struct Bitmap bitmap, int x, int y, t_color color) {
  if (x < 0 || x >= bitmap.width || y < 0 || y <= bitmap.height) {
    return;
  }
  bitmap.pixels[y * bitmap.width + x] = color;
}

int calc_size(int width, int height) {
  int row_length = width * 3; // RGB bytes
  row_length +=
      4 - (row_length % 4); // Which row px data must to be multiple 4bytes

  return row_length * height;
}

void write_header(FILE *file, int width, int height) {
  // Bitmap Magic Words
  fputs("BM", file);

  int file_size = 16 + 40 + calc_size(width, height);

  // file size bytes
  fwrite(&file_size, 4, 1, file);

  int reserved = 0;
  fwrite(&reserved, 4, 1, file);

  int offset = 14 + 40;
  fwrite(&offset, 4, 1, file);
}

void write_info_header(FILE *file, int width, int height) {
  int header_size = 40;
  int bit_planes = 1;
  int bit_per_pixel = 24;
  int blank = 0;
  int image_size = calc_size(width, height);

  fwrite(&header_size, 4, 1, file);
  fwrite(&width, 4, 1, file);
  fwrite(&height, 4, 1, file);
  fwrite(&bit_planes, 2, 1, file);
  fwrite(&bit_per_pixel, 2, 1, file);
  fwrite(&blank, 4, 1, file);
  fwrite(&image_size, 4, 1, file);
  fwrite(&blank, 16, 1, file);
}

void write_file(struct Bitmap bitmap, char *file) {
  FILE *opened_file = fopen(file, "wb");

  if (opened_file == NULL) {
    puts("Something went wrong");
    return;
  }

  write_header(opened_file, bitmap.width, bitmap.height);
  write_info_header(opened_file, bitmap.width, bitmap.height);

  fclose(opened_file);
}
