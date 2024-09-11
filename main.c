#include "lib.c"

int main() {
  struct Bitmap bitmap = alloc_bitmap(30, 40);
  draw_pixel(bitmap, 22, 33, 0x0e0e0e);
  write_file(bitmap, "out.bitmap");
  return 0;
}
