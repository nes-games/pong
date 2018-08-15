#include "state.h"
#include "global.h"

void state_next(void) {
    if (++game.state.current > STATE_LAST) {
        game.state.current = STATE_FIRST;
    }

    game.state.start = true;
}

void state_goto(u8 state) {
    game.state.current = state;
    game.state.start = true;
}