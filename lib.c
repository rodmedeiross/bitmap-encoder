#include <assert.h>
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
  Bitmap bitmap = {.width = width, .height = height, .pixels = pixels};
  return bitmap;
}

void free_bitmap(struct Bitmap bitmap) {
  free(bitmap.pixels);
  bitmap.pixels = NULL;
}

void draw_pixel(struct Bitmap bitmap, int x, int y, t_color color) {
  if (x < 0 || x >= bitmap.width || y < 0 || y >= bitmap.height) {
    printf("Get Out! X:%i, Y:%i\n", x, y);
    return;
  }

  bitmap.pixels[y * bitmap.width + x] = color;
}

int calc_row_size(int width) {
  int row_length = width * 3; // RGB bytes
  row_length +=
      4 - (row_length % 4); // Which row px data must to be multiple 4bytes
  assert(row_length % 4 == 0);
  return row_length;
}

int calc_size(int width, int height) { return calc_row_size(width) * height; }

void write_header(FILE *file, int width, int height) {
  // Bitmap Magic Words
  fputs("BM", file);

  int file_size = 14 + 40 + calc_size(width, height);

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
  int compression = 0;
  int image_size = calc_size(width, height);
  int x_p_meter = 0;
  int y_p_meter = 0;
  int colors_used = 0;
  int important_colors = 0;

  fwrite(&header_size, 4, 1, file);
  fwrite(&width, 4, 1, file);
  fwrite(&height, 4, 1, file);
  fwrite(&bit_planes, 2, 1, file);
  fwrite(&bit_per_pixel, 2, 1, file);
  fwrite(&compression, 4, 1, file);
  fwrite(&image_size, 4, 1, file);
  fwrite(&x_p_meter, 4, 1, file);
  fwrite(&y_p_meter, 4, 1, file);
  fwrite(&colors_used, 4, 1, file);
  fwrite(&important_colors, 4, 1, file);
}

void write_pixels(FILE *file, Bitmap bitmap) {
  int row_size = calc_row_size(bitmap.width);
  for (int y = bitmap.height - 1; y >= 0; y--) {
    for (int x = 0; x < bitmap.width; x++) {
      t_color color = bitmap.pixels[y * bitmap.width + x];
      unsigned char red = (color & 0xFF0000) >> 16;
      unsigned char green = (color & 0x00FF00) >> 8;
      unsigned char blue = color & 0x0000FF;
      fwrite(&blue, 1, 1, file);
      fwrite(&green, 1, 1, file);
      fwrite(&red, 1, 1, file);
    }

    // 5 * 3  = 15 + 1 = 16 -> 1px
    for (int i = bitmap.width * 3; i < row_size; i++) {
      fputc(0, file);
    }
  }
}

void write_file(Bitmap bitmap, char *file) {
  FILE *opened_file = fopen(file, "wb");

  if (opened_file == NULL)
    return;

  write_header(opened_file, bitmap.width, bitmap.height);
  write_info_header(opened_file, bitmap.width, bitmap.height);
  write_pixels(opened_file, bitmap);

  fclose(opened_file);
}
