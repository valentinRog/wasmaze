#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <stack>

#include "Maze.hpp"
#include "MazeBuilder.hpp"
#include "Rgb.hpp"
#include "Vec2.hpp"
#include "api.h"
#include "type.h"

namespace game {

struct Screen {
    i32 width;
    i32 height;
};

static Screen      screen;
static MazeBuilder mb;

void init( i32 width, i32 height ) {
    screen           = { .width = width, .height = height };
    mb.maze.n_cell_x = 70;
    mb.maze.n_cell_y = mb.maze.n_cell_x
                       * ( static_cast< f32 >( screen.height )
                           / static_cast< f32 >( screen.width ) );
    mb.maze.resetGrid();
}

const i32 step_by_sec = 100;

void render() {
    api::draw_rect( 0, 0, screen.width, screen.height, Rgb( 0, 0, 0 ).n );
    static auto t  = std::chrono::high_resolution_clock::now();
    auto        dt = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::high_resolution_clock::now() - t );

    int steps = ( static_cast< float >( dt.count() ) / 1000.f )
                * ( static_cast< float >( step_by_sec ) );
    for ( auto i( 0 ); i < steps; i++ ) {
        mb.step();
        t = std::chrono::high_resolution_clock::now();
    }
    const auto  cw   = mb.maze.getCellWidth( screen.width );
    const auto &maze = mb.maze;
    const auto  pos  = mb.getPos();
    if ( pos.has_value() ) {
        api::draw_rect( pos->x * cw, pos->y * cw, cw, cw, Rgb( 200, 0, 0 ).n );
    } else {
        for ( const auto &p : maze.getShortestPath() ) {
            api::draw_rect( p.x * cw, p.y * cw, cw, cw, Rgb( 0, 150, 0 ).n );
        }
    }
    for ( i32 y( 0 ); y < maze.n_cell_y; y++ ) {
        for ( i32 x( 0 ); x < maze.n_cell_x; x++ ) {
            const auto &conns = maze.conns.at( Vec2{ x, y } );
            const auto  color = Rgb( 200, 200, 200 );

            if ( !maze.conns.count( Vec2{ x - 1, y } )
                 || !conns.count( Vec2{ x - 1, y } ) ) {
                api::draw_line( x * cw,
                                y * cw,
                                x * cw,
                                ( y + 1 ) * cw,
                                color.n );
            }
            if ( !maze.conns.count( Vec2{ x + 1, y } )
                 || !conns.count( Vec2{ x + 1, y } ) ) {
                api::draw_line( ( x + 1 ) * cw,
                                y * cw,
                                ( x + 1 ) * cw,
                                ( y + 1 ) * cw,
                                color.n );
            }
            if ( !maze.conns.count( Vec2{ x, y + 1 } )
                 || !conns.count( Vec2{ x, y + 1 } ) ) {
                api::draw_line( x * cw,
                                ( y + 1 ) * cw,
                                ( x + 1 ) * cw,
                                ( y + 1 ) * cw,
                                color.n );
            }
            if ( !maze.conns.count( Vec2{ x, y - 1 } )
                 || !conns.count( Vec2{ x, y - 1 } ) ) {
                api::draw_line( x * cw,
                                y * cw,
                                ( x + 1 ) * cw,
                                y * cw,
                                color.n );
            }
        }
    }
}
}
