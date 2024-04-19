#include <emscripten.h>
#include <stdio.h>

namespace api {
void draw_line(int x0, int y0, int x1, int y1) {
  EM_ASM_({ draw_line($0, $1, $2, $3); }, x0, y0, x1, y1);
}

void draw_rect(int x, int y, int width, int height) {
  EM_ASM_({ draw_rect($0, $1, $2, $3); }, x, y, width, height);
}
} // namespace api
