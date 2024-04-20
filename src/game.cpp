#include "api.h"
#include <map>
#include <set>

namespace game {
struct Vec2 {
  int x;
  int y;
};

bool operator<(const Vec2 &lhs, const Vec2 &rhs) {
  if (lhs.x == rhs.x) {
    return lhs.y < rhs.y;
  }
  return lhs.x < rhs.x;
}

struct Maze {
  int n_cell_x;
  int n_cell_y;
  int cell_w;
  std::map<Vec2, std::set<Vec2>> conns;
};

struct Screen {
  int width;
  int height;
};

static Screen screen;
static Maze maze;

void init(int width, int height) {
  screen = {.width = width, .height = height};
  maze.n_cell_x = 40;
  maze.n_cell_y = maze.n_cell_x * (static_cast<float>(screen.height) /
                                   static_cast<float>(screen.width));
  maze.cell_w = screen.width / maze.n_cell_x;
  for (int y(0); y < screen.height; y++) {
    for (int x(0); x < screen.width; x++) {
      maze.conns[Vec2{.x = x, .y = y}];
    }
  }
}

void render() {
  for (int y(0); y < maze.n_cell_y; y++) {
    for (int x(0); x < maze.n_cell_x; x++) {
      const auto cw = maze.cell_w;
      const auto &conns = maze.conns.at(Vec2{x, y});
      if (!maze.conns.count(Vec2{x - 1, y}) || !conns.count(Vec2{x - 1, y})) {
        api::draw_line(x * cw, y * cw, x * cw, (y + 1) * cw);
      }
      if (!maze.conns.count(Vec2{x + 1, y}) || !conns.count(Vec2{x + 1, y})) {
        api::draw_line((x + 1) * cw, y * cw, (x + 1) * cw, (y + 1) * cw);
      }
      if (!maze.conns.count(Vec2{x, y + 1}) || !conns.count(Vec2{x, y + 1})) {
        api::draw_line(x * cw, (y + 1) * cw, (x + 1) * cw, (y + 1) * cw);
      }
      if (!maze.conns.count(Vec2{x, y - 1}) || !conns.count(Vec2{x, y - 1})) {
        api::draw_line(x * cw, y * cw, (x + 1) * cw, y * cw);
      }
    }
  }
}
} // namespace game
