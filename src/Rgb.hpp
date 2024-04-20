#pragma once

#include "type.h"

namespace game {
struct Rgb {
    u32 n;
    Rgb( u8 r, u8 g, u8 b ) : n( r << 16 | g << 8 | b ) {}
};
}