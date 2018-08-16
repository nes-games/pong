#include "playfield.h"
#include "ball.h"
#include "familib/familib.h"
#include "global.h"
#include "paddle.h"

const u8 DIGITS[10][5][3] = {
    // 0
    {{1, 1, 1}, {1, 0, 1}, {1, 0, 1}, {1, 0, 1}, {1, 1, 1}},
    // 1
    {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}},
    // 2
    {{1, 1, 1}, {0, 0, 1}, {1, 1, 1}, {1, 0, 0}, {1, 1, 1}},
    // 3
    {{1, 1, 1}, {0, 0, 1}, {0, 1, 1}, {0, 0, 1}, {1, 1, 1}},
    // 4
    {{1, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 0, 1}, {0, 0, 1}},
    // 5
    {{1, 1, 1}, {1, 0, 0}, {1, 1, 1}, {0, 0, 1}, {1, 1, 1}},
    // 6
    {{1, 1, 1}, {1, 0, 0}, {1, 1, 1}, {1, 0, 1}, {1, 1, 1}},
    // 7
    {{1, 1, 1}, {0, 0, 1}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}},
    // 8
    {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}, {1, 0, 1}, {1, 1, 1}},
    // 9
    {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}, {0, 0, 1}, {0, 0, 1}},
};

const u8 PAUSE_METASPR[] = {
    // spr 0
    0 << 3, 0, 'P', 0,
    // spr 1
    1 << 3, 0, 'A', 0,
    // spr 2
    2 << 3, 0, 'U', 0,
    // spr 3
    3 << 3, 0, 'S', 0,
    // spr 4
    4 << 3, 0, 'E', 0,
    // eom
    0x80};

const u8 CPU_METASPR[] = {
    // spr 0
    0 << 3, 0, 'C', 0,
    // spr 1
    1 << 3, 0, 'P', 0,
    // spr 2
    2 << 3, 0, 'U', 0,
    // eom
    0x80};

const u8 PLAYER_METASPR[] = {
    // spr 0
    0 << 3, 0, 'P', 0,
    // spr 1
    1 << 3, 0, 'L', 0,
    // spr 2
    2 << 3, 0, 'A', 0,
    // spr 3
    3 << 3, 0, 'Y', 0,
    // spr 4
    4 << 3, 0, 'E', 0,
    // spr 5
    5 << 3, 0, 'R', 0,
    // eom
    0x80};

const u8 WINS_METASPR[] = {
    // spr 0
    0 << 3, 0, 'W', 0,
    // spr 1
    1 << 3, 0, 'I', 0,
    // spr 2
    2 << 3, 0, 'N', 0,
    // spr 3
    3 << 3, 0, 'S', 0,
    // eom
    0x80};

const u8 GAME_OVER_METASPR[] = {
    // spr 0
    0 << 3, 0, 'G', 0,
    // spr 1
    1 << 3, 0, 'A', 0,
    // spr 2
    2 << 3, 0, 'M', 0,
    // spr 3
    3 << 3, 0, 'E', 0,
    // spr 4
    5 << 3, 0, 'O', 0,
    // spr 5
    6 << 3, 0, 'V', 0,
    // spr 6
    7 << 3, 0, 'E', 0,
    // spr 7
    8 << 3, 0, 'R', 0,
    // eom
    0x80};

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

void playfield_hide_pause(void) {
    oam_hide_sprites_from(PF_TEXT_SPRNUM);
}

void playfield_draw_pause(void) {
    oam_buffer_metaspr((SCREEN_WIDTH_PX - 5 * 8) / 2,  // x
                       (PF_TOP + PF_BOTTOM) / 2,       // y
                       PF_TEXT_SPRNUM,                 // num
                       (u8*)PAUSE_METASPR);
}

void playfield_draw_game_over(void) {
    oam_hide_sprites_from(PF_TEXT_SPRNUM + 8);
    oam_buffer_metaspr((SCREEN_WIDTH_PX - 9 * 8) / 2,  // x-offset
                       (PF_TOP + PF_BOTTOM) / 2,       // y-offset
                       PF_TEXT_SPRNUM,                 // sprite number for texts
                       (u8*)GAME_OVER_METASPR);
}

void playfield_draw_cpu_wins(void) {
    // draw CPU
    oam_buffer_metaspr((SCREEN_WIDTH_PX - 3 * 8) / 2,  // x-offset
                       (PF_TOP + PF_BOTTOM) / 2 - 17,  // y-offset
                       PF_TEXT_SPRNUM,                 // sprite number for texts
                       (u8*)CPU_METASPR);
    // draw WINS
    oam_buffer_metaspr((SCREEN_WIDTH_PX - 4 * 8) / 2,  // x-offset
                       (PF_TOP + PF_BOTTOM) / 2,       // y-offset
                       PF_TEXT_SPRNUM + 3,             // sprite number for texts
                       (u8*)WINS_METASPR);
}

void playfield_draw_winner(void) {
    bool p1wins = match.score[0] > match.score[1];
    ball_hide();
    paddle_hide(0);
    paddle_hide(1);

    if (game.vs_cpu && !p1wins) {
        playfield_draw_cpu_wins();
        return;
    }

    // draw PLAYER
    oam_buffer_metaspr((SCREEN_WIDTH_PX - 8 * 8) / 2,  // x-offset
                       (PF_TOP + PF_BOTTOM) / 2 - 17,  // y-offset
                       PF_TEXT_SPRNUM,                 // sprite number for texts
                       (u8*)PLAYER_METASPR);
    // draw 1 or 2
    oam_buffer_spr((SCREEN_WIDTH_PX) / 2 + 3 * 8,  // x-offset
                   (PF_TOP + PF_BOTTOM) / 2 - 17,  // y-offset
                   p1wins ? '1' : '2',             // sprite 1 or 2
                   0,                              // no options
                   PF_TEXT_SPRNUM + 6              // sprite number for texts
    );
    // draw WINS
    oam_buffer_metaspr((SCREEN_WIDTH_PX - 4 * 8) / 2,  // x-offset
                       (PF_TOP + PF_BOTTOM) / 2,       // y-offset
                       PF_TEXT_SPRNUM + 7,             // sprite number for texts
                       (u8*)WINS_METASPR);
}

void playfield_draw_ball_n_paddles(void) {
    ball_draw();
    paddle_draw(0);
    paddle_draw(1);
}

void playfield_draw_score(u8 n) {
    u8 col   = PF_SCORE_COL + n * (SCREEN_WIDTH_TILES / 2 + 1);
    u8 score = match.score[n];

    if (score == 0) {
        ppu_load_name_table_section(NT, PF_SCORE_ROW, col, PF_SCORE_DIGIT_H, PF_SCORE_DIGIT_W, (u8*)DIGITS[0]);
        ppu_load_name_table_section(NT, PF_SCORE_ROW, col + PF_SCORE_DIGIT_W + 1, PF_SCORE_DIGIT_H, PF_SCORE_DIGIT_W, (u8*)DIGITS[0]);
    } else if (score == 10) {
        ppu_load_name_table_section(NT, PF_SCORE_ROW, col, PF_SCORE_DIGIT_H, PF_SCORE_DIGIT_W, (u8*)DIGITS[1]);
        ppu_load_name_table_section(NT, PF_SCORE_ROW, col + PF_SCORE_DIGIT_W + 1, PF_SCORE_DIGIT_H, PF_SCORE_DIGIT_W, (u8*)DIGITS[0]);
    } else if (score < 10) {
        ppu_load_name_table_section(NT, PF_SCORE_ROW, col + PF_SCORE_DIGIT_W + 1, PF_SCORE_DIGIT_H, PF_SCORE_DIGIT_W, (u8*)DIGITS[score]);
    } else {
        ppu_load_name_table_section(NT, PF_SCORE_ROW, col + PF_SCORE_DIGIT_W + 1, PF_SCORE_DIGIT_H, PF_SCORE_DIGIT_W, (u8*)DIGITS[score - 10]);
    }
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

    playfield_draw_score(0);
    playfield_draw_score(1);
    playfield_draw_ball_n_paddles();

    oam_hide_sprites_from(PADDLE_SPR_LEN * 2 + 1);

    ppu_wait_vblank();
    ppu_set_bg_on();
    ppu_set_spr_on();
    ppu_update();
}

void playfield_restart(void) {
    playfield_reset_ball_n_paddles();
    playfield_draw_ball_n_paddles();
}