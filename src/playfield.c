#include "playfield.h"
#include "ball.h"
#include "familib/familib.h"
#include "global.h"
#include "paddle.h"

const u8 d0[5][3] = {{1, 1, 1}, {1, 0, 1}, {1, 0, 1}, {1, 0, 1}, {1, 1, 1}};
const u8 DIGITS[16][5][3] = {
    // 0
    {{1, 1, 1}, {1, 0, 1}, {1, 0, 1}, {1, 0, 1}, {1, 1, 1}},
    // 1
    {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}},
    // 2
    {{1, 1, 1}, {0, 0, 1}, {1, 1, 1}, {1, 0, 0}, {1, 1, 1}},
    // 3
    {{1, 1, 1}, {1, 0, 0}, {1, 1, 0}, {1, 0, 0}, {1, 1, 1}},
    // 4
    {{1, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 0, 1}, {0, 0, 1}},
    // 5
    {{1, 1, 1}, {1, 0, 0}, {1, 1, 1}, {0, 0, 1}, {1, 1, 1}},
    // 6
    {{1, 1, 1}, {1, 0, 0}, {1, 1, 1}, {1, 0, 1}, {1, 1, 1}},
    // 7
    {{1, 1, 1}, {0, 0, 1}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}},
    // 8
    {{1, 1, 1}, {1, 0, 1}, {1, 1, 0}, {1, 0, 1}, {1, 1, 1}},
    // 9
    {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}, {0, 0, 1}, {0, 0, 1}},
    // A
    {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}, {1, 0, 1}, {1, 0, 1}},
    // B
    {{1, 1, 0}, {1, 0, 1}, {1, 1, 1}, {1, 0, 1}, {1, 1, 1}},
    // C
    {{1, 1, 1}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 1, 1}},
    // D
    {{1, 1, 0}, {1, 0, 1}, {1, 0, 1}, {1, 0, 1}, {1, 1, 0}},
    // E
    {{1, 1, 1}, {1, 0, 0}, {1, 1, 1}, {1, 0, 0}, {1, 1, 1}},
    // F
    {{1, 1, 1}, {1, 0, 0}, {1, 1, 0}, {1, 0, 0}, {1, 0, 0}}};

match_t match;

void playfield_reset_ball_n_paddles(void) {
    match.ball.pos.x   = (PF_LEFT + PF_RIGHT - BALL_WIDTH_PX) / 2;
    match.ball.pos.y   = (PF_TOP + PF_BOTTOM - BALL_HEIGHT_PX) / 2;
    match.ball.speed.x = 0;
    match.ball.speed.y = 0;
    match.ball.out     = false;

    match.paddle[0].pos.x     = PF_LEFT;
    match.paddle[0].pos.y     = (PF_TOP + PF_BOTTOM - PADDLE_HEIGHT_PX) / 2;
    match.paddle[0].direction = 0;

    match.paddle[1].pos.x     = PF_RIGHT - PADDLE_WIDTH_PX;
    match.paddle[1].pos.y     = (PF_TOP + PF_BOTTOM - PADDLE_HEIGHT_PX) / 2;
    match.paddle[1].direction = 0;
}

void playfield_reset(void) {
    match.score[0] = 0;
    match.score[1] = 0;
    playfield_reset_ball_n_paddles();
}

void playfield_draw_ball_n_paddles(void) {
    ball_draw();
    paddle_draw(0);
    paddle_draw(1);
}

void playfield_draw_score1(void) {
    ppu_load_name_table_section(NT, 4, 07, 5, 3, (u8*)DIGITS[match.score[0]]);
}

void playfield_draw_score2(void) {
    ppu_load_name_table_section(NT, 4, 21, 5, 3, (u8*)DIGITS[match.score[1]]);
}

void playfield_draw(void) {
    static u8 row, col;
    ppu_set_bg_off();
    ppu_set_spr_off();
    ppu_update();

    ppu_load_bg_pal(PAL);
    ppu_load_spr_pal(PAL);

    // clear screen
    for (row = 0; row < SCREEN_HEIGHT_TILES; row++) {
        for (col = 0; col < SCREEN_WIDTH_TILES; col++) {
            ppu_load_name_table_tile(PF_TILE_OFF, NT, row, col);
        }
    }

    // top and bottom borders
    for (col = 0; col < SCREEN_WIDTH_TILES; col++) {
        ppu_load_name_table_tile(PF_TILE_ON, NT, 1, col);
        ppu_load_name_table_tile(PF_TILE_ON, NT, PF_BOTTOM / 8, col);
    }

    // middle line
    for (row = PF_TOP / 8; row < PF_BOTTOM / 8; row += 3) {
        ppu_load_name_table_tile(2, NT, row, SCREEN_WIDTH_TILES / 2 - 1);
        ppu_load_name_table_tile(3, NT, row, SCREEN_WIDTH_TILES / 2);
    }

    ppu_load_attr_table(NT, ATR);

    playfield_draw_score1();
    playfield_draw_score2();
    playfield_draw_ball_n_paddles();

    oam_hide_sprites_from(PADDLE_SPR_2_2_NUM + 1);

    ppu_wait_vblank();
    ppu_set_bg_on();
    ppu_set_spr_on();
    ppu_update();
}

void playfield_restart(void) {
    playfield_reset_ball_n_paddles();
    playfield_draw_ball_n_paddles();
}