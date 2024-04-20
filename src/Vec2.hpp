#pragma once

#include "type.h"

namespace game {
struct Vec2 {
  i32 x;
  i32 y;
};

bool operator<(const Vec2 &lhs, const Vec2 &rhs) {
  if (lhs.x == rhs.x) {
    return lhs.y < rhs.y;
  }
  return lhs.x < rhs.x;
}

} // namespace game
