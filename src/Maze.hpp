#pragma once

#include <map>
#include <set>

#include "Vec2.hpp"
#include "type.h"

namespace game {

struct Maze {
    i32                                n_cell_x;
    i32                                n_cell_y;
    std::map< Vec2, std::set< Vec2 > > conns;

    void resetGrid() {
        conns.clear();
        for ( i32 y( 0 ); y < n_cell_y; y++ ) {
            for ( i32 x( 0 ); x < n_cell_x; x++ ) { conns[{ x, y }]; }
        }
    }

    f32 getCellWidth( i32 screen_width ) const {
        return static_cast< f32 >( screen_width )
               / static_cast< f32 >( n_cell_x );
    }
};
}
