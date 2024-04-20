#include <emscripten.h>

namespace api {
void draw_line( int x0, int y0, int x1, int y1, int color ) {
    EM_ASM_( { draw_line( $0, $1, $2, $3, $4 ); }, x0, y0, x1, y1, color );
}

void draw_rect( int x, int y, int width, int height, int color ) {
    EM_ASM_( { draw_rect( $0, $1, $2, $3, $4 ); }, x, y, width, height, color );
}
}