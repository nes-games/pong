#include "paddle.h"
#include "familib/familib.h"
#include "global.h"
#include "playfield.h"

const u8 PADDLE_METASPR[2][9] = {
    // paddle 0
    {
        // spr 0
        0,               // x
        0,               // y
        PADDLE_SPR1_ON,  // spr
        0,               // opt
        // spr 1
        0,               // x
        8,               // y
        PADDLE_SPR1_ON,  // spr
        0,               // opt
        // eom
        0x80,
    },
    // paddle 1
    {
        // spr 0
        0,               // x
        0,               // y
        PADDLE_SPR2_ON,  // spr
        0,               // opt
        // spr 1
        0,               // x
        8,               // y
        PADDLE_SPR2_ON,  // spr
        0,               // opt
        // eom
        0x80,
    }};

void paddle_hide(u8 n) {
    oam_hide_spr(n == 0 ? 1 : 3);
    oam_hide_spr(n == 0 ? 2 : 4);
}

void paddle_draw(u8 n) {
    oam_buffer_metaspr(match.paddle[n].pos.x,  // x-offset
                       match.paddle[n].pos.y,  // y-offset
                       n == 0 ? 1 : 3,         // p1 has spr num 1, p2 has spr num 3
                       (u8 *)PADDLE_METASPR[n]);
}

void paddle_move_up(u8 n) {
    paddle_t *paddle = &match.paddle[n];

    if (paddle->pos.y > PF_TOP + PADDLE_SPEED) {
        paddle->pos.y -= PADDLE_SPEED;
        paddle->direction = -1;
        paddle_draw(n);
    }
}

void paddle_moved_down(u8 n) {
    paddle_t *paddle = &match.paddle[n];

    if (paddle->pos.y < PF_BOTTOM - PADDLE_HEIGHT_PX - PADDLE_SPEED - 1) {
        paddle->pos.y += PADDLE_SPEED;
        paddle->direction = 1;
        paddle_draw(n);
    }
}

void paddle_move_cpu(void) {
    if (match.ball.speed.x > 0 && match.ball.pos.x > (PF_LEFT + PF_RIGHT) / 3) {
        if (match.ball.pos.y >= match.paddle[1].pos.y + PADDLE_HEIGHT_PX) {
            paddle_moved_down(1);
            return;
        } else if (match.ball.pos.y + BALL_HEIGHT_PX <= match.paddle[1].pos.y) {
            paddle_move_up(1);
            return;
        }
    }

    match.paddle[1].direction = 0;
}