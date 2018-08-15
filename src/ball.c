#include "ball.h"
#include "familib/familib.h"
#include "global.h"
#include "paddle.h"
#include "playfield.h"

void ball_draw(void) { oam_buffer_spr(match.ball.pos.x, match.ball.pos.y, BALL_SPR_ON, 0, BALL_SPR_NUM); }

void ball_move(void) {
    if (match.ball.pos.y < PF_TOP) {
        // playsfx
        match.ball.pos.y   = PF_TOP;
        match.ball.speed.y = -match.ball.speed.y;
    } else if (match.ball.pos.y > PF_BOTTOM - BALL_HEIGHT_PX) {
        // playsfx
        match.ball.pos.y   = PF_BOTTOM - BALL_HEIGHT_PX - 1;
        match.ball.speed.y = -match.ball.speed.y;
    } else if (match.ball.pos.x < PF_LEFT || match.ball.pos.x > PF_RIGHT) {
        match.ball.out = true;
    } else if (match.ball.pos.x < match.paddle[0].pos.x + PADDLE_WIDTH_PX * 2 && match.ball.pos.y < match.paddle[0].pos.y + PADDLE_HEIGHT_PX &&
               match.ball.pos.y + BALL_HEIGHT_PX > match.paddle[0].pos.y) {  // ball hits left paddle
        // playsfx
        match.ball.pos.x = match.paddle[0].pos.x + PADDLE_WIDTH_PX * 2 + 1;

        if (match.paddle[0].direction < 0 && match.ball.speed.y > BALL_MAX_SPEED_UP) {
            match.ball.speed.y -= 1;
        } else if (match.paddle[0].direction > 0 && match.ball.speed.y < BALL_MAX_SPEED_DOWN) {
            match.ball.speed.y += 1;
        }

        match.ball.speed.x = -match.ball.speed.x;
    } else if (match.ball.pos.x + BALL_WIDTH_PX > match.paddle[1].pos.x && match.ball.pos.y < match.paddle[1].pos.y + PADDLE_HEIGHT_PX &&
               match.ball.pos.y + BALL_HEIGHT_PX > match.paddle[1].pos.y) {
        // playsfx
        match.ball.pos.x = match.paddle[1].pos.x - BALL_WIDTH_PX;

        if (match.paddle[1].direction < 0 && match.ball.speed.y > BALL_MAX_SPEED_UP) {
            match.ball.speed.y -= 1;
        } else if (match.paddle[1].direction > 0 && match.ball.speed.y < BALL_MAX_SPEED_DOWN) {
            match.ball.speed.y += 1;
        }

        match.ball.speed.x = -match.ball.speed.x;
    }

    if (!match.ball.out) {
        match.ball.pos.x += match.ball.speed.x;
        match.ball.pos.y += match.ball.speed.y;
    }

    ball_draw();
}