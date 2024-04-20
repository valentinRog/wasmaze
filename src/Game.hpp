#pragma once

#include <chrono>
#include <iostream>

#include "MazeBuilder.hpp"
#include "Rgb.hpp"
#include "api.h"
#include "type.h"

namespace game {
class Game {
    static const i32 _step_by_sec = 60;

    i32 _screen_w;
    i32 _screen_h;

    MazeBuilder _mb;

    std::chrono::steady_clock::time_point _last_update;

public:
    void init( i32 screen_w, i32 screen_h ) {
        _screen_w         = screen_w;
        _screen_h         = screen_h;
        _mb.maze.n_cell_x = 70;
        _mb.maze.n_cell_y = _mb.maze.n_cell_x
                            * ( static_cast< f32 >( _screen_h )
                                / static_cast< f32 >( _screen_w ) );
        _mb.maze.resetGrid();
        _last_update = std::chrono::high_resolution_clock::now();
    }

    void handle_mouse( i32 x, i32 y ) {
        if ( !_mb.done() ) { return; }
        const auto cw = _mb.maze.getCellWidth( _screen_w );
        Vec2 cell     = { static_cast< i32 >( static_cast< float >( x ) / cw ),
                          static_cast< i32 >( static_cast< float >( y ) / cw ) };
        if ( cell.x >= _mb.maze.n_cell_x ) { cell.x = _mb.maze.n_cell_x - 1; }
        if ( cell.x < 0 ) { cell.x = 0; }
        if ( cell.y >= _mb.maze.n_cell_y ) { cell.y = _mb.maze.n_cell_y - 1; }
        if ( cell.y < 0 ) { cell.y = 0; }
        _mb.path = _mb.maze.getShortestPath( { 0, 0 }, cell );
    }

    void update() {
        if ( _mb.done() ) { return; }
        auto t  = std::chrono::high_resolution_clock::now();
        auto dt = std::chrono::duration_cast< std::chrono::milliseconds >(
            t - _last_update );
        int steps = ( static_cast< float >( dt.count() ) / 1000.f )
                    * ( static_cast< float >( _step_by_sec ) );
        for ( auto i( 0 ); i < steps; i++ ) {
            _mb.step();
            _last_update = std::chrono::high_resolution_clock::now();
        }
    }

    void render() {
        api::draw_rect( 0, 0, _screen_w, _screen_h, Rgb( 0, 0, 0 ).n );
        update();
        const auto cw  = _mb.maze.getCellWidth( _screen_w );
        const auto pos = _mb.getPos();
        if ( pos.has_value() ) {
            api::draw_rect( pos->x * cw,
                            pos->y * cw,
                            cw,
                            cw,
                            Rgb( 200, 0, 0 ).n );
        } else {
            _render_path();
        }
        _render_maze();
    }

    void _render_maze() const {
        const auto  cw   = _mb.maze.getCellWidth( _screen_w );
        const auto &maze = _mb.maze;

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

    void _render_path() const {
        const auto cw = _mb.maze.getCellWidth( _screen_w );
        for ( auto i( 0 ); i < _mb.path.size(); i++ ) {
            struct Line {
                i32 x0;
                i32 y0;
                i32 x1;
                i32 y1;
            };
            const auto p     = _mb.path[i];
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
                const auto prev_p = _mb.path[i - 1];
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

            if ( i < _mb.path.size() - 1 ) {
                const auto next_p = _mb.path[i + 1];
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
};
}
