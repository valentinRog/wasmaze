#include "api.h"
#include "src/Game.hpp"
#include <emscripten/bind.h>

using namespace emscripten;

static game::Game g;

void init( int width, int height ) { g.init( width, height ); }
void render() { g.render(); }
void handle_mouse( i32 x, i32 y ) { g.handle_mouse( x, y ); }

EMSCRIPTEN_BINDINGS( my_module ) {
    function( "init", init );
    function( "render", render );
    function( "handle_mouse", handle_mouse );
}
