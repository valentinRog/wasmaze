#pragma once

#include <optional>
#include <random>
#include <stack>

#include "Maze.hpp"
#include "Vec2.hpp"
#include "type.h"

namespace game {
class MazeBuilder {
    std::stack< Vec2 >         stack;
    std::set< Vec2 >           visited;
    std::default_random_engine gen;

public:
    Maze maze;

    MazeBuilder() : gen( std::random_device()() ) {
        stack.push( { 0, 0 } );
        visited.insert( stack.top() );
    }

    void step() {
        if ( stack.size() == 0 ) { return; }
        auto                  p   = stack.top();
        std::array< Vec2, 4 > nps = { Vec2{ p.x + 1, p.y },
                                      Vec2{ p.x - 1, p.y },
                                      Vec2{ p.x, p.y + 1 },
                                      Vec2{ p.x, p.y - 1 } };
        std::shuffle( nps.begin(), nps.end(), gen );
        bool any = false;
        for ( const auto np : nps ) {
            if ( !maze.conns.count( np ) ) { continue; }
            if ( maze.conns.at( p ).count( np ) ) { continue; }
            if ( visited.count( np ) ) { continue; }
            visited.insert( np );
            stack.push( np );
            maze.conns[p].insert( np );
            maze.conns[np].insert( p );
            any = true;
            break;
        }
        if ( !any ) { stack.pop(); }
    }

    std::optional< Vec2 > getPos() const {
        if ( stack.empty() ) { return std::optional< Vec2 >(); }
        return stack.top();
    }
};

}
