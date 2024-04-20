#include "api.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <stack>

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

static void make_maze() {
  for (auto &[_, v] : maze.conns) {
    v.clear();
  }

  std::stack<Vec2> st;
  st.push(Vec2{0, 0});
  std::set<Vec2> visited;
  visited.insert(Vec2{0, 0});

  std::random_device rd;
  std::default_random_engine gen(rd());

  while (st.size()) {
    auto p = st.top();
    std::array<Vec2, 4> nps = {Vec2{p.x + 1, p.y}, Vec2{p.x - 1, p.y},
                               Vec2{p.x, p.y + 1}, Vec2{p.x, p.y - 1}};
    std::shuffle(nps.begin(), nps.end(), gen);
    bool any = false;
    for (const auto np : nps) {
      if (!maze.conns.count(np)) {
        continue;
      }
      if (maze.conns.at(p).count(np)) {
        continue;
      }
      if (visited.count(np)) {
        continue;
      }
      visited.insert(np);
      st.push(np);
      maze.conns[p].insert(np);
      maze.conns[np].insert(p);
      any = true;
      break;
    }
    if (!any) {
      st.pop();
    }
  }
}

void init(int width, int height) {
  screen = {.width = width, .height = height};
  maze.n_cell_x = 80;
  maze.n_cell_y = maze.n_cell_x * (static_cast<float>(screen.height) /
                                   static_cast<float>(screen.width));
  maze.cell_w =
      static_cast<float>(screen.width) / static_cast<float>(maze.n_cell_x);
  for (int y(0); y < maze.n_cell_y; y++) {
    for (int x(0); x < maze.n_cell_x; x++) {
      maze.conns[Vec2{.x = x, .y = y}];
    }
  }
}

void render() {
  make_maze();

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
