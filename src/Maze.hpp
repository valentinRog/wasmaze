#pragma once

#include <deque>
#include <map>
#include <set>
#include <vector>

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

    std::vector< Vec2 > getShortestPath() const {
        const Vec2                        pf = { n_cell_x - 1, n_cell_y - 1 };
        std::deque< std::vector< Vec2 > > q;
        std::set< Vec2 >                  visited;
        q.push_back( { { 0, 0 } } );
        visited.insert( { 0, 0 } );

        while ( true ) {
            auto path = q.front();
            q.pop_front();
            auto p = path.back();
            for ( const auto np : { Vec2{ p.x + 1, p.y },
                                    Vec2{ p.x - 1, p.y },
                                    Vec2{ p.x, p.y + 1 },
                                    Vec2{ p.x, p.y - 1 } } ) {
                if ( !conns.count( np ) ) { continue; }
                if ( visited.count( np ) ) { continue; }
                if ( !conns.at( p ).count( np ) ) { continue; }
                visited.insert( np );
                auto npath( path );
                npath.push_back( np );
                q.push_back( npath );
                if ( np == pf ) { return npath; }
            }
        }
    }
};
}
