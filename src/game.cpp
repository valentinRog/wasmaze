#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <map>
#include <optional>
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

const i32 step_by_sec = 300;

void handle_mouse( i32 x, i32 y ) {
    if ( !mb.done() ) { return; }
    const auto cw   = mb.maze.getCellWidth( screen.width );
    const Vec2 cell = { static_cast< i32 >( static_cast< float >( x ) / cw )
                            % mb.maze.n_cell_x,
                        static_cast< i32 >( static_cast< float >( y ) / cw )
                            % mb.maze.n_cell_y };
    mb.path         = mb.maze.getShortestPath( { 0, 0 }, cell );
}

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
        for ( auto i( 0 ); i < mb.path.size(); i++ ) {
            struct Line {
                i32 x0;
                i32 y0;
                i32 x1;
                i32 y1;
            };
            const auto p     = mb.path[i];
            const Line north = {
                .x0 = static_cast< i32 >( static_cast< f32 >( p.x ) * cw
                                          + cw / 2.f ),
                .y0 = static_cast< i32 >( static_cast< f32 >( p.y ) * cw ),
                .x1 = static_cast< i32 >( static_cast< f32 >( p.x ) * cw
                                          + cw / 2.f ),
                .y1 = static_cast< i32 >( static_cast< f32 >( p.y ) * cw
                                          + cw / 2.f ),
            };
            const Line south = {
                .x0 = static_cast< i32 >( static_cast< f32 >( p.x ) * cw
                                          + cw / 2.f ),
                .y0 = static_cast< i32 >( static_cast< f32 >( p.y ) * cw
                                          + cw / 2.f ),
                .x1 = static_cast< i32 >( static_cast< f32 >( p.x ) * cw
                                          + cw / 2.f ),
                .y1 = static_cast< i32 >( static_cast< f32 >( p.y + 1 ) * cw ),
            };
            const Line west = {
                .x0 = static_cast< i32 >( static_cast< f32 >( p.x ) * cw ),
                .y0 = static_cast< i32 >( static_cast< f32 >( p.y ) * cw
                                          + cw / 2.f ),
                .x1 = static_cast< i32 >( static_cast< f32 >( p.x ) * cw
                                          + cw / 2.f ),
                .y1 = static_cast< i32 >( static_cast< f32 >( p.y ) * cw
                                          + cw / 2.f ),
            };
            const Line east = {
                .x0 = static_cast< i32 >( static_cast< f32 >( p.x ) * cw
                                          + cw / 2.f ),
                .y0 = static_cast< i32 >( static_cast< f32 >( p.y ) * cw
                                          + cw / 2.f ),
                .x1 = static_cast< i32 >( static_cast< f32 >( p.x + 1 ) * cw ),
                .y1 = static_cast< i32 >( static_cast< f32 >( p.y ) * cw
                                          + cw / 2.f ),
            };

            if ( i ) {
                const auto prev_p = mb.path[i - 1];
                if ( p.x < prev_p.x ) {
                    api::draw_line( east.x0,
                                    east.y0,
                                    east.x1,
                                    east.y1,
                                    Rgb( 0, 200, 0 ).n );
                } else if ( p.x > prev_p.x ) {
                    api::draw_line( west.x0,
                                    west.y0,
                                    west.x1,
                                    west.y1,
                                    Rgb( 0, 200, 0 ).n );
                } else if ( p.y > prev_p.y ) {
                    api::draw_line( north.x0,
                                    north.y0,
                                    north.x1,
                                    north.y1,
                                    Rgb( 0, 200, 0 ).n );
                } else {
                    api::draw_line( south.x0,
                                    south.y0,
                                    south.x1,
                                    south.y1,
                                    Rgb( 0, 200, 0 ).n );
                }
            }

            if ( i < mb.path.size() - 1 ) {
                const auto next_p = mb.path[i + 1];
                if ( p.x < next_p.x ) {
                    api::draw_line( east.x0,
                                    east.y0,
                                    east.x1,
                                    east.y1,
                                    Rgb( 0, 200, 0 ).n );
                } else if ( p.x > next_p.x ) {
                    api::draw_line( west.x0,
                                    west.y0,
                                    west.x1,
                                    west.y1,
                                    Rgb( 0, 200, 0 ).n );
                } else if ( p.y > next_p.y ) {
                    api::draw_line( north.x0,
                                    north.y0,
                                    north.x1,
                                    north.y1,
                                    Rgb( 0, 200, 0 ).n );
                } else {
                    api::draw_line( south.x0,
                                    south.y0,
                                    south.x1,
                                    south.y1,
                                    Rgb( 0, 200, 0 ).n );
                }
            }
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
