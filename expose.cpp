#include "api.h"
#include "src/game.h"
#include <emscripten/bind.h>

using namespace emscripten;

void init( int width, int height ) { game::init( width, height ); }
void render() { game::render(); }
void handle_mouse( i32 x, i32 y ) { game::handle_mouse( x, y ); }

EMSCRIPTEN_BINDINGS( my_module ) {
    function( "init", init );
    function( "render", render );
    function( "handle_mouse", handle_mouse );
}
