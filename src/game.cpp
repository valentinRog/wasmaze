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
  std::map<Vec2, std::set<Vec2>> conns;
};

static Maze maze;

void init(int width, int height) {}

void render() {
  api::draw_line(0, 0, 200, 200);
  api::draw_rect(20, 20, 200, 80);
}
} // namespace game
