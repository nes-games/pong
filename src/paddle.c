#include "paddle.h"
#include "familib/familib.h"
#include "global.h"
#include "playfield.h"

void paddle_draw(u8 n) {
    if (n == 0) {
        oam_buffer_spr(match.paddle[0].pos.x, match.paddle[0].pos.y, PADDLE_SPR1_ON, 0, PADDLE_SPR_1_1_NUM);
        oam_buffer_spr(match.paddle[0].pos.x, match.paddle[0].pos.y + 8, PADDLE_SPR1_ON, 0, PADDLE_SPR_1_2_NUM);
    } else {
        oam_buffer_spr(match.paddle[1].pos.x, match.paddle[1].pos.y, PADDLE_SPR2_ON, 0, PADDLE_SPR_2_1_NUM);
        oam_buffer_spr(match.paddle[1].pos.x, match.paddle[1].pos.y + 8, PADDLE_SPR2_ON, 0, PADDLE_SPR_2_2_NUM);
    }
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