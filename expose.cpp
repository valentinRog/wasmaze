#include "api.h"
#include "game.h"
#include <emscripten/bind.h>

using namespace emscripten;

void init(int width, int height) { game::init(width, height); }
void render() { game::render(); }

EMSCRIPTEN_BINDINGS(my_module) {
  function("init", init);
  function("render", render);
}
