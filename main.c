#include "lib.c"

int main() {
  struct Bitmap bitmap = alloc_bitmap(10, 10);
  draw_pixel(bitmap, 1, 4, 0xFF0000);
  draw_pixel(bitmap, 1, 3, 0x00FF00);
  write_file(bitmap, "out.bmp");
  free_bitmap(bitmap);

  return 0;
}
